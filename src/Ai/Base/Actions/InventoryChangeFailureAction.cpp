/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "InventoryChangeFailureAction.h"

#include "Event.h"
#include "Playerbots.h"

namespace
{
struct EquipErrText
{
    char const* key;
    char const* defaultText;
};

std::map<InventoryResult, EquipErrText> messages;

EquipErrText const ERR_NO_SLOT = {"msg_equip_err_no_slot", "No equipment slot available"};
EquipErrText const ERR_NEVER_USE = {"msg_equip_err_never_use", "I will never be able to use this"};
EquipErrText const ERR_WRONG_BAG = {"msg_equip_err_wrong_bag", "This item cannot go in this bag"};
EquipErrText const ERR_CANT_STACK = {"msg_equip_err_cant_stack", "Item cannot stack"};
EquipErrText const ERR_NOT_FOUND = {"msg_equip_err_not_found", "Cannot find the item"};
EquipErrText const ERR_CANT_WRAP = {"msg_equip_err_cant_wrap", "Item can't be wrapped"};
EquipErrText const ERR_BAGS_FULL = {"msg_equip_err_bags_full", "My bags are full"};
EquipErrText const ERR_BANK_FULL = {"msg_equip_err_bank_full", "My bank is full"};
EquipErrText const ERR_SOLD_OUT = {"msg_equip_err_sold_out", "Item item is sold out"};

void InitInventoryChangeFailureMessages()
{
    messages[EQUIP_ERR_CANT_EQUIP_LEVEL_I] = {"msg_equip_err_level_low", "My level is too low"};
    messages[EQUIP_ERR_CANT_EQUIP_SKILL] = {"msg_equip_err_skill_low", "My skill level is too low"};
    messages[EQUIP_ERR_ITEM_DOESNT_GO_TO_SLOT] = {"msg_equip_err_invalid_slot", "Invalid slot"};
    messages[EQUIP_ERR_BAG_FULL] = ERR_BAGS_FULL;
    messages[EQUIP_ERR_NONEMPTY_BAG_OVER_OTHER_BAG] = {"msg_equip_err_bag_not_empty", "This bag is not empty"};
    messages[EQUIP_ERR_CANT_TRADE_EQUIP_BAGS] = {"msg_equip_err_cant_trade_bags", "Cannot trade equipped bags"};
    messages[EQUIP_ERR_ONLY_AMMO_CAN_GO_HERE] = {"msg_equip_err_ammo_only", "Invalid slot (only ammo is required)"};
    messages[EQUIP_ERR_NO_REQUIRED_PROFICIENCY] = {"msg_equip_err_no_proficiency", "I don't have necessary skill"};
    messages[EQUIP_ERR_NO_EQUIPMENT_SLOT_AVAILABLE] = ERR_NO_SLOT;
    messages[EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM] = ERR_NEVER_USE;
    messages[EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM2] = ERR_NEVER_USE;
    messages[EQUIP_ERR_NO_EQUIPMENT_SLOT_AVAILABLE2] = ERR_NO_SLOT;
    messages[EQUIP_ERR_CANT_EQUIP_WITH_TWOHANDED] = {"msg_equip_err_two_handed", "Cannot equip with two-handed weapon equipped"};
    messages[EQUIP_ERR_CANT_DUAL_WIELD] = {"msg_equip_err_dual_wield", "I cannot dual-wield"};
    messages[EQUIP_ERR_ITEM_DOESNT_GO_INTO_BAG] = ERR_WRONG_BAG;
    messages[EQUIP_ERR_ITEM_DOESNT_GO_INTO_BAG2] = ERR_WRONG_BAG;
    messages[EQUIP_ERR_CANT_CARRY_MORE_OF_THIS] = {"msg_equip_err_cant_carry_more", "I can't carry anymore of those"};
    messages[EQUIP_ERR_NO_EQUIPMENT_SLOT_AVAILABLE3] = ERR_NO_SLOT;
    messages[EQUIP_ERR_ITEM_CANT_STACK] = ERR_CANT_STACK;
    messages[EQUIP_ERR_ITEM_CANT_BE_EQUIPPED] = {"msg_equip_err_cant_equip", "Item cannot be equipped"};
    messages[EQUIP_ERR_ITEMS_CANT_BE_SWAPPED] = {"msg_equip_err_cant_swap", "Cannot swap these items"};
    messages[EQUIP_ERR_SLOT_IS_EMPTY] = {"msg_equip_err_empty_slot", "Nothing to equip"};
    messages[EQUIP_ERR_ITEM_NOT_FOUND] = ERR_NOT_FOUND;
    messages[EQUIP_ERR_CANT_DROP_SOULBOUND] = {"msg_equip_err_soulbound", "Cannot drop soulbound items"};
    messages[EQUIP_ERR_OUT_OF_RANGE] = {"msg_equip_err_out_of_range", "I am out of range"};
    messages[EQUIP_ERR_TRIED_TO_SPLIT_MORE_THAN_COUNT] = {"msg_equip_err_invalid_split", "Invalid split number"};
    messages[EQUIP_ERR_COULDNT_SPLIT_ITEMS] = {"msg_equip_err_cant_split", "Cannot split this"};
    messages[EQUIP_ERR_MISSING_REAGENT] = {"msg_equip_err_missing_reagent", "Missing reagents"};
    messages[EQUIP_ERR_NOT_ENOUGH_MONEY] = {"msg_equip_err_not_enough_money", "Not enough money"};
    messages[EQUIP_ERR_NOT_A_BAG] = {"msg_equip_err_not_a_bag", "This is not a bag"};
    messages[EQUIP_ERR_CAN_ONLY_DO_WITH_EMPTY_BAGS] = {"msg_equip_err_bag_not_empty_alt", "The bag is not empty"};
    messages[EQUIP_ERR_DONT_OWN_THAT_ITEM] = {"msg_equip_err_not_my_item", "This is not my item"};
    messages[EQUIP_ERR_CAN_EQUIP_ONLY1_QUIVER] = {"msg_equip_err_quiver_only", "Only quiver can be equipped"};
    messages[EQUIP_ERR_MUST_PURCHASE_THAT_BAG_SLOT] = {"msg_equip_err_purchase_slot", "I must purchase the slot before"};
    messages[EQUIP_ERR_TOO_FAR_AWAY_FROM_BANK] = {"msg_equip_err_far_from_bank", "I am too far away from bank"};
    messages[EQUIP_ERR_ITEM_LOCKED] = {"msg_equip_err_item_locked", "This item is locked"};
    messages[EQUIP_ERR_YOU_ARE_STUNNED] = {"msg_equip_err_stunned", "I am stunned"};
    messages[EQUIP_ERR_YOU_ARE_DEAD] = {"msg_equip_err_dead", "I am dead"};
    messages[EQUIP_ERR_CANT_DO_RIGHT_NOW] = {"msg_equip_err_cant_do_now", "I can't do this right now"};
    messages[EQUIP_ERR_INT_BAG_ERROR] = {"msg_equip_err_internal", "Internal error"};
    messages[EQUIP_ERR_CAN_EQUIP_ONLY1_BOLT] = {"msg_equip_err_bolts_only", "Only bolts are allowed"};
    messages[EQUIP_ERR_CAN_EQUIP_ONLY1_AMMOPOUCH] = {"msg_equip_err_ammo_pouch", "Ammo poach is allowed"};
    messages[EQUIP_ERR_STACKABLE_CANT_BE_WRAPPED] = ERR_CANT_WRAP;
    messages[EQUIP_ERR_EQUIPPED_CANT_BE_WRAPPED] = ERR_CANT_WRAP;
    messages[EQUIP_ERR_WRAPPED_CANT_BE_WRAPPED] = ERR_CANT_WRAP;
    messages[EQUIP_ERR_BOUND_CANT_BE_WRAPPED] = ERR_CANT_WRAP;
    messages[EQUIP_ERR_UNIQUE_CANT_BE_WRAPPED] = ERR_CANT_WRAP;
    messages[EQUIP_ERR_BAGS_CANT_BE_WRAPPED] = ERR_CANT_WRAP;
    messages[EQUIP_ERR_ALREADY_LOOTED] = {"msg_equip_err_already_looted", "Already looted"};
    messages[EQUIP_ERR_INVENTORY_FULL] = {"msg_equip_err_inventory_full", "My inventory is full"};
    messages[EQUIP_ERR_BANK_FULL] = ERR_BANK_FULL;
    messages[EQUIP_ERR_ITEM_IS_CURRENTLY_SOLD_OUT] = ERR_SOLD_OUT;
    messages[EQUIP_ERR_BAG_FULL3] = ERR_BANK_FULL;
    messages[EQUIP_ERR_ITEM_NOT_FOUND2] = ERR_NOT_FOUND;
    messages[EQUIP_ERR_ITEM_CANT_STACK2] = ERR_CANT_STACK;
    messages[EQUIP_ERR_BAG_FULL4] = ERR_BAGS_FULL;
    messages[EQUIP_ERR_ITEM_SOLD_OUT] = ERR_SOLD_OUT;
    messages[EQUIP_ERR_OBJECT_IS_BUSY] = {"msg_equip_err_object_busy", "This object is busy"};
    messages[EQUIP_ERR_NOT_IN_COMBAT] = {"msg_equip_err_in_combat", "I am in combat"};
    messages[EQUIP_ERR_NOT_WHILE_DISARMED] = {"msg_equip_err_disarmed", "Cannot do while disarmed"};
    messages[EQUIP_ERR_BAG_FULL6] = ERR_BAGS_FULL;
    messages[EQUIP_ERR_CANT_EQUIP_RANK] = {"msg_equip_err_rank", "Not enough rank"};
    messages[EQUIP_ERR_CANT_EQUIP_REPUTATION] = {"msg_equip_err_reputation", "Not enough reputation"};
    messages[EQUIP_ERR_TOO_MANY_SPECIAL_BAGS] = {"msg_equip_err_too_many_special_bags", "Too many special bags"};
    messages[EQUIP_ERR_LOOT_CANT_LOOT_THAT_NOW] = {"msg_equip_err_cant_loot_now", "Cannot loot this right now"};
}
} // namespace

bool InventoryChangeFailureAction::Execute(Event event)
{
    if (!botAI->GetMaster())
        return false;

    if (messages.empty())
        InitInventoryChangeFailureMessages();

    WorldPacket p(event.getPacket());
    p.rpos(0);
    uint8 err;
    p >> err;
    if (err == EQUIP_ERR_OK)
        return false;

    auto const it = messages.find(static_cast<InventoryResult>(err));
    if (it == messages.end() || !it->second.key)
        return false;

    EquipErrText const& text = it->second;
    botAI->TellError(botAI->GetLocalizedBotTextOrDefault(text.key, text.defaultText));
    return true;
}
