/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "HelpAction.h"

#include "ChatActionContext.h"
#include "Event.h"
#include "AiObjectContext.h"

HelpAction::HelpAction(PlayerbotAI* botAI) : Action(botAI, "help") { chatContext = new ChatActionContext(); }

HelpAction::~HelpAction() { delete chatContext; }

bool HelpAction::Execute(Event /*event*/)
{
    TellChatCommands();
    TellStrategies();
    return true;
}

void HelpAction::TellChatCommands()
{
    std::string listStr = CombineSupported(chatContext->supports());
    botAI->TellError(botAI->GetLocalizedBotTextOrDefault("msg_help_whisper_any", "Whisper any of: %list, [item], [quest] or [object] link", {{"%list", listStr}}));
}

void HelpAction::TellStrategies()
{
    std::string listStr = CombineSupported(botAI->GetAiObjectContext()->GetSupportedStrategies());
    botAI->TellError(botAI->GetLocalizedBotTextOrDefault("msg_help_strategies", "Possible strategies (co/nc/dead commands): %list", {{"%list", listStr}}));
}

std::string const HelpAction::CombineSupported(std::set<std::string> commands)
{
    std::ostringstream out;

    for (std::set<std::string>::iterator i = commands.begin(); i != commands.end();)
    {
        out << *i;
        if (++i != commands.end())
            out << ", ";
    }

    return out.str();
}
