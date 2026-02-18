#include "UnlockTradedItemAction.h"
#include "Playerbots.h"
#include "TradeData.h"
#include "SpellInfo.h"

#define PICK_LOCK_SPELL_ID 1804

bool UnlockTradedItemAction::Execute(Event event)
{
    Player* trader = bot->GetTrader();
    if (!trader)
        return false; // No active trade session

    TradeData* tradeData = trader->GetTradeData();
    if (!tradeData)
        return false; // No trade data available

    Item* lockbox = tradeData->GetItem(TRADE_SLOT_NONTRADED);
    if (!lockbox)
    {
        botAI->TellError(botAI->GetLocalizedBotTextOrDefault("error_no_item_do_not_trade_slot", "No item in the Do Not Trade slot."));
        return false;
    }

    if (!CanUnlockItem(lockbox))
    {
        botAI->TellError(botAI->GetLocalizedBotTextOrDefault("error_cannot_unlock_item", "Cannot unlock this item."));
        return false;
    }

    UnlockItem(lockbox);
    return true;
}

bool UnlockTradedItemAction::CanUnlockItem(Item* item)
{
    if (!item)
        return false;

    ItemTemplate const* itemTemplate = item->GetTemplate();
    if (!itemTemplate)
        return false;

    // Ensure the bot is a rogue and has Lockpicking skill
    if (bot->getClass() != CLASS_ROGUE || !botAI->HasSkill(SKILL_LOCKPICKING))
        return false;

    // Ensure the item is actually locked
    if (itemTemplate->LockID == 0 || !item->IsLocked())
        return false;

    // Check if the bot's Lockpicking skill is high enough
    uint32 lockId = itemTemplate->LockID;
    LockEntry const* lockInfo = sLockStore.LookupEntry(lockId);
    if (!lockInfo)
        return false;

    uint32 botSkill = bot->GetSkillValue(SKILL_LOCKPICKING);
    for (uint8 j = 0; j < 8; ++j)
    {
        if (lockInfo->Type[j] == LOCK_KEY_SKILL && SkillByLockType(LockType(lockInfo->Index[j])) == SKILL_LOCKPICKING)
        {
            uint32 requiredSkill = lockInfo->Skill[j];
            if (botSkill >= requiredSkill)
                return true;
            else
            {
                botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("error_lockpick_skill_too_low",
                    "Lockpicking skill too low (%current/%required) to unlock: %item",
                    {{"%current", std::to_string(botSkill)}, {"%required", std::to_string(requiredSkill)}, {"%item", item->GetTemplate()->Name1}}));
            }
        }
    }

    return false;
}

void UnlockTradedItemAction::UnlockItem(Item* item)
{
    if (!bot->HasSpell(PICK_LOCK_SPELL_ID))
    {
        botAI->TellError(botAI->GetLocalizedBotTextOrDefault("error_pick_lock_spell_missing", "Cannot unlock, Pick Lock spell is missing."));
        return;
    }

    // Use CastSpell to unlock the item
    if (botAI->CastSpell(PICK_LOCK_SPELL_ID, bot->GetTrader(), item)) // Unit target is trader
    {
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_picking_lock_on_traded", "Picking Lock on traded item: %item", {{"%item", item->GetTemplate()->Name1}}));
    }
    else
    {
        botAI->TellError(botAI->GetLocalizedBotTextOrDefault("error_failed_cast_pick_lock", "Failed to cast Pick Lock."));
    }
}
