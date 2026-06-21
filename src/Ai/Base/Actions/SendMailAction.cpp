/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "SendMailAction.h"

#include "ChatHelper.h"
#include "Event.h"
#include "ItemVisitors.h"
#include "Mail.h"
#include "PlayerbotMailSubjects.h"
#include "PlayerbotTextMgr.h"
#include "Playerbots.h"

bool SendMailAction::Execute(Event event)
{
    uint32 account = bot->GetSession()->GetAccountId();
    bool randomBot = sPlayerbotAIConfig.IsInRandomAccountList(account);

    GuidVector gos = *context->GetValue<GuidVector>("nearest game objects");
    bool mailboxFound = false;
    for (ObjectGuid const guid : gos)
    {
        if (GameObject* go = botAI->GetGameObject(guid))
            if (go->GetGoType() == GAMEOBJECT_TYPE_MAILBOX)
            {
                mailboxFound = true;
                break;
            }
    }

    std::string const text = event.getParam();

    Player* receiver = GetMaster();
    Player* tellTo = receiver;

    if (!receiver)
        receiver = event.getOwner();

    if (!receiver || receiver == bot)
        return false;

    if (!tellTo)
        tellTo = receiver;

    if (!sPlayerbotAIConfig.botSendMailEnabled)
    {
        bot->Whisper(botAI->GetLocalizedBotTextOrDefault("send_mail_disabled", "I cannot send mail", {}, tellTo), LANG_UNIVERSAL, tellTo);
        return false;
    }

    if (!mailboxFound && !randomBot)
    {
bot->Whisper(botAI->GetLocalizedBotTextOrDefault("send_mail_no_mailbox_nearby", "There is no mailbox nearby", {}, tellTo), LANG_UNIVERSAL, tellTo);
        return false;
    }

    ItemIds ids = chat->parseItems(text);
    if (ids.size() > 1)
    {
bot->Whisper(botAI->GetLocalizedBotTextOrDefault("send_mail_one_item_only", "You can not request more than one item", {}, tellTo), LANG_UNIVERSAL, tellTo);
        return false;
    }

    if (ids.empty())
    {
        uint32 money = chat->parseMoney(text);
        if (!money)
            return false;

        if (randomBot)
        {
bot->Whisper(botAI->GetLocalizedBotTextOrDefault("send_mail_cannot_send_money", "I cannot send money", {}, tellTo), LANG_UNIVERSAL, tellTo);
            return false;
        }

        if (bot->GetMoney() < money)
        {
botAI->TellError(botAI->GetLocalizedBotTextOrDefault("send_mail_not_enough_money", "I don't have enough money"));
            return false;
        }

        std::ostringstream body;
        body << botAI->GetLocalizedBotTextOrDefault("mail_body_greeting", "Hello, %name,\n\n",
            {{"%name", receiver->GetName()}}, receiver);
        body << botAI->GetLocalizedBotTextOrDefault("mail_body_money_asked", "Here is the money you asked for", {}, receiver);
        body << "\n\n";
        body << botAI->GetLocalizedBotTextOrDefault("mail_body_signoff", "Thanks,\n%botname\n",
            {{"%botname", bot->GetName()}}, receiver);

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

        MailDraft draft(PlayerbotMailSubjects::MoneyAsked, body.str());
        draft.AddMoney(money);
        bot->SetMoney(bot->GetMoney() - money);
        draft.SendMailTo(trans, MailReceiver(receiver), MailSender(bot));

        CharacterDatabase.CommitTransaction(trans);

        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("send_mail_sending_to", "Sending mail to %receiver", {{"%receiver", receiver->GetName()}}));
        return true;
    }

    std::ostringstream body;
    body << botAI->GetLocalizedBotTextOrDefault("mail_body_greeting", "Hello, %name,\n\n",
        {{"%name", receiver->GetName()}}, receiver);
    body << botAI->GetLocalizedBotTextOrDefault("mail_body_items_asked", "Here are the item(s) you asked for", {}, receiver);
    body << "\n\n";
    body << botAI->GetLocalizedBotTextOrDefault("mail_body_signoff", "Thanks,\n%botname\n",
        {{"%botname", bot->GetName()}}, receiver);

    MailDraft draft(PlayerbotMailSubjects::ItemsAsked, body.str());
    for (ItemIds::iterator i = ids.begin(); i != ids.end(); i++)
    {
        FindItemByIdVisitor visitor(*i);
        IterateItems(&visitor, ITERATE_ITEMS_IN_BAGS);

        std::vector<Item*> items = visitor.GetResult();
        for (Item* item : items)
        {
            if (item->IsSoulBound() || item->IsConjuredConsumable())
            {
                bot->Whisper(botAI->GetLocalizedBotTextOrDefault("send_mail_cannot_send_item", "Cannot send %item", {{"%item", ChatHelper::FormatItem(item->GetTemplate())}}, tellTo), LANG_UNIVERSAL, tellTo);
                continue;
            }

            ItemTemplate const* proto = item->GetTemplate();
            if (!proto)
                continue;

            if (randomBot)
            {
                uint32 price = item->GetCount() * proto->SellPrice;
                if (!price)
                {
                    bot->Whisper(botAI->GetLocalizedBotTextOrDefault("send_mail_item_not_for_sale", "%item: it is not for sale", {{"%item", ChatHelper::FormatItem(item->GetTemplate())}}, tellTo), LANG_UNIVERSAL, tellTo);
                    return false;
                }

                draft.AddCOD(price);
            }

            CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

            bot->MoveItemFromInventory(item->GetBagSlot(), item->GetSlot(), true);
            item->DeleteFromInventoryDB(trans);
            item->SetOwnerGUID(receiver->GetGUID());
            item->SaveToDB(trans);
            draft.AddItem(item);
            draft.SendMailTo(trans, MailReceiver(receiver), MailSender(bot));

            CharacterDatabase.CommitTransaction(trans);

            bot->Whisper(botAI->GetLocalizedBotTextOrDefault(
                "send_mail_sent_to", "Sent mail to %receiver",
                {{"%receiver", receiver->GetName()}}, tellTo), LANG_UNIVERSAL, tellTo);
            return true;
        }
    }

    return false;
}
