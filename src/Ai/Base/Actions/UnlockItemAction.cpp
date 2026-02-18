#include "UnlockItemAction.h"
#include "PlayerbotAI.h"
#include "ItemTemplate.h"
#include "WorldPacket.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "SpellInfo.h"

#define PICK_LOCK_SPELL_ID 1804

bool UnlockItemAction::Execute(Event event)
{
    bool foundLockedItem = false;

    Item* item = botAI->FindLockedItem();
    if (item)
    {
        UnlockItem(item);
        foundLockedItem = true;
    }

    return foundLockedItem;
}

void UnlockItemAction::UnlockItem(Item* item)
{
    // Use CastSpell to unlock the item
    if (botAI->CastSpell(PICK_LOCK_SPELL_ID, bot, item))
    {
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_used_pick_lock_on", "Used Pick Lock on: %item", {{"%item", item->GetTemplate()->Name1}}));
    }
    else
    {
        botAI->TellError(botAI->GetLocalizedBotTextOrDefault("error_failed_cast_pick_lock", "Failed to cast Pick Lock."));
    }
}
