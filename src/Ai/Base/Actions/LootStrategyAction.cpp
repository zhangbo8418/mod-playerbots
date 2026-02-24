/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "LootStrategyAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "LootAction.h"
#include "LootObjectStack.h"
#include "LootStrategyValue.h"
#include "Playerbots.h"

bool LootStrategyAction::Execute(Event event)
{
    std::string const strategy = event.getParam();

    LootObjectStack* lootItems = AI_VALUE(LootObjectStack*, "available loot");
    std::set<uint32>& alwaysLootItems = AI_VALUE(std::set<uint32>&, "always loot list");
    Value<LootStrategy*>* lootStrategy = context->GetValue<LootStrategy*>("loot strategy");

    if (strategy == "?")
    {
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_loot_strategy", "Loot strategy: %name", {{"%name", lootStrategy->Get()->GetName()}}));

        {
            std::string listStr;
            for (uint32 itemId : alwaysLootItems)
            {
                ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
                if (!proto)
                    continue;
                if (!listStr.empty())
                    listStr += " ";
                listStr += chat->FormatItem(proto);
            }
            botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_always_loot_items", "Always loot items: %list", {{"%list", listStr}}));
        }
    }
    else
    {
        ItemIds items = chat->parseItems(strategy);

        if (items.size() == 0)
        {
            lootStrategy->Set(LootStrategyValue::instance(strategy));

            botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_loot_strategy_set", "Loot strategy set to %name", {{"%name", lootStrategy->Get()->GetName()}}));
            return true;
        }

        bool remove = strategy.size() > 1 && strategy.substr(0, 1) == "-";
        bool query = strategy.size() > 1 && strategy.substr(0, 1) == "?";
        for (uint32 itemid : items)
        {
            if (query)
            {
                if (ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemid))
                {
                    std::string key = StoreLootAction::IsLootAllowed(itemid, botAI) ? "msg_will_loot" : "msg_wont_loot";
                    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault(key, key == "msg_will_loot" ? "|cFF000000Will loot %item" : "|c00FF0000Won't loot %item", {{"%item", ChatHelper::FormatItem(proto)}}));
                }
            }
            else if (remove)
            {
                std::set<uint32>::iterator j = alwaysLootItems.find(itemid);
                if (j != alwaysLootItems.end())
                    alwaysLootItems.erase(j);

                botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_loot_removed_from_always", "Item(s) removed from always loot list"));
            }
            else
            {
                alwaysLootItems.insert(itemid);
                botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_loot_added_to_always", "Item(s) added to always loot list"));
            }
        }
    }

    return true;
}
