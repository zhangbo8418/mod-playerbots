/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "TellCastFailedAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "Playerbots.h"

bool TellCastFailedAction::Execute(Event event)
{
    WorldPacket p(event.getPacket());
    p.rpos(0);
    uint8 castCount, result;
    uint32 spellId;
    p >> castCount >> spellId >> result;
    botAI->SpellInterrupted(spellId);

    if (result == SPELL_CAST_OK)
        return false;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);

    std::string reason;
    switch (result)
    {
        case SPELL_FAILED_NOT_READY:
            reason = botAI->GetLocalizedBotTextOrDefault("msg_spell_failed_not_ready", "not ready");
            break;
        case SPELL_FAILED_REQUIRES_SPELL_FOCUS:
            reason = botAI->GetLocalizedBotTextOrDefault("msg_spell_failed_focus", "requires spell focus");
            break;
        case SPELL_FAILED_REQUIRES_AREA:
            reason = botAI->GetLocalizedBotTextOrDefault("msg_spell_failed_area", "cannot cast here");
            break;
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS:
            reason = botAI->GetLocalizedBotTextOrDefault("msg_spell_failed_item", "requires item");
            break;
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND:
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND:
            reason = botAI->GetLocalizedBotTextOrDefault("msg_spell_failed_weapon", "requires weapon");
            break;
        case SPELL_FAILED_PREVENTED_BY_MECHANIC:
            reason = botAI->GetLocalizedBotTextOrDefault("msg_spell_failed_interrupted", "interrupted");
            break;
        default:
            reason = botAI->GetLocalizedBotTextOrDefault("msg_spell_failed_default", "cannot cast");
    }

    if (spellInfo->CalcCastTime() >= 2000)
    {
        std::string msg = chat->FormatSpell(spellInfo) + ": " + reason;
        botAI->TellError(msg);
    }

    return true;
}

bool TellSpellAction::Execute(Event event)
{
    std::string const spell = event.getParam();
    uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
    if (!spellId)
        return false;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return false;

    botAI->TellError(chat->FormatSpell(spellInfo));
    return true;
}
