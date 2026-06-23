/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_TRANSPORT_FOLLOW_HELPER_H
#define _PLAYERBOT_TRANSPORT_FOLLOW_HELPER_H

#include "Define.h"

class Map;
class MovementAction;
class Player;
class PlayerbotAI;
class Transport;
class Unit;
class WorldObject;

class TransportFollowHelper
{
public:
    enum class FollowResult
    {
        NotHandled,
        Handled
    };

    enum class TransportProbeProfile
    {
        Normal,
        StaticVertical
    };

    static Transport* GetTransportForPosTolerant(Map* map, WorldObject* ref, uint32 phaseMask, float x, float y,
        float z, TransportProbeProfile profile = TransportProbeProfile::Normal);
    static Transport* GetLeaderTransport(Player* leader, Map* map);
    static Player* GetFollowLeader(Player* bot, PlayerbotAI* botAI);

    static bool ShouldSuppressMount(Player* bot, Player* leader);
    static void DismountForTransport(Player* bot);

    // Drop cached boarding state when the bot logs out.
    static void ClearBoardingState(Player* bot);

    // Respect activity rotation when off transport; always tick while a passenger.
    static bool ShouldRunPeriodicTransportTick(Player* bot, bool activityAllowed);

    // Periodic tick from PlayerbotAI (mirror fallback when walk stalls).
    static void TickTransport(Player* bot, PlayerbotAI* botAI);

    // FollowAction entry: walk-up boarding first, mirror fallback if stuck.
    static FollowResult HandleFollowTransport(Player* bot, Player* leader, PlayerbotAI* botAI, MovementAction* moveAction);

    // MovementAction::Follow — local-coordinate follow on a shared transport.
    static bool AdjustFollowOnSharedTransport(Player* bot, Unit* target, float distance);
};

#endif
