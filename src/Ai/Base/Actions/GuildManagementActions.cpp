/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "GuildManagementActions.h"

#include "GuildMgr.h"
#include "GuildPackets.h"
#include "Playerbots.h"
#include "ServerFacade.h"
#include "BroadcastHelper.h"

Player* GuidManageAction::GetPlayer(Event event)
{
    Player* player = nullptr;
    ObjectGuid guid = event.getObject();

    if (guid)
    {
        player = ObjectAccessor::FindPlayer(guid);

        if (player)
            return player;
    }

    std::string text = event.getParam();

    if (!text.empty())
    {
        if (normalizePlayerName(text))
        {
            player = ObjectAccessor::FindPlayerByName(text.c_str());

            if (player)
                return player;
        }

        return nullptr;
    }

    Player* master = GetMaster();
    if (!master)
        guid = bot->GetTarget();
    else
        guid = master->GetTarget();

    player = ObjectAccessor::FindPlayer(guid);

    if (player)
        return player;

    player = event.getOwner();

    if (player)
        return player;

    return nullptr;
}

void GuidManageAction::SendPacket(WorldPacket const& packet)
{
    // make a heap copy because QueuePacket takes ownership
    WorldPacket* data = new WorldPacket(packet);

    bot->GetSession()->QueuePacket(data);
}

bool GuidManageAction::Execute(Event event)
{
    Player* player = GetPlayer(event);

    if (!player || !PlayerIsValid(player) || player == bot)
        return false;

    WorldPacket data(opcode);
    data << player->GetName();
    SendPacket(data);

    return true;
}

bool GuidManageAction::PlayerIsValid(Player* member) { return !member->GetGuildId(); }

uint8 GuidManageAction::GetRankId(Player* member)
{
    return sGuildMgr->GetGuildById(member->GetGuildId())->GetMember(member->GetGUID())->GetRankId();
}

bool GuildInviteAction::isUseful()
{
    return bot->GetGuildId() && sGuildMgr->GetGuildById(bot->GetGuildId())->HasRankRight(bot, GR_RIGHT_INVITE);
}

bool GuildInviteAction::PlayerIsValid(Player* member)
{
    return !member->GetGuildId() && (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) ||
                                     (bot->GetTeamId() == member->GetTeamId()));
}

bool GuildPromoteAction::isUseful()
{
    return bot->GetGuildId() && sGuildMgr->GetGuildById(bot->GetGuildId())->HasRankRight(bot, GR_RIGHT_PROMOTE);
}

bool GuildPromoteAction::PlayerIsValid(Player* member)
{
    return member->GetGuildId() == bot->GetGuildId() && GetRankId(bot) < GetRankId(member) - 1;
}

bool GuildDemoteAction::isUseful()
{
    return bot->GetGuildId() && sGuildMgr->GetGuildById(bot->GetGuildId())->HasRankRight(bot, GR_RIGHT_DEMOTE);
}

bool GuildDemoteAction::PlayerIsValid(Player* member)
{
    return member->GetGuildId() == bot->GetGuildId() && GetRankId(bot) < GetRankId(member);
}

bool GuildRemoveAction::isUseful()
{
    return bot->GetGuildId() && sGuildMgr->GetGuildById(bot->GetGuildId())->HasRankRight(bot, GR_RIGHT_REMOVE);
}

bool GuildRemoveAction::PlayerIsValid(Player* member)
{
    return member->GetGuildId() == bot->GetGuildId() && GetRankId(bot) < GetRankId(member);
};

bool GuildManageNearbyAction::Execute(Event /*event*/)
{
    uint32 found = 0;

    Guild* guild = sGuildMgr->GetGuildById(bot->GetGuildId());
    Guild::Member* botMember = guild->GetMember(bot->GetGUID());

    GuidVector nearGuids = botAI->GetAiObjectContext()->GetValue<GuidVector>("nearest friendly players")->Get();
    for (auto& guid : nearGuids)
    {
        Player* player = ObjectAccessor::FindPlayer(guid);

        if (!player || bot == player)
            continue;

        if (player->isDND())
            continue;

        // Promote or demote nearby members based on chance.
        if (player->GetGuildId() && player->GetGuildId() == bot->GetGuildId())
        {
            uint32 dCount = AI_VALUE(uint32, "death count");

            if (!urand(0, 30) && dCount < 2 && guild->GetRankRights(botMember->GetRankId()) & GR_RIGHT_PROMOTE)
            {
                BroadcastHelper::BroadcastGuildMemberPromotion(botAI, bot, player);

                botAI->DoSpecificAction("guild promote", Event("guild management", guid), true);
                continue;
            }

            if (!urand(0, 30) && dCount > 2 && guild->GetRankRights(botMember->GetRankId()) & GR_RIGHT_DEMOTE)
            {
                BroadcastHelper::BroadcastGuildMemberDemotion(botAI, bot, player);

                botAI->DoSpecificAction("guild demote", Event("guild management", guid), true);
                continue;
            }

            continue;
        }

        if (!sPlayerbotAIConfig.randomBotGuildNearby)
            return false;

        if (guild->GetMemberSize() > 1000)
            return false;

        if ((guild->GetRankRights(botMember->GetRankId()) & GR_RIGHT_INVITE) == 0)
            continue;

        if (player->GetGuildIdInvited())
            continue;

        PlayerbotAI* botAi = GET_PLAYERBOT_AI(player);

        if (!sPlayerbotAIConfig.randomBotInvitePlayer && botAi && botAi->IsRealPlayer())
            continue;

        if (botAi)
        {
            if (botAi->GetGuilderType() == GuilderType::SOLO && !botAi->HasRealPlayerMaster()) //Do not invite solo players.
                continue;

            if (botAi->HasActivePlayerMaster() && !sRandomPlayerbotMgr.IsRandomBot(player)) //Do not invite alts of active players.
                continue;
        }

        bool sameGroup = bot->GetGroup() && bot->GetGroup()->IsMember(player->GetGUID());

        if (!sameGroup && ServerFacade::instance().GetDistance2d(bot, player) > sPlayerbotAIConfig.spellDistance)
            continue;

        if (sPlayerbotAIConfig.inviteChat && (sRandomPlayerbotMgr.IsRandomBot(bot) || !botAI->HasActivePlayerMaster()))
        {
            std::map<std::string, std::string> placeholders;
            placeholders["%name"] = player->GetName();
            placeholders["%members"] = std::to_string(guild->GetMemberSize());
            placeholders["%guildname"] = guild->GetName();
            AreaTableEntry const* current_area = botAI->GetCurrentArea();
            AreaTableEntry const* current_zone = botAI->GetCurrentZone();
            placeholders["%area_name"] = current_area ? botAI->GetLocalizedAreaName(current_area) : PlayerbotTextMgr::instance().GetBotText("string_unknown_area");
            placeholders["%zone_name"] = current_zone ? botAI->GetLocalizedAreaName(current_zone) : PlayerbotTextMgr::instance().GetBotText("string_unknown_area");

            std::vector<std::string> lines;

            switch ((urand(0, 10) * urand(0, 10)) / 10)
            {
            case 0:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_hey_want_join", placeholders));
                break;
            case 1:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_hey_man", placeholders));
                break;
            case 2:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_good_contribution", placeholders));
                break;
            case 3:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_quality_members", placeholders));
                break;
            case 4:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_number_one", placeholders));
                break;
            case 5:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_no_smalltalk", placeholders));
                break;
            case 6:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_welcome_zone", placeholders));
                break;
            case 7:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_should_join", placeholders));
                break;
            case 8:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_got_guild", placeholders));
                break;
            case 9:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_going_to_join", placeholders));
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_raid_molten", placeholders));
                break;
            case 10:
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_hey_gild", placeholders));
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_friendly_high_levels", placeholders));
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_homework", placeholders));
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_mc_raids", placeholders));
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_more_members", placeholders));
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_lonely", placeholders));
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_crying", placeholders));
                lines.push_back(PlayerbotTextMgr::instance().GetBotText("guild_invite_so_join", placeholders));
                break;
            }

            for (auto const& line : lines)
            {
                if (line.empty())
                    continue;

                if (sameGroup)
                {
                    WorldPacket data;
                    ChatHandler::BuildChatPacket(data, bot->GetGroup()->isRaidGroup() ? CHAT_MSG_RAID : CHAT_MSG_PARTY, line.c_str(), LANG_UNIVERSAL, CHAT_TAG_NONE, bot->GetGUID(), bot->GetName());
                    bot->GetGroup()->BroadcastPacket(&data, true);
                }
                else
                    bot->Say(line, (bot->GetTeamId() == TEAM_ALLIANCE ? LANG_COMMON : LANG_ORCISH));
            }
        }

        if (botAI->DoSpecificAction("guild invite", Event("guild management", guid), true))
        {
            if (sPlayerbotAIConfig.inviteChat)
                return true;
            found++;
        }
    }

    return found > 0;
}

bool GuildManageNearbyAction::isUseful()
{
    if (!bot->GetGuildId())
        return false;

    Guild* guild = sGuildMgr->GetGuildById(bot->GetGuildId());
    Guild::Member* botMember = guild->GetMember(bot->GetGUID());

    return guild->GetRankRights(botMember->GetRankId()) & (GR_RIGHT_DEMOTE | GR_RIGHT_PROMOTE | GR_RIGHT_INVITE);
}

bool GuildLeaveAction::Execute(Event event)
{
    Player* owner = event.getOwner();
    if (owner && !botAI->GetSecurity()->CheckLevelFor(PLAYERBOT_SECURITY_INVITE, false, owner, true))
    {
        botAI->TellError(botAI->GetLocalizedBotTextOrDefault("error_guild_happy", "Sorry, I am happy in my guild :)"));
        return false;
    }

    WorldPackets::Guild::GuildLeave data = WorldPacket(CMSG_GUILD_LEAVE);
    bot->GetSession()->HandleGuildLeaveOpcode(data);
    return true;
}

bool GuildLeaveAction::isUseful() { return bot->GetGuildId(); }
