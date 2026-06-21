/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "CheckMailAction.h"

#include "Event.h"
#include "GuildTaskMgr.h"
#include "PlayerbotAIConfig.h"
#include "PlayerbotAI.h"
#include "PlayerbotMailSubjects.h"
#include "Playerbots.h"

bool CheckMailAction::Execute(Event /*event*/)
{
    WorldPacket p;
    bot->GetSession()->HandleQueryNextMailTime(p);

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    std::vector<uint32> ids;
    for (PlayerMails::const_iterator i = bot->GetMails().begin(); i != bot->GetMails().end(); ++i)
    {
        Mail* mail = *i;
        if (!mail || mail->state == MAIL_STATE_DELETED)
            continue;

        Player* owner = ObjectAccessor::FindConnectedPlayer(ObjectGuid::Create<HighGuid::Player>(mail->sender));
        if (!owner)
            continue;

        uint32 account = owner->GetSession()->GetAccountId();
        if (PlayerbotAIConfig::instance().IsInRandomAccountList(account))
            continue;

        ProcessMail(mail, owner, trans);
        ids.push_back(mail->messageID);
        mail->state = MAIL_STATE_DELETED;
    }

    for (uint32 id : ids)
    {
        bot->SendMailResult(id, MAIL_DELETED, MAIL_OK);

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
        stmt->SetData(0, id);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEM_BY_ID);
        stmt->SetData(0, id);
        trans->Append(stmt);

        bot->RemoveMail(id);
    }

    CharacterDatabase.CommitTransaction(trans);

    return true;
}

bool CheckMailAction::isUseful()
{
    if (botAI->GetMaster() || !bot->GetMailSize() || bot->InBattleground())
        return false;

    return true;
}

void CheckMailAction::ProcessMail(Mail* mail, Player* owner, CharacterDatabaseTransaction trans)
{
    if (mail->items.empty())
    {
        return;
    }

    if (mail->subject.find(PlayerbotMailSubjects::ItemsAsked) != std::string::npos)
        return;

    for (MailItemInfoVec::iterator i = mail->items.begin(); i != mail->items.end(); ++i)
    {
        Item* item = bot->GetMItem(i->item_guid);
        if (!item)
            continue;

        if (!GuildTaskMgr::instance().CheckItemTask(i->item_template, item->GetCount(), owner, bot, true))
        {
            std::ostringstream body;
            body << botAI->GetLocalizedBotTextOrDefault("mail_body_greeting", "Hello, %name,\n\n",
                {{"%name", owner->GetName()}}, owner);
            body << botAI->GetLocalizedBotTextOrDefault("mail_body_items_mistake",
                "Here are the item(s) you've sent me by mistake", {}, owner);
            body << "\n\n";
            body << botAI->GetLocalizedBotTextOrDefault("mail_body_signoff", "Thanks,\n%botname\n",
                {{"%botname", bot->GetName()}}, owner);

            MailDraft draft(PlayerbotMailSubjects::ItemsSentByMistake, body.str());
            draft.AddItem(item);
            bot->RemoveMItem(i->item_guid);
            draft.SendMailTo(trans, MailReceiver(owner), MailSender(bot));
            return;
        }

        bot->RemoveMItem(i->item_guid);
        item->DestroyForPlayer(bot);
    }
}
