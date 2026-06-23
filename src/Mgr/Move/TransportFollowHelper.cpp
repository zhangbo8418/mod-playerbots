/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "TransportFollowHelper.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <unordered_map>
#include <vector>

#include "Group.h"
#include "Map.h"
#include "MotionMaster.h"
#include "MovementActions.h"
#include "ObjectAccessor.h"
#include "PlayerbotAI.h"
#include "PlayerbotAIConfig.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "Playerbots.h"
#include "ServerFacade.h"
#include "Timer.h"
#include "Transport.h"

namespace
{
    constexpr float BOARDING_WALK_DISTANCE = 80.0f;
    constexpr float MIRROR_SYNC_DISTANCE = 100.0f;
    constexpr float BOARDING_FORMATION_SPREAD = 2.5f;
    constexpr float BOARDING_PROGRESS_MIN_DELTA = 1.0f;

    constexpr uint32 BOARDING_MIRROR_TIMEOUT_MS = 6000;
    constexpr uint32 BOARDING_STUCK_TIMEOUT_MS = 3500;
    constexpr uint32 BOARDING_MIRROR_TIMEOUT_MOVING_MS = 2000;
    constexpr uint32 BOARDING_STUCK_TIMEOUT_MOVING_MS = 1000;

    constexpr uint32 BOARDING_MIRROR_TIMEOUT_STATIC_MS = 9000;
    constexpr uint32 BOARDING_STUCK_TIMEOUT_STATIC_MS = 5000;
    constexpr uint32 BOARDING_MIRROR_TIMEOUT_STATIC_PASSENGER_MS = 4500;
    constexpr uint32 BOARDING_STUCK_TIMEOUT_STATIC_PASSENGER_MS = 2800;

    constexpr uint32 BOARDING_DEST_ROTATE_MS = 1800;
    constexpr uint32 BOARDING_DEST_ROTATE_STATIC_MS = 2400;

    constexpr float TRANSPORT_FOLLOW_REISSUE_DIST = 2.0f;
    constexpr float TRANSPORT_FOLLOW_MIN_STEP = 0.35f;
    constexpr float TRANSPORT_FOLLOW_MAX_STEP = 4.0f;

    struct BoardingPoint
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    struct BoardingState
    {
        ObjectGuid leaderGuid;
        ObjectGuid transportGuid;
        uint32 boardingSinceMs = 0;
        uint32 lastProgressMs = 0;
        uint32 lastDestRotateMs = 0;
        float lastDist2d = 0.0f;
        uint8 destIndex = 0;
    };

    std::unordered_map<ObjectGuid::LowType, BoardingState> s_boardingStates;

    BoardingState& GetOrCreateBoardingState(Player* bot, Player* leader, Transport* transport)
    {
        ObjectGuid::LowType const key = bot->GetGUID().GetCounter();
        BoardingState& state = s_boardingStates[key];
        uint32 const now = getMSTime();

        if (state.leaderGuid != leader->GetGUID() || state.transportGuid != transport->GetGUID())
        {
            state.leaderGuid = leader->GetGUID();
            state.transportGuid = transport->GetGUID();
            state.boardingSinceMs = now;
            state.lastProgressMs = now;
            state.lastDestRotateMs = now;
            state.lastDist2d = bot->GetDistance(leader);
            state.destIndex = 0;
        }

        return state;
    }

    void ClearBoardingState(Player* bot)
    {
        if (bot)
            s_boardingStates.erase(bot->GetGUID().GetCounter());
    }

    void NoteBoardingProgress(BoardingState& state, float dist2d)
    {
        uint32 const now = getMSTime();
        if (state.lastDist2d - dist2d >= BOARDING_PROGRESS_MIN_DELTA)
            state.lastProgressMs = now;

        state.lastDist2d = dist2d;
    }

    bool IsMotionTransportMoving(Transport* transport)
    {
        if (!transport || transport->IsStaticTransport() || !transport->IsMotionTransport())
            return false;

        // GO_STATE_ACTIVE while underway; GO_STATE_READY at harbour stops.
        return transport->GetGoState() == GO_STATE_ACTIVE;
    }

    bool IsStaticTransportType(Transport* transport)
    {
        return transport && transport->IsStaticTransport();
    }

    TransportFollowHelper::TransportProbeProfile GetProbeProfile(Transport* transport)
    {
        return IsStaticTransportType(transport) ? TransportFollowHelper::TransportProbeProfile::StaticVertical
                                                : TransportFollowHelper::TransportProbeProfile::Normal;
    }

    Transport* ProbeTransportAt(Map* map, WorldObject* ref, uint32 phaseMask, float x, float y, float z,
        TransportFollowHelper::TransportProbeProfile profile)
    {
        if (!map || !ref)
            return nullptr;

        if (profile == TransportFollowHelper::TransportProbeProfile::StaticVertical)
        {
            std::array<float, 14> const probes = {
                z, z + 0.5f, z - 0.5f, z + 1.0f, z - 1.0f, z + 1.5f, z - 1.5f,
                z + 3.0f, z - 3.0f, z + 6.0f, z - 6.0f, z + 10.0f, z - 10.0f, z + 15.0f
            };
            for (float const pz : probes)
            {
                if (Transport* t = map->GetTransportForPos(phaseMask, x, y, pz, ref))
                    return t;
            }

            return nullptr;
        }

        std::array<float, 8> const probes = {
            z, z + 0.5f, z + 1.5f, z - 0.5f, z + 3.0f, z - 3.0f, z + 6.0f, z - 6.0f
        };
        for (float const pz : probes)
        {
            if (Transport* t = map->GetTransportForPos(phaseMask, x, y, pz, ref))
                return t;
        }

        return nullptr;
    }

    bool IsLeaderOnTransportPassenger(Player* leader, Transport* transport)
    {
        return leader && transport &&
            (leader->GetTransport() == transport || leader->HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT));
    }

    bool IsLeaderOnTransportSurface(Map* map, Player* leader, Transport* transport)
    {
        if (!map || !leader || !transport)
            return false;

        if (IsLeaderOnTransportPassenger(leader, transport))
            return true;

        TransportFollowHelper::TransportProbeProfile const profile = GetProbeProfile(transport);
        float const probeZ = IsStaticTransportType(transport) ? leader->GetPositionZ()
            : std::max(leader->GetPositionZ(), transport->GetPositionZ());
        return ProbeTransportAt(map, leader, leader->GetPhaseMask(), leader->GetPositionX(),
            leader->GetPositionY(), probeZ, profile) == transport;
    }

    void GetMirrorTimeouts(Transport* transport, bool leaderPassenger, uint32& mirrorTimeoutMs, uint32& stuckTimeoutMs)
    {
        if (IsStaticTransportType(transport))
        {
            if (leaderPassenger)
            {
                mirrorTimeoutMs = BOARDING_MIRROR_TIMEOUT_STATIC_PASSENGER_MS;
                stuckTimeoutMs = BOARDING_STUCK_TIMEOUT_STATIC_PASSENGER_MS;
            }
            else
            {
                mirrorTimeoutMs = BOARDING_MIRROR_TIMEOUT_STATIC_MS;
                stuckTimeoutMs = BOARDING_STUCK_TIMEOUT_STATIC_MS;
            }

            return;
        }

        if (IsMotionTransportMoving(transport))
        {
            mirrorTimeoutMs = BOARDING_MIRROR_TIMEOUT_MOVING_MS;
            stuckTimeoutMs = BOARDING_STUCK_TIMEOUT_MOVING_MS;
            return;
        }

        mirrorTimeoutMs = BOARDING_MIRROR_TIMEOUT_MS;
        stuckTimeoutMs = BOARDING_STUCK_TIMEOUT_MS;
    }

    bool ShouldUseMirrorFallback(BoardingState const& state, float dist2d, bool leaderEngaged,
        Transport* transport, bool leaderPassenger)
    {
        if (!leaderEngaged)
            return false;

        if (dist2d > MIRROR_SYNC_DISTANCE)
            return false;

        uint32 mirrorTimeout = BOARDING_MIRROR_TIMEOUT_MS;
        uint32 stuckTimeout = BOARDING_STUCK_TIMEOUT_MS;
        GetMirrorTimeouts(transport, leaderPassenger, mirrorTimeout, stuckTimeout);

        uint32 const now = getMSTime();
        if (now - state.boardingSinceMs >= mirrorTimeout)
            return true;

        if (now - state.lastProgressMs >= stuckTimeout)
            return true;

        return false;
    }

    bool FindBoardingEdgePoint(Map* map, Transport* expectedTransport, WorldObject* ref,
        float masterX, float masterY, float masterZ,
        float botX, float botY, float botZ,
        float& outX, float& outY, float& outZ)
    {
        if (!map || !expectedTransport || !ref || IsStaticTransportType(expectedTransport))
            return false;

        uint32 const phaseMask = ref->GetPhaseMask();
        float const probeZ = std::max({ masterZ, botZ, expectedTransport->GetPositionZ() });
        TransportFollowHelper::TransportProbeProfile const profile = GetProbeProfile(expectedTransport);

        if (ProbeTransportAt(map, ref, phaseMask, masterX, masterY, masterZ, profile) != expectedTransport)
            return false;

        float const dx2 = botX - masterX;
        float const dy2 = botY - masterY;
        float const dist2d = std::sqrt(dx2 * dx2 + dy2 * dy2);
        int32 const steps = std::clamp(static_cast<int32>(dist2d / 0.4f), 16, 48);

        float const dx = (botX - masterX) / static_cast<float>(steps);
        float const dy = (botY - masterY) / static_cast<float>(steps);

        float lastX = masterX;
        float lastY = masterY;
        bool found = false;

        for (int32 i = 1; i <= steps; ++i)
        {
            float const px = masterX + dx * static_cast<float>(i);
            float const py = masterY + dy * static_cast<float>(i);

            Transport* const t = ProbeTransportAt(map, ref, phaseMask, px, py, probeZ, profile);
            if (t != expectedTransport)
                break;

            lastX = px;
            lastY = py;
            found = true;
        }

        if (!found)
            return false;

        outX = lastX;
        outY = lastY;
        outZ = masterZ;
        return true;
    }

    bool FindApproachPointFromBot(Map* map, Transport* expectedTransport, WorldObject* ref,
        float botX, float botY, float botZ,
        float targetX, float targetY, float targetZ,
        float& outX, float& outY, float& outZ)
    {
        if (!map || !expectedTransport || !ref)
            return false;

        uint32 const phaseMask = ref->GetPhaseMask();
        bool const isStatic = IsStaticTransportType(expectedTransport);
        TransportFollowHelper::TransportProbeProfile const profile = GetProbeProfile(expectedTransport);
        float const probeZ = isStatic ? targetZ : std::max({ botZ, targetZ, expectedTransport->GetPositionZ() });

        float const dx2 = targetX - botX;
        float const dy2 = targetY - botY;
        float const dist2d = std::sqrt(dx2 * dx2 + dy2 * dy2);
        if (dist2d < 0.5f)
            return false;

        int32 const steps = std::clamp(static_cast<int32>(dist2d / 0.4f), 16, 64);
        float const dx = dx2 / static_cast<float>(steps);
        float const dy = dy2 / static_cast<float>(steps);

        float bestX = 0.0f;
        float bestY = 0.0f;
        float bestZ = targetZ;
        bool found = false;

        for (int32 i = 1; i <= steps; ++i)
        {
            float const px = botX + dx * static_cast<float>(i);
            float const py = botY + dy * static_cast<float>(i);

            Transport* const t = ProbeTransportAt(map, ref, phaseMask, px, py, probeZ, profile);
            if (t == expectedTransport)
            {
                bestX = px;
                bestY = py;
                bestZ = targetZ;
                found = true;
                break;
            }
        }

        if (!found)
            return false;

        outX = bestX;
        outY = bestY;
        outZ = bestZ;
        return true;
    }

    void SanitizeDestinationZ(Player* bot, Map* map, float x, float y, float& z)
    {
        if (!bot || !map)
            return;

        float const groundZ = map->GetHeight(bot->GetPhaseMask(), x, y, z + 2.0f, true, 25.0f);
        if (groundZ > -200000.0f)
            z = std::max(z, groundZ + 0.2f);
    }

    void CollectBoardingDestinations(Map* map, Transport* transport, Player* leader, Player* bot,
        bool leaderOnSurface, bool leaderPassenger, std::vector<BoardingPoint>& destinations)
    {
        destinations.clear();
        if (!map || !transport || !leader || !bot)
            return;

        bool const isStatic = IsStaticTransportType(transport);
        TransportFollowHelper::TransportProbeProfile const profile = GetProbeProfile(transport);

        auto addPoint = [&](float x, float y, float z)
        {
            SanitizeDestinationZ(bot, map, x, y, z);

            for (BoardingPoint const& existing : destinations)
            {
                if (existing.x == x && existing.y == y && std::fabs(existing.z - z) < 0.25f)
                    return;
            }

            destinations.push_back({ x, y, z });
        };

        float const leaderX = leader->GetPositionX();
        float const leaderY = leader->GetPositionY();
        float const leaderZ = leader->GetPositionZ();

        if (isStatic)
        {
            if (leaderOnSurface)
            {
                addPoint(leaderX, leaderY, leaderZ);

                std::array<float, 6> const zOffsets = { 0.5f, -0.5f, 1.5f, -1.5f, 3.0f, -3.0f };
                for (float const offset : zOffsets)
                {
                    if (ProbeTransportAt(map, leader, leader->GetPhaseMask(), leaderX, leaderY, leaderZ + offset,
                            profile) == transport)
                        addPoint(leaderX, leaderY, leaderZ);
                }
            }

            float approachX = 0.0f;
            float approachY = 0.0f;
            float approachZ = 0.0f;
            if (FindApproachPointFromBot(map, transport, bot, bot->GetPositionX(), bot->GetPositionY(),
                    bot->GetPositionZ(), leaderX, leaderY, leaderZ, approachX, approachY, approachZ))
                addPoint(approachX, approachY, approachZ);

            if (leaderOnSurface)
            {
                float const midX = (bot->GetPositionX() + leaderX) * 0.5f;
                float const midY = (bot->GetPositionY() + leaderY) * 0.5f;
                if (ProbeTransportAt(map, bot, bot->GetPhaseMask(), midX, midY, leaderZ, profile) == transport)
                    addPoint(midX, midY, leaderZ);
            }

            if (destinations.empty())
                addPoint(leaderX, leaderY, leaderZ);

            std::sort(destinations.begin(), destinations.end(),
                [bot](BoardingPoint const& a, BoardingPoint const& b)
                {
                    return bot->GetExactDist(a.x, a.y, a.z) < bot->GetExactDist(b.x, b.y, b.z);
                });
            return;
        }

        // MotionTransport (ship / zeppelin): gangplank edge first, then approach paths.
        BoardingPoint edgePoint;
        bool hasEdgePoint = false;
        if (leaderPassenger &&
            FindBoardingEdgePoint(map, transport, leader, leaderX, leaderY, leaderZ, bot->GetPositionX(),
                bot->GetPositionY(), bot->GetPositionZ(), edgePoint.x, edgePoint.y, edgePoint.z))
            hasEdgePoint = true;

        if (hasEdgePoint)
            addPoint(edgePoint.x, edgePoint.y, edgePoint.z);

        if (leaderOnSurface)
            addPoint(leaderX, leaderY, leaderZ);

        float approachX = 0.0f;
        float approachY = 0.0f;
        float approachZ = 0.0f;
        float const targetX = leaderOnSurface ? leaderX : transport->GetPositionX();
        float const targetY = leaderOnSurface ? leaderY : transport->GetPositionY();
        float const targetZ = leaderOnSurface ? leaderZ : transport->GetPositionZ();
        if (FindApproachPointFromBot(map, transport, bot, bot->GetPositionX(), bot->GetPositionY(), bot->GetPositionZ(),
                targetX, targetY, targetZ, approachX, approachY, approachZ))
            addPoint(approachX, approachY, approachZ);

        addPoint(transport->GetPositionX(), transport->GetPositionY(), transport->GetPositionZ());

        float const midX = (bot->GetPositionX() + targetX) * 0.5f;
        float const midY = (bot->GetPositionY() + targetY) * 0.5f;
        float const midZ = (bot->GetPositionZ() + targetZ) * 0.5f;
        if (ProbeTransportAt(map, bot, bot->GetPhaseMask(), midX, midY, midZ, profile) == transport)
            addPoint(midX, midY, targetZ);

        if (destinations.empty())
            addPoint(targetX, targetY, targetZ);

        std::sort(destinations.begin(), destinations.end(),
            [bot](BoardingPoint const& a, BoardingPoint const& b)
            {
                return bot->GetExactDist(a.x, a.y, a.z) < bot->GetExactDist(b.x, b.y, b.z);
            });

        if (hasEdgePoint)
        {
            destinations.erase(
                std::remove_if(destinations.begin(), destinations.end(),
                    [&edgePoint](BoardingPoint const& point)
                    {
                        return std::fabs(point.x - edgePoint.x) < 0.1f && std::fabs(point.y - edgePoint.y) < 0.1f;
                    }),
                destinations.end());
            destinations.insert(destinations.begin(), edgePoint);
        }
    }

    BoardingPoint PickBoardingDestination(BoardingState& state, std::vector<BoardingPoint> const& destinations,
        bool isStatic)
    {
        if (destinations.empty())
            return {};

        uint32 const rotateMs = isStatic ? BOARDING_DEST_ROTATE_STATIC_MS : BOARDING_DEST_ROTATE_MS;
        uint32 const now = getMSTime();
        if (now - state.lastDestRotateMs >= rotateMs && now - state.lastProgressMs >= rotateMs)
        {
            state.destIndex = static_cast<uint8>((state.destIndex + 1) % destinations.size());
            state.lastDestRotateMs = now;
        }

        return destinations[state.destIndex % destinations.size()];
    }

    void BoardOnTransport(Player* bot, Transport* transport, Player* leader)
    {
        if (!bot || !transport || !leader)
            return;

        if (bot->GetTransport() && bot->GetTransport() != transport)
            bot->GetTransport()->RemovePassenger(bot, true);

        if (bot->GetTransport() == transport)
            return;

        TransportFollowHelper::DismountForTransport(bot);

        uint32 const slot = bot->GetGUID().GetCounter() % 8;
        float const angle = static_cast<float>(slot) * (2.0f * static_cast<float>(M_PI) / 8.0f);
        float const ox = std::cos(angle) * BOARDING_FORMATION_SPREAD;
        float const oy = std::sin(angle) * BOARDING_FORMATION_SPREAD;

        bot->NearTeleportTo(leader->GetPositionX() + ox, leader->GetPositionY() + oy, leader->GetPositionZ(),
            leader->GetOrientation());

        transport->AddPassenger(bot, true);
        bot->StopMovingOnCurrentPos();

        if (bot->GetMotionMaster())
            bot->GetMotionMaster()->Clear();

        ClearBoardingState(bot);
    }

    void PrepareBotForBoardingWalk(Player* bot, PlayerbotAI* botAI)
    {
        if (!bot)
            return;

        TransportFollowHelper::DismountForTransport(bot);

        if (bot->IsSitState())
            bot->SetStandState(UNIT_STAND_STATE_STAND);

        if (bot->IsNonMeleeSpellCast(true))
        {
            bot->CastStop();
            if (botAI)
                botAI->InterruptSpell();
        }
    }

    bool WalkToBoardingPoint(Player* bot, PlayerbotAI* botAI, MovementAction* moveAction, uint32 mapId,
        float destX, float destY, float destZ, MovementPriority priority)
    {
        if (!bot || !moveAction)
            return false;

        // Ground pathing while already a passenger can walk off deck / platform edges.
        if (bot->GetTransport())
            return false;

        PrepareBotForBoardingWalk(bot, botAI);

        if (moveAction->MoveToForTransportFollow(mapId, destX, destY, destZ, priority))
            return true;

        float const dist = bot->GetExactDist(destX, destY, destZ);
        if (dist <= sPlayerbotAIConfig.followDistance)
            return false;

        if (MotionMaster* mm = bot->GetMotionMaster())
        {
            mm->MovePoint(0, destX, destY, destZ, FORCED_MOVEMENT_NONE, 0.0f, 0.0f, false, false);
            float delay = 1000.0f * dist / std::max(bot->GetSpeed(MOVE_RUN), 1.0f);
            delay = std::clamp(delay, 0.0f, static_cast<float>(sPlayerbotAIConfig.maxWaitForMove));
            moveAction->SetTransportFollowDelay(delay);
            return true;
        }

        return false;
    }

    enum class BoardingAttemptResult
    {
        NotApplicable,
        OnTransport,
        WalkIssued,
        Mirrored
    };

    BoardingAttemptResult AttemptLeaderTransportBoarding(Player* bot, Player* leader, PlayerbotAI* botAI,
        MovementAction* moveAction, bool allowWalk)
    {
        if (!bot || !leader || !bot->IsInWorld() || !leader->IsInWorld() || bot->GetMapId() != leader->GetMapId())
            return BoardingAttemptResult::NotApplicable;

        Map* map = leader->GetMap();
        if (!map)
            return BoardingAttemptResult::NotApplicable;

        Transport* leaderTransport = TransportFollowHelper::GetLeaderTransport(leader, map);
        if (!leaderTransport)
        {
            ClearBoardingState(bot);
            return BoardingAttemptResult::NotApplicable;
        }

        bool const leaderPassenger = IsLeaderOnTransportPassenger(leader, leaderTransport);
        bool const leaderOnSurface = IsLeaderOnTransportSurface(map, leader, leaderTransport);
        bool const leaderEngaged = leaderPassenger || leaderOnSurface;
        bool const transportAtDock = !IsMotionTransportMoving(leaderTransport);

        if (bot->GetTransport() == leaderTransport)
        {
            bot->StopMovingOnCurrentPos();
            if (bot->GetMotionMaster())
                bot->GetMotionMaster()->Clear();

            TransportFollowHelper::AdjustFollowOnSharedTransport(bot, leader, sPlayerbotAIConfig.followDistance);
            ClearBoardingState(bot);
            return BoardingAttemptResult::OnTransport;
        }

        float const botProbeZ = IsStaticTransportType(leaderTransport) ? leader->GetPositionZ()
            : std::max(bot->GetPositionZ(), leaderTransport->GetPositionZ());
        TransportFollowHelper::TransportProbeProfile const profile = GetProbeProfile(leaderTransport);
        Transport* botSurfaceTransport = ProbeTransportAt(
            map, bot, bot->GetPhaseMask(), bot->GetPositionX(), bot->GetPositionY(), botProbeZ, profile);

        if (botSurfaceTransport == leaderTransport)
        {
            PrepareBotForBoardingWalk(bot, botAI);
            leaderTransport->AddPassenger(bot, true);
            bot->StopMovingOnCurrentPos();
            ClearBoardingState(bot);
            return BoardingAttemptResult::OnTransport;
        }

        if (!leaderEngaged && !(transportAtDock && leader->GetDistance(leaderTransport) <= BOARDING_WALK_DISTANCE))
            return BoardingAttemptResult::NotApplicable;

        float const dist2d = ServerFacade::instance().GetDistance2d(bot, leader);
        BoardingState& state = GetOrCreateBoardingState(bot, leader, leaderTransport);
        NoteBoardingProgress(state, dist2d);

        if (ShouldUseMirrorFallback(state, dist2d, leaderPassenger || leaderOnSurface, leaderTransport, leaderPassenger))
        {
            BoardOnTransport(bot, leaderTransport, leader);
            return BoardingAttemptResult::Mirrored;
        }

        if (!allowWalk || !ServerFacade::instance().IsDistanceLessOrEqualThan(dist2d, BOARDING_WALK_DISTANCE))
            return BoardingAttemptResult::NotApplicable;

        bool const isStatic = IsStaticTransportType(leaderTransport);
        std::vector<BoardingPoint> destinations;
        CollectBoardingDestinations(map, leaderTransport, leader, bot, leaderOnSurface, leaderPassenger, destinations);
        BoardingPoint const dest = PickBoardingDestination(state, destinations, isStatic);

        MovementPriority const priority = botAI && botAI->GetState() == BOT_STATE_COMBAT
            ? MovementPriority::MOVEMENT_COMBAT
            : MovementPriority::MOVEMENT_NORMAL;

        if (WalkToBoardingPoint(bot, botAI, moveAction, bot->GetMapId(), dest.x, dest.y, dest.z, priority))
            return BoardingAttemptResult::WalkIssued;

        return BoardingAttemptResult::NotApplicable;
    }

    bool CanSafelyLeaveTransport(Transport* transport)
    {
        if (!transport)
            return true;

        if (transport->IsMotionTransport())
            return !IsMotionTransportMoving(transport);

        // StaticTransport (elevator / subway): only disembark while paused at a stop.
        return transport->GetGoState() == GO_STATE_READY;
    }

    void UpdatePassiveTransportMembership(Player* bot)
    {
        if (!bot || !bot->IsInWorld() || !bot->GetMap())
            return;

        Map* map = bot->GetMap();
        float const probeZ = bot->GetPositionZ();
        uint32 const phaseMask = bot->GetPhaseMask();
        float const x = bot->GetPositionX();
        float const y = bot->GetPositionY();
        Transport* newTransport = ProbeTransportAt(map, bot, phaseMask, x, y, probeZ,
            TransportFollowHelper::TransportProbeProfile::Normal);
        if (!newTransport)
            newTransport = ProbeTransportAt(map, bot, phaseMask, x, y, probeZ,
                TransportFollowHelper::TransportProbeProfile::StaticVertical);

        if (newTransport == bot->GetTransport())
            return;

        if (bot->GetTransport())
        {
            // Z probes can miss on moving decks / elevator cars — never eject on a null reading.
            if (!newTransport)
                return;

            bot->GetTransport()->RemovePassenger(bot, true);
        }

        if (newTransport)
            newTransport->AddPassenger(bot, true);

        bot->StopMovingOnCurrentPos();
    }

    void TryLeaveTransportWithLeader(Player* bot, Player* leader, Transport* leaderTransport)
    {
        if (!bot || !leader || leaderTransport || !bot->GetTransport())
            return;

        if (!CanSafelyLeaveTransport(bot->GetTransport()))
            return;

        if (bot->GetDistance(leader) > MIRROR_SYNC_DISTANCE)
            return;

        bot->GetTransport()->RemovePassenger(bot, true);
        bot->StopMovingOnCurrentPos();
        ClearBoardingState(bot);
    }

    bool GetUnitTransportLocalPos(Unit* unit, Transport* transport, float& x, float& y, float& z, float& o)
    {
        if (!unit || !transport)
            return false;

        if (unit->HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT) || unit->GetTransport() == transport)
        {
            unit->m_movementInfo.transport.pos.GetPosition(x, y, z, o);
            return true;
        }

        x = unit->GetPositionX();
        y = unit->GetPositionY();
        z = unit->GetPositionZ();
        o = unit->GetOrientation();
        transport->CalculatePassengerOffset(x, y, z, &o);
        return true;
    }

    bool IsLocalPointOnTransport(Map* map, Player* bot, Transport* transport, float localX, float localY, float localZ)
    {
        if (!map || !bot || !transport)
            return false;

        float wx = localX;
        float wy = localY;
        float wz = localZ;
        transport->CalculatePassengerPosition(wx, wy, wz);
        return ProbeTransportAt(map, bot, bot->GetPhaseMask(), wx, wy, wz, GetProbeProfile(transport)) == transport;
    }

    bool ClampTransportFollowDestination(Map* map, Player* bot, Transport* transport, float fromX, float fromY,
        float fromZ, float& destX, float& destY, float& destZ)
    {
        if (!map || !bot || !transport)
            return false;

        if (IsLocalPointOnTransport(map, bot, transport, destX, destY, destZ))
            return true;

        float const dx = destX - fromX;
        float const dy = destY - fromY;
        float const dz = destZ - fromZ;
        float const dist2d = std::sqrt(dx * dx + dy * dy);
        if (dist2d < 0.01f)
            return false;

        int32 const steps = std::clamp(static_cast<int32>(dist2d / 0.35f), 4, 28);
        float lastX = fromX;
        float lastY = fromY;
        float lastZ = fromZ;
        bool found = false;

        for (int32 i = 1; i <= steps; ++i)
        {
            float const t = static_cast<float>(i) / static_cast<float>(steps);
            float const px = fromX + dx * t;
            float const py = fromY + dy * t;
            float const pz = fromZ + dz * t;

            if (!IsLocalPointOnTransport(map, bot, transport, px, py, pz))
                break;

            lastX = px;
            lastY = py;
            lastZ = pz;
            found = true;
        }

        if (!found)
            return false;

        destX = lastX;
        destY = lastY;
        destZ = lastZ;
        return true;
    }

    void LimitTransportFollowStep(float fromX, float fromY, float fromZ, float& destX, float& destY, float& destZ,
        float maxStep)
    {
        float const dx = destX - fromX;
        float const dy = destY - fromY;
        float const dz = destZ - fromZ;
        float const dist = std::sqrt(dx * dx + dy * dy + dz * dz);
        if (dist <= maxStep || dist < 0.01f)
            return;

        float const scale = maxStep / dist;
        destX = fromX + dx * scale;
        destY = fromY + dy * scale;
        destZ = fromZ + dz * scale;
    }

    bool ShouldReissueTransportMove(Player* bot, float localX, float localY, float localZ)
    {
        if (!bot || !bot->movespline || bot->movespline->Finalized())
            return true;

        if (!bot->HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT))
            return true;

        G3D::Vector3 const final = bot->movespline->FinalDestination();
        float const dx = final.x - localX;
        float const dy = final.y - localY;
        float const dz = final.z - localZ;
        return (dx * dx + dy * dy + dz * dz) > TRANSPORT_FOLLOW_REISSUE_DIST * TRANSPORT_FOLLOW_REISSUE_DIST;
    }

    void IssueTransportLocalMove(Player* bot, Transport* transport, Map* map, float localX, float localY, float localZ,
        float worldOrientation)
    {
        if (!bot || !transport || !map)
            return;

        bot->m_movementInfo.flags |= MOVEMENTFLAG_ONTRANSPORT;
        bot->m_movementInfo.transport.guid = transport->GetGUID();

        float bx = 0.0f;
        float by = 0.0f;
        float bz = 0.0f;
        float bo = 0.0f;
        bot->m_movementInfo.transport.pos.GetPosition(bx, by, bz, bo);

        float destX = localX;
        float destY = localY;
        float destZ = localZ;
        LimitTransportFollowStep(bx, by, bz, destX, destY, destZ, TRANSPORT_FOLLOW_MAX_STEP);
        if (!ClampTransportFollowDestination(map, bot, transport, bx, by, bz, destX, destY, destZ))
            return;

        if (!ShouldReissueTransportMove(bot, destX, destY, destZ))
            return;

        float const stepDist = std::sqrt((destX - bx) * (destX - bx) + (destY - by) * (destY - by));
        if (stepDist < TRANSPORT_FOLLOW_MIN_STEP)
            return;

        if (!bot->IsStopped())
            bot->StopMoving();

        if (G3D::fuzzyEq(bx, destX) && G3D::fuzzyEq(by, destY))
        {
            destX += 0.2f * std::cos(bo);
            destY += 0.2f * std::sin(bo);
        }

        Movement::MoveSplineInit init(bot);
        init.DisableTransportPathTransformations();
        init.MoveTo(destX, destY, destZ, false);
        init.SetFacing(worldOrientation);
        init.Launch();
    }
}

Transport* TransportFollowHelper::GetTransportForPosTolerant(Map* map, WorldObject* ref, uint32 phaseMask, float x,
    float y, float z, TransportFollowHelper::TransportProbeProfile profile)
{
    return ProbeTransportAt(map, ref, phaseMask, x, y, z, profile);
}

Transport* TransportFollowHelper::GetLeaderTransport(Player* leader, Map* map)
{
    if (!leader || !map)
        return nullptr;

    if (Transport* transport = leader->GetTransport())
        return transport;

    float const leaderZ = leader->GetPositionZ();
    uint32 const phaseMask = leader->GetPhaseMask();
    float const x = leader->GetPositionX();
    float const y = leader->GetPositionY();

    if (Transport* transport = ProbeTransportAt(map, leader, phaseMask, x, y, leaderZ,
            TransportProbeProfile::Normal))
        return transport;

    return ProbeTransportAt(map, leader, phaseMask, x, y, leaderZ, TransportProbeProfile::StaticVertical);
}

Player* TransportFollowHelper::GetFollowLeader(Player* bot, PlayerbotAI* botAI)
{
    if (!bot || !botAI)
        return nullptr;

    if (Player* master = botAI->GetMaster())
        return master;

    if (Group* group = bot->GetGroup())
        if (Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID()))
            return leader;

    return nullptr;
}

bool TransportFollowHelper::ShouldSuppressMount(Player* bot, Player* leader)
{
    if (!bot || !leader || !leader->IsInWorld() || bot->GetMapId() != leader->GetMapId())
        return false;

    if (leader->GetTransport() || leader->HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT))
        return true;

    Map* map = leader->GetMap();
    if (!map)
        return false;

    if (Transport* transport = GetLeaderTransport(leader, map))
    {
        float const dist = bot->GetDistance(leader);
        if (dist <= BOARDING_WALK_DISTANCE)
        {
            TransportProbeProfile const profile = transport->IsStaticTransport() ? TransportProbeProfile::StaticVertical
                                                                                 : TransportProbeProfile::Normal;
            float const probeZ = transport->IsStaticTransport() ? leader->GetPositionZ()
                : std::max(leader->GetPositionZ(), transport->GetPositionZ());
            if (TransportFollowHelper::GetTransportForPosTolerant(map, leader, leader->GetPhaseMask(), leader->GetPositionX(),
                    leader->GetPositionY(), probeZ, profile) == transport)
                return true;

            if (!IsMotionTransportMoving(transport) && leader->GetDistance(transport) <= BOARDING_WALK_DISTANCE)
                return true;
        }
    }

    return false;
}

void TransportFollowHelper::DismountForTransport(Player* bot)
{
    if (!bot)
        return;

    if (bot->IsMounted())
    {
        WorldPacket emptyPacket;
        bot->GetSession()->HandleCancelMountAuraOpcode(emptyPacket);
    }

    if (PlayerbotAI* botAI = GET_PLAYERBOT_AI(bot))
        botAI->RemoveShapeshift();
}

void TransportFollowHelper::TickTransport(Player* bot, PlayerbotAI* botAI)
{
    if (!bot || !botAI || !bot->IsInWorld())
        return;

    Player* leader = GetFollowLeader(bot, botAI);
    if (!leader || !leader->IsInWorld() || bot->GetMapId() != leader->GetMapId())
    {
        UpdatePassiveTransportMembership(bot);
        return;
    }

    Map* map = bot->GetMap();
    Transport* leaderTransport = GetLeaderTransport(leader, map);

    if (leaderTransport)
    {
        AttemptLeaderTransportBoarding(bot, leader, botAI, nullptr, false);
        return;
    }

    TryLeaveTransportWithLeader(bot, leader, leaderTransport);
    UpdatePassiveTransportMembership(bot);
}

TransportFollowHelper::FollowResult TransportFollowHelper::HandleFollowTransport(Player* bot, Player* leader,
    PlayerbotAI* botAI, MovementAction* moveAction)
{
    if (!bot || !leader || !botAI || !moveAction)
        return FollowResult::NotHandled;

    Map* map = leader->GetMap();
    Transport* leaderTransport = GetLeaderTransport(leader, map);
    if (!leaderTransport)
    {
        TryLeaveTransportWithLeader(bot, leader, leaderTransport);
        return FollowResult::NotHandled;
    }

    switch (AttemptLeaderTransportBoarding(bot, leader, botAI, moveAction, true))
    {
        case BoardingAttemptResult::OnTransport:
        case BoardingAttemptResult::WalkIssued:
        case BoardingAttemptResult::Mirrored:
            return FollowResult::Handled;
        default:
            break;
    }

    return FollowResult::NotHandled;
}

bool TransportFollowHelper::AdjustFollowOnSharedTransport(Player* bot, Unit* target, float distance)
{
    if (!bot || !target || !bot->GetTransport() || bot->GetTransport() != target->GetTransport())
        return false;

    Transport* transport = bot->GetTransport();

    float tx = 0.0f;
    float ty = 0.0f;
    float tz = 0.0f;
    float to = 0.0f;
    if (!GetUnitTransportLocalPos(target, transport, tx, ty, tz, to))
        return false;

    float bx = 0.0f;
    float by = 0.0f;
    float bz = 0.0f;
    float bo = 0.0f;
    if (!GetUnitTransportLocalPos(bot, transport, bx, by, bz, bo))
        return false;

    float const dx = tx - bx;
    float const dy = ty - by;
    float const dist2d = std::sqrt(dx * dx + dy * dy);

    if (dist2d <= distance)
    {
        bot->StopMovingOnCurrentPos();
        if (bot->GetMotionMaster())
            bot->GetMotionMaster()->Clear();
        return true;
    }

    float destLocalX = tx;
    float destLocalY = ty;
    float destLocalZ = tz;
    if (dist2d > 0.01f)
    {
        float const scale = distance / dist2d;
        destLocalX = tx - dx * scale;
        destLocalY = ty - dy * scale;
        destLocalZ = tz;
    }

    Map* map = bot->GetMap();
    if (!map)
        return false;

    LimitTransportFollowStep(bx, by, bz, destLocalX, destLocalY, destLocalZ, TRANSPORT_FOLLOW_MAX_STEP);
    if (!ClampTransportFollowDestination(map, bot, transport, bx, by, bz, destLocalX, destLocalY, destLocalZ))
    {
        bot->StopMovingOnCurrentPos();
        if (bot->GetMotionMaster())
            bot->GetMotionMaster()->Clear();
        return true;
    }

    float const stepDist = std::sqrt((destLocalX - bx) * (destLocalX - bx) + (destLocalY - by) * (destLocalY - by));
    if (stepDist < TRANSPORT_FOLLOW_MIN_STEP)
        return true;

    IssueTransportLocalMove(bot, transport, map, destLocalX, destLocalY, destLocalZ, target->GetOrientation());
    return true;
}
