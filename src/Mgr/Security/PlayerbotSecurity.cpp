/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "PlayerbotSecurity.h"

#include "LFGMgr.h"
#include "PlayerbotAIConfig.h"
#include "PlayerbotTextMgr.h"
#include "Playerbots.h"
#include "SharedDefines.h"

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

    // Use receiver's locale for whisper so private chat can be in their language (like party/guild)
    uint32 locale = from->GetSession()->GetSessionDbLocaleIndex();
    if (locale >= MAX_LOCALES)
        locale = 0;

    std::ostringstream out;

    switch (realLevel)
    {
        case PLAYERBOT_SECURITY_DENY_ALL:
        {
            std::string t = PlayerbotTextMgr::instance().GetBotText("security_busy", locale);
            out << (t.empty() ? "I'm kind of busy now" : t);
            break;
        }
        case PLAYERBOT_SECURITY_TALK:
            switch (reason)
            {
                case PLAYERBOT_DENY_NONE:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_later", locale);
                    out << (t.empty() ? "I'll do it later" : t);
                    break;
                }
                case PLAYERBOT_DENY_LOW_LEVEL:
                {
                    std::map<std::string, std::string> ph;
                    ph["%from_level"] = std::to_string(from->GetLevel());
                    ph["%bot_level"] = std::to_string(bot->GetLevel());
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_low_level", ph, locale);
                    if (t.empty())
                        out << "You are too low level: |cffff0000" << uint32(from->GetLevel()) << "|cffffffff/|cff00ff00"
                            << uint32(bot->GetLevel());
                    else
                        out << t;
                    break;
                }
                case PLAYERBOT_DENY_GEARSCORE:
                {
                    int botGS = int(botAI->GetEquipGearScore(bot));
                    int fromGS = int(botAI->GetEquipGearScore(from));
                    int diff = (100 * (botGS - fromGS) / botGS);
                    int req = 12 * sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) / from->GetLevel();
                    std::map<std::string, std::string> ph;
                    ph["%from_gs"] = std::to_string(fromGS);
                    ph["%bot_gs"] = std::to_string(botGS);
                    ph["%diff"] = std::to_string(diff);
                    ph["%req"] = std::to_string(req);
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_gearscore_low", ph, locale);
                    if (t.empty())
                        out << "Your gearscore is too low: |cffff0000" << fromGS << "|cffffffff/|cff00ff00" << botGS
                            << " |cffff0000" << diff << "%|cffffffff/|cff00ff00" << req << "%";
                    else
                        out << t;
                    break;
                }
                case PLAYERBOT_DENY_NOT_YOURS:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_has_master", locale);
                    out << (t.empty() ? "I have a master already" : t);
                    break;
                }
                case PLAYERBOT_DENY_IS_BOT:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_you_are_bot", locale);
                    out << (t.empty() ? "You are a bot" : t);
                    break;
                }
                case PLAYERBOT_DENY_OPPOSING:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_enemy", locale);
                    out << (t.empty() ? "You are the enemy" : t);
                    break;
                }
                case PLAYERBOT_DENY_DEAD:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_dead", locale);
                    out << (t.empty() ? "I'm dead. Will do it later" : t);
                    break;
                }
                case PLAYERBOT_DENY_INVITE:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_invite_first", locale);
                    out << (t.empty() ? "Invite me to your group first" : t);
                    break;
                }
                case PLAYERBOT_DENY_FAR:
                {
                    std::string areaName;
                    if (AreaTableEntry const* entry = sAreaTableStore.LookupEntry(bot->GetAreaId()))
                        areaName = entry->area_name[0];
                    std::map<std::string, std::string> ph;
                    ph["%area"] = areaName;
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_too_far", ph, locale);
                    if (t.empty())
                    {
                        out << "You must be closer to invite me to your group. I am in ";
                        if (!areaName.empty())
                            out << " |cffffffff(|cffff0000" << areaName << "|cffffffff)";
                    }
                    else
                        out << t;
                    break;
                }
                case PLAYERBOT_DENY_FULL_GROUP:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_full_group", locale);
                    out << (t.empty() ? "I am in a full group. Will do it later" : t);
                    break;
                }
                case PLAYERBOT_DENY_IS_LEADER:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_leading_group", locale);
                    out << (t.empty() ? "I am currently leading a group. I can invite you if you want." : t);
                    break;
                }
                case PLAYERBOT_DENY_NOT_LEADER:
                {
                    std::string leaderName = botAI->GetGroupLeader() ? botAI->GetGroupLeader()->GetName() : "";
                    std::map<std::string, std::string> ph;
                    ph["%leader"] = leaderName;
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_not_leader", ph, locale);
                    if (t.empty())
                    {
                        if (!leaderName.empty())
                            out << "I am in a group with " << leaderName << ". You can ask him for invite.";
                        else
                            out << "I am in a group with someone else. You can ask him for invite.";
                    }
                    else
                        out << t;
                    break;
                }
                case PLAYERBOT_DENY_BG:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_queue_bg", locale);
                    out << (t.empty() ? "I am in a queue for BG. Will do it later" : t);
                    break;
                }
                case PLAYERBOT_DENY_LFG:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_queue_dungeon", locale);
                    out << (t.empty() ? "I am in a queue for dungeon. Will do it later" : t);
                    break;
                }
                default:
                {
                    std::string t = PlayerbotTextMgr::instance().GetBotText("security_cant_do", locale);
                    out << (t.empty() ? "I can't do that" : t);
                    break;
                }
            }
            break;
        case PLAYERBOT_SECURITY_INVITE:
        {
            std::string t = PlayerbotTextMgr::instance().GetBotText("security_invite_first", locale);
            out << (t.empty() ? "Invite me to your group first" : t);
            break;
        }
        default:
        {
            std::string t = PlayerbotTextMgr::instance().GetBotText("security_cant_do", locale);
            out << (t.empty() ? "I can't do that" : t);
            break;
        }
    }

    std::string const text = out.str();
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
