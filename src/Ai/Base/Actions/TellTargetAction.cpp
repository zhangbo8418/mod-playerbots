/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "TellTargetAction.h"

#include "Event.h"
#include "ThreatMgr.h"
#include "AiObjectContext.h"
#include "PlayerbotAI.h"

bool TellTargetAction::Execute(Event /*event*/)
{
    Unit* target = context->GetValue<Unit*>("current target")->Get();
    if (target)
    {
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_attacking_target", "Attacking %target", {{"%target", target->GetName()}}));
        context->GetValue<Unit*>("old target")->Set(target);
    }

    return true;
}

bool TellAttackersAction::Execute(Event /*event*/)
{
    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_attackers_title", "--- Attackers ---"));

    GuidVector attackers = context->GetValue<GuidVector>("attackers")->Get();
    int32 count = 0;
    for (ObjectGuid const guid : attackers)
    {
        Unit* unit = botAI->GetUnit(guid);
        if (!unit || !unit->IsAlive())
            continue;

        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_attacker_line", "%num.%name", {{"%num", std::to_string(++count)}, {"%name", unit->GetName()}}));
    }

    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_threat_title", "--- Threat ---"));

    HostileReference* ref = bot->getHostileRefMgr().getFirst();
    if (!ref)
        return true;

    while (ref)
    {
        ThreatMgr* threatMgr = ref->GetSource();
        Unit* unit = threatMgr->GetOwner();
        float threat = ref->GetThreat();

        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_threat_entry", "%name (%threat)", {{"%name", unit->GetName()}, {"%threat", std::to_string(threat)}}));

        ref = ref->next();
    }

    return true;
}
