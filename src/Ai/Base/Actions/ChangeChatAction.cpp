/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ChangeChatAction.h"

#include "Event.h"
#include "Playerbots.h"

bool ChangeChatAction::Execute(Event event)
{
    std::string const text = event.getParam();
    ChatMsg parsed = chat->parseChat(text);
    if (parsed == CHAT_MSG_SYSTEM)
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_current_chat", "Current chat is %chat", {{"%chat", chat->FormatChat(*context->GetValue<ChatMsg>("chat"))}}));
    else
    {
        context->GetValue<ChatMsg>("chat")->Set(parsed);
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_chat_set_to", "Chat set to %chat", {{"%chat", chat->FormatChat(parsed)}}));
    }

    return true;
}
