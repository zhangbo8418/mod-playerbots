/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GiveItemAction.h"

#include "Event.h"
#include "ItemCountValue.h"
#include "Playerbots.h"

std::vector<std::string> split(std::string const s, char delim);

bool GiveItemAction::Execute(Event /*event*/)
{
    Unit* target = GetTarget();
    if (!target)
        return false;

    Player* receiver = dynamic_cast<Player*>(target);
    if (!receiver)
        return false;

    PlayerbotAI* receiverAi = GET_PLAYERBOT_AI(receiver);
    if (!receiverAi)
        return false;

    if (receiverAi->GetAiObjectContext()->GetValue<uint32>("item count", item)->Get())
        return true;

    bool moved = false;
    std::vector<Item*> items = InventoryAction::parseItems(item, ITERATE_ITEMS_IN_BAGS);
    for (Item* item : items)
    {
        if (receiver->CanUseItem(item->GetTemplate()) != EQUIP_ERR_OK)
            continue;

        ItemPosCountVec dest;
        InventoryResult msg = receiver->CanStoreItem(NULL_BAG, NULL_SLOT, dest, item, false);
        if (msg == EQUIP_ERR_OK)
        {
            bot->MoveItemFromInventory(item->GetBagSlot(), item->GetSlot(), true);
            item->SetOwnerGUID(target->GetGUID());
            receiver->MoveItemToInventory(dest, item, true);
            moved = true;

            receiverAi->TellMasterNoFacing(receiverAi->GetLocalizedBotTextOrDefault("msg_got_from", "Got %item from %name",
                {{"%item", chat->FormatItem(item->GetTemplate(), item->GetCount())}, {"%name", bot->GetName()}}));
        }
        else
        {
            receiverAi->TellError(receiverAi->GetLocalizedBotTextOrDefault("error_cannot_get_bags_full",
                "Cannot get %item from %name - my bags are full",
                {{"%item", chat->FormatItem(item->GetTemplate(), item->GetCount())}, {"%name", bot->GetName()}}));
        }
    }

    return true;
}

Unit* GiveItemAction::GetTarget() { return AI_VALUE2(Unit*, "party member without item", item); }

bool GiveItemAction::isUseful()
{
    return GetTarget() && AI_VALUE2(uint8, "mana", "self target") > sPlayerbotAIConfig.lowMana;
}

Unit* GiveFoodAction::GetTarget() { return AI_VALUE(Unit*, "party member without food"); }

bool GiveFoodAction::isUseful()
{
    if (!GetTarget())
        return false;

    bool isRandomBot = GetTarget()->IsPlayer() && sRandomPlayerbotMgr.IsRandomBot((Player*)GetTarget());

    return !isRandomBot || (isRandomBot && !botAI->HasCheat(BotCheatMask::food));
}

Unit* GiveWaterAction::GetTarget() { return AI_VALUE(Unit*, "party member without water"); }

bool GiveWaterAction::isUseful()
{
    if (!GetTarget())
        return false;

    bool isRandomBot = GetTarget()->IsPlayer() && sRandomPlayerbotMgr.IsRandomBot((Player*)GetTarget());

    return !isRandomBot || (isRandomBot && !botAI->HasCheat(BotCheatMask::food));
}
