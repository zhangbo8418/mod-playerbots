/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GuildBankAction.h"

#include "GuildMgr.h"
#include "PlayerbotAI.h"
#include "AiObjectContext.h"

bool GuildBankAction::Execute(Event event)
{
    std::string const text = event.getParam();
    if (text.empty())
        return false;

    if (!bot->GetGuildId() || (GetMaster() && GetMaster()->GetGuildId() != bot->GetGuildId()))
    {
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("error_not_in_guild", "I'm not in your guild!"));
        return false;
    }

    GuidVector gos = *botAI->GetAiObjectContext()->GetValue<GuidVector>("nearest game objects");
    for (GuidVector::iterator i = gos.begin(); i != gos.end(); ++i)
    {
        GameObject* go = botAI->GetGameObject(*i);
        if (!go || !bot->GetGameObjectIfCanInteractWith(go->GetGUID(), GAMEOBJECT_TYPE_GUILD_BANK))
            continue;

        return Execute(text, go);
    }

    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("error_no_guild_bank_nearby", "Cannot find the guild bank nearby"));
    return false;
}

bool GuildBankAction::Execute(std::string const text, GameObject* bank)
{
    bool result = true;

    std::vector<Item*> found = parseItems(text);
    if (found.empty())
        return false;

    for (std::vector<Item*>::iterator i = found.begin(); i != found.end(); i++)
    {
        Item* item = *i;
        if (item)
            result &= MoveFromCharToBank(item, bank);
    }

    return result;
}

bool GuildBankAction::MoveFromCharToBank(Item* item, GameObject* bank)
{
    uint32 playerSlot = item->GetSlot();
    uint32 playerBag = item->GetBagSlot();

    Guild* guild = sGuildMgr->GetGuildById(bot->GetGuildId());

    if (!guild->MemberHasTabRights(bot->GetGUID(), 0, GUILD_BANK_RIGHT_DEPOSIT_ITEM))
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("error_guild_cant_put",
            "I can't put %item to guild bank. I have no rights to put items in the first guild bank tab",
            {{"%item", chat->FormatItem(item->GetTemplate())}}));
    else
    {
        guild->SwapItemsWithInventory(bot, false, 0, 255, playerBag, playerSlot, 0);
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_put_to_guild_bank", "%item put to guild bank", {{"%item", chat->FormatItem(item->GetTemplate())}}));
    }

    return true;
}
