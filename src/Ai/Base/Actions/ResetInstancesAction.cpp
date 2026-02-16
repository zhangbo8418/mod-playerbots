/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ResetInstancesAction.h"

#include "Playerbots.h"
#include "Group.h"
#include "Player.h"
#include "SharedDefines.h"

bool ResetInstancesAction::Execute(Event /*event*/)
{
    // Mirror WorldSession::HandleResetInstancesOpcode logic without relying on packet type
    if (Group* group = bot->GetGroup())
    {
        if (group->IsLeader(bot->GetGUID()))
            group->ResetInstances(INSTANCE_RESET_ALL, false, bot);
    }
    else
    {
        Player::ResetInstances(bot->GetGUID(), INSTANCE_RESET_ALL, false);
    }

    botAI->TellMaster(botAI->BotTextForMaster("tell_reset_instances", "Resetting all instances"));
    return true;
}

bool ResetInstancesAction::isUseful() { return botAI->GetGroupLeader() == bot; };
