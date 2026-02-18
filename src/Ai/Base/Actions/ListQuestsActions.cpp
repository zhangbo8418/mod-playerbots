/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "ListQuestsActions.h"

#include "Event.h"
#include "Playerbots.h"

bool ListQuestsAction::Execute(Event event)
{
    if (event.getParam() == "completed" || event.getParam() == "co")
    {
        ListQuests(QUEST_LIST_FILTER_COMPLETED);
    }
    else if (event.getParam() == "incompleted" || event.getParam() == "in")
    {
        ListQuests(QUEST_LIST_FILTER_INCOMPLETED);
    }
    else if (event.getParam() == "all")
    {
        ListQuests(QUEST_LIST_FILTER_ALL);
    }
    else if (event.getParam() == "travel")
    {
        ListQuests(QUEST_LIST_FILTER_ALL, QUEST_TRAVEL_DETAIL_SUMMARY);
    }
    else if (event.getParam() == "travel detail")
    {
        ListQuests(QUEST_LIST_FILTER_ALL, QUEST_TRAVEL_DETAIL_FULL);
    }
    else
    {
        ListQuests(QUEST_LIST_FILTER_SUMMARY);
    }

    return true;
}

void ListQuestsAction::ListQuests(QuestListFilter filter, QuestTravelDetail travelDetail)
{
    bool showIncompleted = filter & QUEST_LIST_FILTER_INCOMPLETED;
    bool showCompleted = filter & QUEST_LIST_FILTER_COMPLETED;

    if (showIncompleted)
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_quest_incompleted_title", "--- Incompleted quests ---"));

    uint32 incompleteCount = ListQuests(false, !showIncompleted, travelDetail);

    if (showCompleted)
        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_quest_completed_title", "--- Completed quests ---"));

    uint32 completeCount = ListQuests(true, !showCompleted, travelDetail);

    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_quest_summary_title", "--- Summary ---"));
    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_quest_total", "Total: %total / 25 (incompleted: %incomplete, completed: %complete)",
        {{"%total", std::to_string(completeCount + incompleteCount)}, {"%incomplete", std::to_string(incompleteCount)}, {"%complete", std::to_string(completeCount)}}));
}

uint32 ListQuestsAction::ListQuests(bool completed, bool silent, QuestTravelDetail travelDetail)
{
    TravelTarget* target;
    WorldPosition botPos(bot);
    if (travelDetail != QUEST_TRAVEL_DETAIL_NONE)
        target = context->GetValue<TravelTarget*>("travel target")->Get();

    uint32 count = 0;
    for (uint16 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
    {
        uint32 questId = bot->GetQuestSlotQuestId(slot);
        if (!questId)
            continue;

        Quest const* pQuest = sObjectMgr->GetQuestTemplate(questId);
        bool isCompletedQuest = bot->GetQuestStatus(questId) == QUEST_STATUS_COMPLETE;
        if (completed != isCompletedQuest)
            continue;

        ++count;

        if (silent)
            continue;

        botAI->TellMaster(chat->FormatQuest(pQuest));

        if (travelDetail != QUEST_TRAVEL_DETAIL_NONE && target->getDestination())
        {
            if (target->getDestination()->getName() == "QuestRelationTravelDestination" ||
                target->getDestination()->getName() == "QuestObjectiveTravelDestination")
            {
                QuestTravelDestination* QuestDestination = (QuestTravelDestination*)target->getDestination();

                if (QuestDestination->GetQuestTemplate()->GetQuestId() == questId)
                {
                    std::string msg = botAI->GetLocalizedBotTextOrDefault("msg_quest_active_travel", "[Active] traveling %dist to %title",
                        {{"%dist", std::to_string(static_cast<uint32>(target->getPosition()->distance(botPos)))}, {"%title", QuestDestination->getTitle()}});
                    botAI->TellMaster(msg);
                }
            }
        }

        if (travelDetail == QUEST_TRAVEL_DETAIL_SUMMARY)
        {
            std::vector<TravelDestination*> allDestinations =
                TravelMgr::instance().getQuestTravelDestinations(bot, questId, true, true, -1);
            std::vector<TravelDestination*> availDestinations =
                TravelMgr::instance().getQuestTravelDestinations(bot, questId, botAI->GetMaster(), false, -1);

            uint32 desTot = allDestinations.size();
            uint32 desAvail = availDestinations.size();
            uint32 desFull = desAvail - TravelMgr::instance().getQuestTravelDestinations(bot, questId, false, false, -1).size();
            uint32 desRange = desAvail - TravelMgr::instance().getQuestTravelDestinations(bot, questId, false, false).size();

            uint32 tpoints = 0;
            uint32 apoints = 0;

            for (auto dest : allDestinations)
                tpoints += dest->getPoints(true).size();

            for (auto dest : availDestinations)
                apoints += dest->getPoints().size();

            std::string crowdedStr = (desFull > 0) ? botAI->GetLocalizedBotTextOrDefault("msg_quest_crowded", "%n crowded.", {{"%n", std::to_string(desFull)}}) : "";
            std::string rangeStr = (desRange > 0) ? botAI->GetLocalizedBotTextOrDefault("msg_quest_out_of_range", "%n out of range.", {{"%n", std::to_string(desRange)}}) : "";
            std::string msg = botAI->GetLocalizedBotTextOrDefault("msg_quest_dest_summary", "%avail%/%tot% destinations %ap%/%tp% points. %crowded%%range%",
                {{"%avail%", std::to_string(desAvail)}, {"%tot%", std::to_string(desTot)}, {"%ap%", std::to_string(apoints)}, {"%tp%", std::to_string(tpoints)}, {"%crowded%", crowdedStr}, {"%range%", rangeStr}});
            botAI->TellMaster(msg);
        }
        else if (travelDetail == QUEST_TRAVEL_DETAIL_FULL)
        {
            uint32 limit = 0;
            std::vector<TravelDestination*> allDestinations =
                TravelMgr::instance().getQuestTravelDestinations(bot, questId, true, true, -1);

            std::sort(allDestinations.begin(), allDestinations.end(),
                      [botPos](TravelDestination* i, TravelDestination* j) {
                          return i->distanceTo(const_cast<WorldPosition*>(&botPos)) <
                                 j->distanceTo(const_cast<WorldPosition*>(&botPos));
                      });

            for (auto dest : allDestinations)
            {
                if (limit > 5)
                    continue;

                uint32 tpoints = dest->getPoints(true).size();
                uint32 apoints = dest->getPoints().size();
                std::string pointsStr = std::to_string(apoints);
                if (apoints < tpoints)
                    pointsStr += "/" + std::to_string(tpoints);
                std::string extra;
                if (!dest->isActive(bot))
                    extra += botAI->GetLocalizedBotTextOrDefault("msg_quest_not_active", " not active");
                if (dest->isFull(bot))
                    extra += botAI->GetLocalizedBotTextOrDefault("msg_quest_crowded_suffix", " crowded");
                std::string msg = botAI->GetLocalizedBotTextOrDefault("msg_quest_dest_line", "%dist to %title %points points.%extra",
                    {{"%dist", std::to_string(static_cast<uint32>(round(dest->distanceTo(const_cast<WorldPosition*>(&botPos)))))}, {"%title", dest->getTitle()}, {"%points", pointsStr}, {"%extra", extra}});
                botAI->TellMaster(msg);

                limit++;
            }
        }
    }

    return count;
}
