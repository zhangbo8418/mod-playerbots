/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "PlayerbotSecurity.h"

#include "LFGMgr.h"
#include "PlayerbotAIConfig.h"
#include "Playerbots.h"

PlayerbotSecurity::PlayerbotSecurity(Player* const bot) : bot(bot)
{
    if (bot)
        account = sCharacterCache->GetCharacterAccountIdByGuid(bot->GetGUID());
}

PlayerbotSecurityLevel PlayerbotSecurity::LevelFor(Player* from, DenyReason* reason, bool ignoreGroup)
{
    // Basic pointer validity checks
    if (!bot || !from || !from->GetSession())
    {
        if (reason)
            *reason = PLAYERBOT_DENY_NONE;

        return PLAYERBOT_SECURITY_DENY_ALL;
    }

    // GMs always have full access
    if (from->GetSession()->GetSecurity() >= SEC_GAMEMASTER)
        return PLAYERBOT_SECURITY_ALLOW_ALL;

    PlayerbotAI* botAI = GET_PLAYERBOT_AI(bot);
    if (!botAI)
    {
        if (reason)
            *reason = PLAYERBOT_DENY_NONE;

        return PLAYERBOT_SECURITY_DENY_ALL;
    }

    if (botAI->IsOpposing(from))
    {
        if (reason)
            *reason = PLAYERBOT_DENY_OPPOSING;

        return PLAYERBOT_SECURITY_DENY_ALL;
    }

    if (sPlayerbotAIConfig.IsInRandomAccountList(account))
    {
        // (duplicate check in case of faction change)
        if (botAI->IsOpposing(from))
        {
            if (reason)
                *reason = PLAYERBOT_DENY_OPPOSING;

            return PLAYERBOT_SECURITY_DENY_ALL;
        }

        Group* fromGroup = from->GetGroup();
        Group* botGroup = bot->GetGroup();

        if (fromGroup && botGroup && fromGroup == botGroup && !ignoreGroup)
        {
            if (botAI->GetMaster() == from)
                return PLAYERBOT_SECURITY_ALLOW_ALL;

            if (reason)
                *reason = PLAYERBOT_DENY_NOT_YOURS;

            return PLAYERBOT_SECURITY_TALK;
        }

        if (sPlayerbotAIConfig.groupInvitationPermission <= 0)
        {
            if (reason)
                *reason = PLAYERBOT_DENY_NONE;

            return PLAYERBOT_SECURITY_TALK;
        }

        if (sPlayerbotAIConfig.groupInvitationPermission <= 1)
        {
            int32 levelDiff = int32(bot->GetLevel()) - int32(from->GetLevel());
            if (levelDiff > 5)
            {
                if (!bot->GetGuildId() || bot->GetGuildId() != from->GetGuildId())
                {
                    if (reason)
                        *reason = PLAYERBOT_DENY_LOW_LEVEL;

                    return PLAYERBOT_SECURITY_TALK;
                }
            }
        }

        int32 botGS = static_cast<int32>(botAI->GetEquipGearScore(bot));
        int32 fromGS = static_cast<int32>(botAI->GetEquipGearScore(from));

        if (sPlayerbotAIConfig.gearscorecheck && botGS && bot->GetLevel() > 15 && botGS > fromGS)
        {
            uint32 diffPct = uint32(100 * (botGS - fromGS) / botGS);
            uint32 reqPct = uint32(12 * sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) / from->GetLevel());

            if (diffPct >= reqPct)
            {
                if (reason)
                    *reason = PLAYERBOT_DENY_GEARSCORE;

                return PLAYERBOT_SECURITY_TALK;
            }
        }

        if (bot->InBattlegroundQueue())
        {
            if (!bot->GetGuildId() || bot->GetGuildId() != from->GetGuildId())
            {
                if (reason)
                    *reason = PLAYERBOT_DENY_BG;

                return PLAYERBOT_SECURITY_TALK;
            }
        }

        // If the bot is not in the group, we offer an invite
        botGroup = bot->GetGroup();
        if (!botGroup)
        {
            if (reason)
                *reason = PLAYERBOT_DENY_INVITE;

            return PLAYERBOT_SECURITY_INVITE;
        }

        if (!ignoreGroup && botGroup->IsFull())
        {
            if (reason)
                *reason = PLAYERBOT_DENY_FULL_GROUP;

            return PLAYERBOT_SECURITY_TALK;
        }

        if (!ignoreGroup && botGroup->GetLeaderGUID() != bot->GetGUID())
        {
            if (reason)
                *reason = PLAYERBOT_DENY_NOT_LEADER;

            return PLAYERBOT_SECURITY_TALK;
        }

        // The bot is the group leader, you can invite the initiator
        if (reason)
            *reason = PLAYERBOT_DENY_IS_LEADER;

        return PLAYERBOT_SECURITY_INVITE;
    }

    // Non-random bots: only their master has full access
    if (botAI->GetMaster() == from)
        return PLAYERBOT_SECURITY_ALLOW_ALL;

    if (reason)
        *reason = PLAYERBOT_DENY_NOT_YOURS;

    return PLAYERBOT_SECURITY_INVITE;
}

bool PlayerbotSecurity::CheckLevelFor(PlayerbotSecurityLevel level, bool silent, Player* from, bool ignoreGroup)
{
    // If something is wrong with the pointers, we silently refuse
    if (!bot || !from || !from->GetSession())
        return false;

    DenyReason reason = PLAYERBOT_DENY_NONE;
    PlayerbotSecurityLevel realLevel = LevelFor(from, &reason, ignoreGroup);

    if (realLevel >= level || from == bot)
        return true;

    PlayerbotAI* fromBotAI = GET_PLAYERBOT_AI(from);
    if (silent || (fromBotAI && !fromBotAI->IsRealPlayer()))
        return false;

    PlayerbotAI* botAI = GET_PLAYERBOT_AI(bot);
    if (!botAI)
        return false;

    Player* master = botAI->GetMaster();
    if (master && botAI->IsOpposing(master))
        if (WorldSession* session = master->GetSession())
            if (session->GetSecurity() < SEC_GAMEMASTER)
                return false;

    std::string text;
    switch (realLevel)
    {
        case PLAYERBOT_SECURITY_DENY_ALL:
            text = botAI->GetLocalizedBotTextOrDefault("security_busy", "I'm kind of busy now");
            break;
        case PLAYERBOT_SECURITY_TALK:
            switch (reason)
            {
                case PLAYERBOT_DENY_NONE:
                    text = botAI->GetLocalizedBotTextOrDefault("security_do_later", "I'll do it later");
                    break;
                case PLAYERBOT_DENY_LOW_LEVEL:
                    text = botAI->GetLocalizedBotTextOrDefault("security_low_level", "You are too low level: |cffff0000%from|cffffffff/|cff00ff00%bot",
                        {{"%from", std::to_string(from->GetLevel())}, {"%bot", std::to_string(bot->GetLevel())}});
                    break;
                case PLAYERBOT_DENY_GEARSCORE:
                {
                    int botGS = int(botAI->GetEquipGearScore(bot));
                    int fromGS = int(botAI->GetEquipGearScore(from));
                    int diff = (100 * (botGS - fromGS) / std::max(botGS, 1));
                    int fromLevel = static_cast<int>(from->GetLevel());
                    int req = 12 * sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) / std::max(fromLevel, 1);
                    text = botAI->GetLocalizedBotTextOrDefault("security_gearscore_low", "Your gearscore is too low: |cffff0000%from|cffffffff/|cff00ff00%bot |cffff0000%diff%|cffffffff/|cff00ff00%req%",
                        {{"%from", std::to_string(fromGS)}, {"%bot", std::to_string(botGS)}, {"%diff", std::to_string(diff)}, {"%req", std::to_string(req)}});
                    break;
                }
                case PLAYERBOT_DENY_NOT_YOURS:
                    text = botAI->GetLocalizedBotTextOrDefault("security_has_master", "I have a master already");
                    break;
                case PLAYERBOT_DENY_IS_BOT:
                    text = botAI->GetLocalizedBotTextOrDefault("security_you_bot", "You are a bot");
                    break;
                case PLAYERBOT_DENY_OPPOSING:
                    text = botAI->GetLocalizedBotTextOrDefault("security_you_enemy", "You are the enemy");
                    break;
                case PLAYERBOT_DENY_DEAD:
                    text = botAI->GetLocalizedBotTextOrDefault("security_dead", "I'm dead. Will do it later");
                    break;
                case PLAYERBOT_DENY_INVITE:
                    text = botAI->GetLocalizedBotTextOrDefault("security_invite_first", "Invite me to your group first");
                    break;
                case PLAYERBOT_DENY_FAR:
                {
                    std::string areaStr;
                    if (AreaTableEntry const* entry = sAreaTableStore.LookupEntry(bot->GetAreaId()))
                        areaStr = " |cffffffff(|cffff0000" + std::string(entry->area_name[0]) + "|cffffffff)";
                    text = botAI->GetLocalizedBotTextOrDefault("security_too_far", "You must be closer to invite me to your group. I am in %area", {{"%area", areaStr}});
                    break;
                }
                case PLAYERBOT_DENY_FULL_GROUP:
                    text = botAI->GetLocalizedBotTextOrDefault("security_full_group", "I am in a full group. Will do it later");
                    break;
                case PLAYERBOT_DENY_IS_LEADER:
                    text = botAI->GetLocalizedBotTextOrDefault("security_am_leader", "I am currently leading a group. I can invite you if you want.");
                    break;
                case PLAYERBOT_DENY_NOT_LEADER:
                    if (Player* leader = botAI->GetGroupLeader())
                        text = botAI->GetLocalizedBotTextOrDefault("security_in_group_with", "I am in a group with %name. You can ask him for invite.", {{"%name", leader->GetName()}});
                    else
                        text = botAI->GetLocalizedBotTextOrDefault("security_in_group_other", "I am in a group with someone else. You can ask him for invite.");
                    break;
                case PLAYERBOT_DENY_BG:
                    text = botAI->GetLocalizedBotTextOrDefault("security_in_bg_queue", "I am in a queue for BG. Will do it later");
                    break;
                case PLAYERBOT_DENY_LFG:
                    text = botAI->GetLocalizedBotTextOrDefault("security_in_lfg_queue", "I am in a queue for dungeon. Will do it later");
                    break;
                default:
                    text = botAI->GetLocalizedBotTextOrDefault("security_cant_do", "I can't do that");
                    break;
            }
            break;
        case PLAYERBOT_SECURITY_INVITE:
            text = botAI->GetLocalizedBotTextOrDefault("security_invite_first", "Invite me to your group first");
            break;
        default:
            text = botAI->GetLocalizedBotTextOrDefault("security_cant_do", "I can't do that");
            break;
    }

    if (text.empty())
        return false;
    ObjectGuid guid = from->GetGUID();
    time_t lastSaid = whispers[guid][text];

    if (!lastSaid || (time(nullptr) - lastSaid) >= sPlayerbotAIConfig.repeatDelay / 1000)
    {
        whispers[guid][text] = time(nullptr);

        // Additional protection against crashes during logout
        if (bot->IsInWorld() && from->IsInWorld())
            bot->Whisper(text, LANG_UNIVERSAL, from);
    }

    return false;
}
