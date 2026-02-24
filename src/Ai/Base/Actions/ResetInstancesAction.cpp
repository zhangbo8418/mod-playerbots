/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ResetInstancesAction.h"

#include "PlayerbotAI.h"

bool ResetInstancesAction::Execute(Event /*event*/)
{
    // Mirror the core's ResetInstances logic instead of faking a client packet
    if (Group* group = bot->GetGroup())
    {
        if (group->IsLeader(bot->GetGUID()))
            group->ResetInstances(INSTANCE_RESET_ALL, false, bot);
    }
    else
    {
        Player::ResetInstances(bot->GetGUID(), INSTANCE_RESET_ALL, false);
    }

    return true;
}

bool ResetInstancesAction::isUseful() { return botAI->GetGroupLeader() == bot; };
