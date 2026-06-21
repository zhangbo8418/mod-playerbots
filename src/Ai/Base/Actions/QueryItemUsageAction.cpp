/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "QueryItemUsageAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "ItemUsageValue.h"
#include "Playerbots.h"

bool QueryItemUsageAction::Execute(Event event)
{
    std::string param = event.getParam();
    if (param.empty())
    {
        return false;
    }

    // Use parseItems() to extract item IDs from the input
    ItemIds itemIds = chat->parseItems(param);
    if (itemIds.empty())
    {
        return false;
    }

    // Process each extracted item ID (assuming single-item queries for now)
    for (uint32 itemId : itemIds)
    {
        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemId);
        if (!itemTemplate)
            continue;

        uint32 count = GetCount(itemTemplate);
        uint32 total = bot->GetItemCount(itemTemplate->ItemId, true);
        std::string itemInfo = QueryItem(itemTemplate, count, total);

        botAI->TellMaster(itemInfo);
        return true; // Only process the first valid item
    }

    return false;
}

uint32 QueryItemUsageAction::GetCount(ItemTemplate const* item)
{
    uint32 total = 0;

    std::vector<Item*> items = InventoryAction::parseItems(item->Name1);
    if (!items.empty())
    {
        for (std::vector<Item*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            total += (*i)->GetCount();
        }
    }

    return total;
}

std::string const QueryItemUsageAction::QueryItem(ItemTemplate const* item, uint32 count, uint32 total)
{
    std::ostringstream out;
    std::string usage = QueryItemUsage(item);
    std::string const quest = QueryQuestItem(item->ItemId);
    std::string const price = QueryItemPrice(item);
    if (usage.empty())
        usage = quest.empty() ? botAI->GetLocalizedBotTextOrDefault("msg_item_usage_useless", "Useless")
                              : botAI->GetLocalizedBotTextOrDefault("msg_item_usage_quest", "Quest");

    out << chat->FormatItem(item, count, total) << ": " << usage;
    if (!quest.empty())
        out << ", " << quest;

    if (!price.empty())
        out << ", " << price;

    return out.str();
}

std::string const QueryItemUsageAction::QueryItemUsage(ItemTemplate const* item)
{
    std::ostringstream out;
    out << item->ItemId;
    ItemUsage usage = AI_VALUE2(ItemUsage, "item usage", out.str());
    switch (usage)
    {
        case ITEM_USAGE_EQUIP:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_equip", "Equip");
        case ITEM_USAGE_REPLACE:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_equip_replace", "Equip (replace)");
        case ITEM_USAGE_BAD_EQUIP:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_equip_temp", "Equip (temporary)");
        case ITEM_USAGE_BROKEN_EQUIP:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_broken_equip", "Broken Equip");
        case ITEM_USAGE_QUEST:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_quest_other", "Quest (other)");
        case ITEM_USAGE_SKILL:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_tradeskill", "Tradeskill");
        case ITEM_USAGE_USE:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_use", "Use");
        case ITEM_USAGE_GUILD_TASK:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_guild_task", "Guild task");
        case ITEM_USAGE_DISENCHANT:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_disenchant", "Disenchant");
        case ITEM_USAGE_VENDOR:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_vendor", "Vendor");
        case ITEM_USAGE_AH:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_auctionhouse", "Auctionhouse");
        case ITEM_USAGE_AMMO:
            return botAI->GetLocalizedBotTextOrDefault("msg_item_usage_ammunition", "Ammunition");
        default:
            break;
    }

    return "";
}

std::string const QueryItemUsageAction::QueryItemPrice(ItemTemplate const* item)
{
    if (!sRandomPlayerbotMgr.IsRandomBot(bot))
        return "";

    if (item->Bonding == BIND_WHEN_PICKED_UP)
        return "";

    std::ostringstream msg;
    std::vector<Item*> items = InventoryAction::parseItems(item->Name1);
    int32 sellPrice = 0;
    if (!items.empty())
    {
        for (std::vector<Item*>::iterator i = items.begin(); i != items.end(); ++i)
        {
            Item* sell = *i;
            int32 price =
                sell->GetCount() * sell->GetTemplate()->SellPrice * sRandomPlayerbotMgr.GetSellMultiplier(bot);
            if (!sellPrice || sellPrice > price)
                sellPrice = price;
        }
    }
    if (sellPrice)
        msg << botAI->GetLocalizedBotTextOrDefault("msg_item_price_sell", "Sell: %money",
            {{"%money", chat->formatMoney(sellPrice)}});

    std::ostringstream out;
    out << item->ItemId;
    ItemUsage usage = AI_VALUE2(ItemUsage, "item usage", out.str());
    if (usage == ITEM_USAGE_NONE)
        return msg.str();

    int32 buyPrice = item->BuyPrice * sRandomPlayerbotMgr.GetBuyMultiplier(bot);
    if (buyPrice)
    {
        if (sellPrice)
            msg << " ";

        msg << botAI->GetLocalizedBotTextOrDefault("msg_item_price_buy", "Buy: %money",
            {{"%money", chat->formatMoney(buyPrice)}});
    }

    return msg.str();
}

std::string const QueryItemUsageAction::QueryQuestItem(uint32 itemId)
{
    Player* bot = botAI->GetBot();
    QuestStatusMap& questMap = bot->getQuestStatusMap();
    for (QuestStatusMap::const_iterator i = questMap.begin(); i != questMap.end(); i++)
    {
        Quest const* questTemplate = sObjectMgr->GetQuestTemplate(i->first);
        if (!questTemplate)
            continue;

        uint32 questId = questTemplate->GetQuestId();
        QuestStatus status = bot->GetQuestStatus(questId);
        if (status == QUEST_STATUS_INCOMPLETE ||
            (status == QUEST_STATUS_COMPLETE && !bot->GetQuestRewardStatus(questId)))
        {
            QuestStatusData const& questStatus = i->second;
            std::string const usage = QueryQuestItem(itemId, questTemplate, &questStatus);
            if (!usage.empty())
                return usage;
        }
    }

    return "";
}

std::string const QueryItemUsageAction::QueryQuestItem(uint32 itemId, Quest const* questTemplate,
                                                       QuestStatusData const* questStatus)
{
    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; i++)
    {
        if (questTemplate->RequiredItemId[i] != itemId)
            continue;

        uint32 required = questTemplate->RequiredItemCount[i];
        uint32 available = questStatus->ItemCount[i];
        if (!required)
            continue;

        return chat->FormatQuestObjective(chat->FormatQuest(questTemplate), available, required);
    }

    return "";
}
