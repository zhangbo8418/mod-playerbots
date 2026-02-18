/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "LogLevelAction.h"

#include "Event.h"
#include "Playerbots.h"

bool LogLevelAction::Execute(Event event)
{
    std::string const param = event.getParam();
    Value<LogLevel>* value = botAI->GetAiObjectContext()->GetValue<LogLevel>("log level");

    if (param != "?")
    {
        value->Set(string2logLevel(param));
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_log_level_set", "My log level set to %level", {{"%level", logLevel2string(value->Get())}}));
    }
    else
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_log_level_is", "My log level is %level", {{"%level", logLevel2string(value->Get())}}));
    return true;
}

std::string const LogLevelAction::logLevel2string(LogLevel level)
{
    switch (level)
    {
        case LOG_LEVEL_DISABLED:
            return "disabled";
        case LOG_LEVEL_FATAL:
            return "fatal";
        case LOG_LEVEL_ERROR:
            return "error";
        case LOG_LEVEL_WARN:
            return "warn";
        case LOG_LEVEL_INFO:
            return "info";
        case LOG_LEVEL_DEBUG:
            return "debug";
        default:
            return "trace";
    }
}

LogLevel LogLevelAction::string2logLevel(std::string const level)
{
    if (level == "disabled")
        return LOG_LEVEL_DISABLED;
    else if (level == "fatal")
        return LOG_LEVEL_FATAL;
    else if (level == "error")
        return LOG_LEVEL_ERROR;
    else if (level == "warn")
        return LOG_LEVEL_WARN;
    else if (level == "info")
        return LOG_LEVEL_INFO;
    else if (level == "debug")
        return LOG_LEVEL_DEBUG;

    return LOG_LEVEL_TRACE;
}
