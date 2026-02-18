/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "TellLosAction.h"
#include <istream>
#include <sstream>

#include "ChatHelper.h"
#include "DBCStores.h"
#include "Event.h"
#include "ItemTemplate.h"
#include "ObjectMgr.h"
#include "Playerbots.h"
#include "StatsWeightCalculator.h"
#include "World.h"

bool TellLosAction::Execute(Event event)
{
    std::string const param = event.getParam();

    if (param.empty() || param == "targets")
    {
        ListUnits(botAI->GetLocalizedBotTextOrDefault("msg_los_targets", "--- Targets ---"), *context->GetValue<GuidVector>("possible targets"));
        ListUnits(botAI->GetLocalizedBotTextOrDefault("msg_los_targets_all", "--- Targets (All) ---"), *context->GetValue<GuidVector>("all targets"));
    }

    if (param.empty() || param == "npcs")
    {
        ListUnits(botAI->GetLocalizedBotTextOrDefault("msg_los_npcs", "--- NPCs ---"), *context->GetValue<GuidVector>("nearest npcs"));
    }

    if (param.empty() || param == "corpses")
    {
        ListUnits(botAI->GetLocalizedBotTextOrDefault("msg_los_corpses", "--- Corpses ---"), *context->GetValue<GuidVector>("nearest corpses"));
    }

    if (param.empty() || param == "gos" || param == "game objects")
    {
        ListGameObjects(botAI->GetLocalizedBotTextOrDefault("msg_los_game_objects", "--- Game objects ---"), *context->GetValue<GuidVector>("nearest game objects"));
    }

    if (param.empty() || param == "players")
    {
        ListUnits(botAI->GetLocalizedBotTextOrDefault("msg_los_friendly_players", "--- Friendly players ---"), *context->GetValue<GuidVector>("nearest friendly players"));
    }

    if (param.empty() || param == "triggers")
    {
        ListUnits(botAI->GetLocalizedBotTextOrDefault("msg_los_triggers", "--- Triggers ---"), *context->GetValue<GuidVector>("possible triggers"));
    }

    return true;
}

void TellLosAction::ListUnits(std::string const title, GuidVector units)
{
    botAI->TellMaster(title);

    for (ObjectGuid const guid : units)
    {
        if (Unit* unit = botAI->GetUnit(guid))
        {
            botAI->TellMaster(unit->GetNameForLocaleIdx(sWorld->GetDefaultDbcLocale()));
        }
    }
}
void TellLosAction::ListGameObjects(std::string const title, GuidVector gos)
{
    botAI->TellMaster(title);

    for (ObjectGuid const guid : gos)
    {
        if (GameObject* go = botAI->GetGameObject(guid))
            botAI->TellMaster(chat->FormatGameobject(go));
    }
}

bool TellAuraAction::Execute(Event event)
{
    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_auras_title", "--- Auras ---"));
    sLog->outMessage("playerbot", LOG_LEVEL_DEBUG, "--- Auras ---");
    Unit::AuraApplicationMap& map = bot->GetAppliedAuras();
    for (Unit::AuraApplicationMap::iterator i = map.begin(); i != map.end(); ++i)
    {
        Aura* aura = i->second->GetBase();
        if (!aura)
            continue;
        const std::string auraName = aura->GetSpellInfo()->SpellName[0];
        sLog->outMessage("playerbot", LOG_LEVEL_DEBUG, "Info of Aura - name: " + auraName);
        AuraObjectType type = aura->GetType();
        WorldObject* owner = aura->GetOwner();
        std::string owner_name = owner ? owner->GetName() : "unknown";
        float distance = bot->GetDistance2d(owner);
        Unit* caster = aura->GetCaster();
        std::string caster_name = caster ? caster->GetName() : "unknown";
        bool is_area = aura->IsArea();
        int32 duration = aura->GetDuration();
        const SpellInfo* spellInfo = aura->GetSpellInfo();
        int32 spellId = aura->GetSpellInfo()->Id;
        bool isPositive = aura->GetSpellInfo()->IsPositive();
        sLog->outMessage("playerbot", LOG_LEVEL_DEBUG,
                         "Info of Aura - name: " + auraName + " caster: " + caster_name + " type: " +
                             std::to_string(type) + " owner: " + owner_name + " distance: " + std::to_string(distance) +
                             " isArea: " + std::to_string(is_area) + " duration: " + std::to_string(duration) +
                             " spellId: " + std::to_string(spellId) + " isPositive: " + std::to_string(isPositive));

        botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_aura_info",
            "Info of Aura - name: %name caster: %caster type: %type owner: %owner distance: %distance isArea: %isArea duration: %duration spellId: %spellId isPositive: %isPositive",
            {{"%name", auraName}, {"%caster", caster_name}, {"%type", std::to_string(type)}, {"%owner", owner_name},
             {"%distance", std::to_string(distance)}, {"%isArea", std::to_string(is_area)}, {"%duration", std::to_string(duration)},
             {"%spellId", std::to_string(spellId)}, {"%isPositive", std::to_string(isPositive)}}));

        if (type == DYNOBJ_AURA_TYPE)
        {
            DynamicObject* dyn_owner = aura->GetDynobjOwner();
            float radius = dyn_owner->GetRadius();
            int32 dynSpellId = dyn_owner->GetSpellId();
            int32 dynDuration = dyn_owner->GetDuration();
            sLog->outMessage("playerbot", LOG_LEVEL_DEBUG,
                             std::string("Info of DynamicObject -") + " name: " + dyn_owner->GetName() +
                                 " radius: " + std::to_string(radius) + " spell id: " + std::to_string(dynSpellId) +
                                 " duration: " + std::to_string(dynDuration));

            botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_dynobj_info",
                "Info of DynamicObject - name: %name radius: %radius spell id: %spellId duration: %duration",
                {{"%name", dyn_owner->GetName()}, {"%radius", std::to_string(radius)}, {"%spellId", std::to_string(dynSpellId)}, {"%duration", std::to_string(dynDuration)}}));
        }
    }
    return true;
}

bool TellEstimatedDpsAction::Execute(Event event)
{
    float dps = AI_VALUE(float, "estimated group dps");
    botAI->TellMaster(botAI->GetLocalizedBotTextOrDefault("msg_estimated_group_dps", "Estimated Group DPS: %dps", {{"%dps", std::to_string(dps)}}));
    return true;
}

bool TellCalculateItemAction::Execute(Event event)
{
    std::string const text = event.getParam();
    ItemWithRandomProperty item = chat->parseItemWithRandomProperty(text);
    StatsWeightCalculator calculator(bot);

    const ItemTemplate* proto = sObjectMgr->GetItemTemplate(item.itemId);
    if (!proto)
        return false;
    float score = calculator.CalculateItem(item.itemId, item.randomPropertyId);

    botAI->TellMasterNoFacing(botAI->GetLocalizedBotTextOrDefault("msg_calculated_score", "Calculated score of %item : %score",
        {{"%item", chat->FormatItem(proto)}, {"%score", std::to_string(score)}}));
    return true;
}
