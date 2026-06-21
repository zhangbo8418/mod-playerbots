/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_PLAYERBOTMAILSUBJECTS_H
#define _PLAYERBOT_PLAYERBOTMAILSUBJECTS_H

namespace PlayerbotMailSubjects
{
// Fixed English subjects used for mail logic matching (see CheckMailAction).
inline constexpr char const ItemsAsked[] = "Item(s) you asked for";
inline constexpr char const MoneyAsked[] = "Money you asked for";
inline constexpr char const ItemsSentByMistake[] = "Item(s) you've sent me";
}

#endif
