/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PLAYERBOTTEXTHELPER_H
#define _PLAYERBOT_PLAYERBOTTEXTHELPER_H

#include <map>
#include <string>

#include "ObjectAccessor.h"
#include "ObjectGuid.h"
#include "Player.h"
#include "PlayerbotTextMgr.h"
#include "World.h"

inline uint32 GetPlayerSessionLocale(Player* player)
{
    if (player && player->GetSession())
        return static_cast<uint32>(player->GetSession()->GetSessionDbcLocale());

    return static_cast<uint32>(sWorld->GetDefaultDbcLocale());
}

inline std::string GetPlayerLocalizedText(Player* player, std::string const key, std::string const defaultText,
                                          std::map<std::string, std::string> placeholders = {})
{
    std::string localized =
        PlayerbotTextMgr::instance().GetBotTextForLocale(key, GetPlayerSessionLocale(player), placeholders);
    if (!localized.empty())
        return localized;

    std::string result = defaultText;
    for (auto const& placeholder : placeholders)
        PlayerbotTextMgr::replaceAll(result, placeholder.first, placeholder.second);

    return result;
}

inline std::string GetMasterLocalizedText(ObjectGuid masterGuid, std::string const key, std::string const defaultText,
                                          std::map<std::string, std::string> placeholders = {})
{
    return GetPlayerLocalizedText(ObjectAccessor::FindConnectedPlayer(masterGuid), key, defaultText, placeholders);
}

inline std::string GetMasterLocalizedText(Player* master, std::string const key, std::string const defaultText,
                                          std::map<std::string, std::string> placeholders = {})
{
    return GetMasterLocalizedText(master ? master->GetGUID() : ObjectGuid::Empty, key, defaultText, placeholders);
}

#endif
