/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "SkipSpellsListAction.h"

#include "Event.h"
#include "LootAction.h"
#include "Playerbots.h"
#include "SkipSpellsListValue.h"

bool SkipSpellsListAction::Execute(Event event)
{
    std::string cmd = event.getParam();
    std::set<uint32>& skipSpells = AI_VALUE(std::set<uint32>&, "skip spells list");

    SpellIds spellIds = parseIds(cmd);
    if (!spellIds.empty())
    {
        skipSpells.clear();
        for (uint32 spellId : spellIds)
        {
            skipSpells.insert(spellId);
        }

        cmd = "?";
    }

    if (cmd == "reset")
    {
        skipSpells.clear();
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_ignored_spell_list_empty", "Ignored spell list is empty"));
        return true;
    }

    if (cmd.empty() || cmd == "?")
    {
        if (skipSpells.empty())
        {
            botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_ignored_spell_list_empty", "Ignored spell list is empty"));
            return true;
        }

        std::string listStr;
        bool first = true;
        for (uint32 spellId : skipSpells)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
            if (!spellInfo)
                continue;
            if (!first)
                listStr += ", ";
            listStr += chat->FormatSpell(spellInfo);
            first = false;
        }
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_ignored_spell_list", "Ignored spell list: %list", {{"%list", listStr}}));
    }
    else
    {
        bool remove = cmd.size() > 1 && cmd.substr(0, 1) == "-";
        if (remove)
            cmd = cmd.substr(1);

        uint32 spellId = chat->parseSpell(cmd);
        if (!spellId)
        {
            botAI->TellError(botAI->GetLocalizedBotTextOrDefault("error_unknown_spell", "Unknown spell"));
            return false;
        }

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
            return false;

        if (remove)
        {
            std::set<uint32>::iterator j = skipSpells.find(spellId);
            if (j != skipSpells.end())
            {
                skipSpells.erase(j);

                botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_spell_removed_from_ignored", "%spell removed from ignored spells", {{"%spell", chat->FormatSpell(spellInfo)}}));
                return true;
            }
        }
        else
        {
            std::set<uint32>::iterator j = skipSpells.find(spellId);
            if (j == skipSpells.end())
            {
                skipSpells.insert(spellId);

                botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_spell_added_to_ignored", "%spell added to ignored spells", {{"%spell", chat->FormatSpell(spellInfo)}}));
                return true;
            }
        }
    }

    return false;
}

SpellIds SkipSpellsListAction::parseIds(std::string const text)
{
    SpellIds spellIds;

    uint8 pos = 0;
    while (pos < text.size())
    {
        int32 endPos = text.find(',', pos);
        if (endPos == -1)
            endPos = text.size();

        std::string const idC = text.substr(pos, endPos - pos);
        uint32 id = atol(idC.c_str());
        pos = endPos + 1;
        if (id)
            spellIds.insert(id);
    }

    return spellIds;
}
