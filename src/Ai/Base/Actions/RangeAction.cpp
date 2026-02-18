/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "RangeAction.h"

#include "Event.h"
#include "Playerbots.h"

bool RangeAction::Execute(Event event)
{
    std::string const param = event.getParam();
    if (param == "?")
    {
        PrintRange("spell");
        PrintRange("heal");
        PrintRange("shoot");
        PrintRange("flee");
    }

    size_t pos = param.find(" ");
    if (pos == std::string::npos)
        return false;

    std::string const qualifier = param.substr(0, pos);
    std::string const value = param.substr(pos + 1);

    if (value == "?")
    {
        PrintRange(qualifier);
        return true;
    }

    float newVal = (float)atof(value.c_str());
    context->GetValue<float>("range", qualifier)->Set(newVal);

    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_range_set_to", "%qualifier range set to: %value", {{"%qualifier", qualifier}, {"%value", std::to_string(newVal)}}));
    return true;
}

void RangeAction::PrintRange(std::string const type)
{
    float curVal = AI_VALUE2(float, "range", type);

    if (abs(curVal) >= 0.1f)
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_range_is", "%type range: %value", {{"%type", type}, {"%value", std::to_string(curVal)}}));
    else
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_range_is_default", "%type range: %value (default)", {{"%type", type}, {"%value", std::to_string(botAI->GetRange(type))}}));
}
