-- Consolidate Chinese (text_loc4) for GetLocalizedBotTextOrDefault keys.
-- 1) Copy zh from obsolete Test-branch alias keys to upstream names.
-- 2) Remove obsolete alias keys (duplicate/wrong names).
-- 3) Fill text_loc4 on existing rows where still empty (keeps fr/de/ko).

UPDATE `ai_playerbot_texts` SET `text_loc4` = '距离太远，不跟随' WHERE `name` = 'area_trigger_follow_too_far_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我已在攻击 %target。' WHERE `name` = 'attack_already_attacking_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%target 对我友好。' WHERE `name` = 'attack_target_friendly_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我没有目标' WHERE `name` = 'attack_no_target_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '飞行中无法攻击' WHERE `name` = 'attack_in_flight_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法攻击无效目标。' WHERE `name` = 'attack_invalid_target_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你没有目标' WHERE `name` = 'attack_no_target_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%target 不在我视线内。' WHERE `name` = 'attack_target_not_in_sight_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%target 已不在世界中。' WHERE `name` = 'attack_target_not_in_world_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '在禁PVP区域无法攻击其他玩家。' WHERE `name` = 'attack_pvp_prohibited_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%target 已死亡。' WHERE `name` = 'attack_target_dead_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我不想加入你的公会 :(' WHERE `name` = 'guild_accept_declined' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我已经在公会中' WHERE `name` = 'guild_accept_already_in_guild' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你不在公会中！' WHERE `name` = 'guild_accept_inviter_not_in_guild' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '附近没有银行职员' WHERE `name` = 'bank_no_banker_nearby_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '附近没有商人' WHERE `name` = 'no_vendors_nearby' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '距离太远，无法和你一起逃跑' WHERE `name` = 'fleeing_far' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '等等我' WHERE `name` = 'area_trigger_wait_for_me' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '自己' WHERE `name` = 'cast_self' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '交易物品' WHERE `name` = 'cast_trade_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '再见！' WHERE `name` = 'goodbye' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你好' WHERE `name` = 'hello' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '嗨！' WHERE `name` = 'hi' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我要登出了！' WHERE `name` = 'logout_start' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已取消登出！' WHERE `name` = 'logout_cancel' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '攻击中' WHERE `name` = 'attacking' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '逃跑中' WHERE `name` = 'fleeing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%name（%threat）' WHERE `name` = 'threat_entry' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '总花费：%cost' WHERE `name` = 'trainer_total_cost' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 可从 %name 学习 ---' WHERE `name` = 'trainer_can_learn' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' - 已学会' WHERE `name` = 'trainer_learned' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' - 太贵' WHERE `name` = 'trainer_too_expensive' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 于自己' WHERE `name` = 'cast_self' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 于 %target' WHERE `name` = 'msg_cast_on' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 于交易物品' WHERE `name` = 'cast_trade_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '使用 %item' WHERE `name` = 'msg_casting' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '使用 %item 于 %target' WHERE `name` = 'msg_cast_on' AND `text_loc4` = '';

DELETE FROM `ai_playerbot_texts_chance` WHERE `name` IN (
    'error_area_too_far',
    'error_attack_already',
    'error_attack_friendly',
    'error_attack_i_no_target',
    'error_attack_in_flight',
    'error_attack_invalid_target',
    'error_attack_no_target',
    'error_attack_not_in_sight',
    'error_attack_not_in_world',
    'error_attack_pvp_area',
    'error_attack_target_dead',
    'error_cant_fly_with_you',
    'error_creature_template_not_found',
    'error_discount_buy_only',
    'error_failed_create_pet',
    'error_guild_accept_dont_want',
    'error_guild_accept_in_guild',
    'error_guild_accept_not_in_guild',
    'error_hearthstone_not_ready_master',
    'error_hearthstone_not_ready_self',
    'error_hunter_10_for_pet',
    'error_in_combat',
    'error_invalid_tame_id',
    'error_no_banker_nearby',
    'error_no_event_owner',
    'error_no_exotic_without_bm',
    'error_no_flightmaster',
    'error_no_hearthstone_master',
    'error_no_hearthstone_self',
    'error_no_hunter_pet_abandon',
    'error_no_innkeeper_around',
    'error_no_innkeepers_near_you',
    'error_no_innkeepers_nearby',
    'error_no_items_available',
    'error_no_items_to_trade',
    'error_no_meeting_stone_near_you',
    'error_no_meeting_stone_nearby',
    'error_no_pet_guardian',
    'error_no_pet_to_rename',
    'error_no_spells_from_trainer'
);

DELETE FROM `ai_playerbot_texts` WHERE `name` IN (
    'error_area_too_far',
    'error_attack_already',
    'error_attack_friendly',
    'error_attack_i_no_target',
    'error_attack_in_flight',
    'error_attack_invalid_target',
    'error_attack_no_target',
    'error_attack_not_in_sight',
    'error_attack_not_in_world',
    'error_attack_pvp_area',
    'error_attack_target_dead',
    'error_cant_fly_with_you',
    'error_creature_template_not_found',
    'error_discount_buy_only',
    'error_failed_create_pet',
    'error_guild_accept_dont_want',
    'error_guild_accept_in_guild',
    'error_guild_accept_not_in_guild',
    'error_hearthstone_not_ready_master',
    'error_hearthstone_not_ready_self',
    'error_hunter_10_for_pet',
    'error_in_combat',
    'error_invalid_tame_id',
    'error_no_banker_nearby',
    'error_no_event_owner',
    'error_no_exotic_without_bm',
    'error_no_flightmaster',
    'error_no_hearthstone_master',
    'error_no_hearthstone_self',
    'error_no_hunter_pet_abandon',
    'error_no_innkeeper_around',
    'error_no_innkeepers_near_you',
    'error_no_innkeepers_nearby',
    'error_no_items_available',
    'error_no_items_to_trade',
    'error_no_meeting_stone_near_you',
    'error_no_meeting_stone_nearby',
    'error_no_pet_guardian',
    'error_no_pet_to_rename',
    'error_no_spells_from_trainer'
);

DELETE FROM `ai_playerbot_texts_chance` WHERE `name` IN (
    'error_no_tameable_by_family',
    'error_no_tameable_by_id',
    'error_no_tameable_by_name',
    'error_no_vendors_nearby',
    'error_not_enough_money',
    'error_not_enough_place_summon',
    'error_pet_name_alpha_only',
    'error_pet_name_forbidden',
    'error_pet_name_length',
    'error_quest_cant_take',
    'error_quest_have',
    'error_shortcut_flee_far',
    'error_socket_does_not_fit',
    'error_summon_bot_dead',
    'error_summon_master_combat',
    'error_summon_master_dead',
    'error_summon_on_vehicle',
    'error_tame_usage',
    'error_wait_for_me',
    'msg_ai_reset',
    'msg_already_spirit',
    'msg_busy_now',
    'msg_buying_disabled',
    'msg_cannot_craft_this',
    'msg_cannot_send_item',
    'msg_cannot_send_money',
    'msg_cast_self',
    'msg_cast_trade_item',
    'msg_clean_quest_log',
    'msg_consumable_use',
    'msg_craft_fee',
    'msg_craft_usage',
    'msg_dont_need_this',
    'msg_drinking',
    'msg_eating',
    'msg_equipping_outfit',
    'msg_fair_trade',
    'msg_feasting',
    'msg_goodbye',
    'msg_hello'
);

DELETE FROM `ai_playerbot_texts` WHERE `name` IN (
    'error_no_tameable_by_family',
    'error_no_tameable_by_id',
    'error_no_tameable_by_name',
    'error_no_vendors_nearby',
    'error_not_enough_money',
    'error_not_enough_place_summon',
    'error_pet_name_alpha_only',
    'error_pet_name_forbidden',
    'error_pet_name_length',
    'error_quest_cant_take',
    'error_quest_have',
    'error_shortcut_flee_far',
    'error_socket_does_not_fit',
    'error_summon_bot_dead',
    'error_summon_master_combat',
    'error_summon_master_dead',
    'error_summon_on_vehicle',
    'error_tame_usage',
    'error_wait_for_me',
    'msg_ai_reset',
    'msg_already_spirit',
    'msg_busy_now',
    'msg_buying_disabled',
    'msg_cannot_craft_this',
    'msg_cannot_send_item',
    'msg_cannot_send_money',
    'msg_cast_self',
    'msg_cast_trade_item',
    'msg_clean_quest_log',
    'msg_consumable_use',
    'msg_craft_fee',
    'msg_craft_usage',
    'msg_dont_need_this',
    'msg_drinking',
    'msg_eating',
    'msg_equipping_outfit',
    'msg_fair_trade',
    'msg_feasting',
    'msg_goodbye',
    'msg_hello'
);

DELETE FROM `ai_playerbot_texts_chance` WHERE `name` IN (
    'msg_hello_follow',
    'msg_hello_talk',
    'msg_hi',
    'msg_i_want_for_this',
    'msg_inn_new_home',
    'msg_item_not_for_sale',
    'msg_item_not_for_sale_short',
    'msg_logging_out',
    'msg_logout_cancelled',
    'msg_meet_at_graveyard',
    'msg_no_mailbox_nearby',
    'msg_not_dead_wait',
    'msg_off_with_you',
    'msg_one_item_only',
    'msg_outfit_set_as',
    'msg_outfit_usage_add',
    'msg_outfit_usage_equip',
    'msg_outfit_usage_remove',
    'msg_pet_abandoned',
    'msg_pet_changed',
    'msg_pet_changed_to',
    'msg_pet_rename_dismiss_recall',
    'msg_pet_renamed',
    'msg_pleasure_business',
    'msg_quest_accepted',
    'msg_quest_cannot_accept',
    'msg_quest_dropped',
    'msg_quest_not_trivial_kept',
    'msg_quest_removed',
    'msg_quest_removed_trivial',
    'msg_quest_rewarded',
    'msg_quest_shared',
    'msg_quest_will_remove_trivial',
    'msg_range_default',
    'msg_ready_for_flight',
    'msg_releasing',
    'msg_replacing_equip_outfit',
    'msg_reset_instances',
    'msg_resetting_outfit',
    'msg_rpg_start_trade_with'
);

DELETE FROM `ai_playerbot_texts` WHERE `name` IN (
    'msg_hello_follow',
    'msg_hello_talk',
    'msg_hi',
    'msg_i_want_for_this',
    'msg_inn_new_home',
    'msg_item_not_for_sale',
    'msg_item_not_for_sale_short',
    'msg_logging_out',
    'msg_logout_cancelled',
    'msg_meet_at_graveyard',
    'msg_no_mailbox_nearby',
    'msg_not_dead_wait',
    'msg_off_with_you',
    'msg_one_item_only',
    'msg_outfit_set_as',
    'msg_outfit_usage_add',
    'msg_outfit_usage_equip',
    'msg_outfit_usage_remove',
    'msg_pet_abandoned',
    'msg_pet_changed',
    'msg_pet_changed_to',
    'msg_pet_rename_dismiss_recall',
    'msg_pet_renamed',
    'msg_pleasure_business',
    'msg_quest_accepted',
    'msg_quest_cannot_accept',
    'msg_quest_dropped',
    'msg_quest_not_trivial_kept',
    'msg_quest_removed',
    'msg_quest_removed_trivial',
    'msg_quest_rewarded',
    'msg_quest_shared',
    'msg_quest_will_remove_trivial',
    'msg_range_default',
    'msg_ready_for_flight',
    'msg_releasing',
    'msg_replacing_equip_outfit',
    'msg_reset_instances',
    'msg_resetting_outfit',
    'msg_rpg_start_trade_with'
);

DELETE FROM `ai_playerbot_texts_chance` WHERE `name` IN (
    'msg_rpg_you_can_use_this',
    'msg_say_quest_accepted',
    'msg_say_quest_removed',
    'msg_selling_disabled',
    'msg_sending_mail_to',
    'msg_sent_mail_to',
    'msg_shortcut_attacking',
    'msg_shortcut_fleeing',
    'msg_shortcut_following',
    'msg_shortcut_grinding',
    'msg_shortcut_moving_away',
    'msg_shortcut_running',
    'msg_shortcut_staying',
    'msg_thank_you_trader',
    'msg_thanks',
    'msg_threat_entry',
    'msg_total_cost',
    'msg_trading_disabled',
    'msg_trainer_can_learn',
    'msg_trainer_learned',
    'msg_trainer_too_expensive',
    'msg_updating_outfit',
    'msg_use_on_self',
    'msg_use_on_target',
    'msg_use_on_traded_item',
    'msg_welcome',
    'msg_will_craft_using_reagents',
    'msg_will_loot',
    'msg_will_not_craft',
    'msg_wont_loot',
    'use_command_go',
    'use_command_on'
);

DELETE FROM `ai_playerbot_texts` WHERE `name` IN (
    'msg_rpg_you_can_use_this',
    'msg_say_quest_accepted',
    'msg_say_quest_removed',
    'msg_selling_disabled',
    'msg_sending_mail_to',
    'msg_sent_mail_to',
    'msg_shortcut_attacking',
    'msg_shortcut_fleeing',
    'msg_shortcut_following',
    'msg_shortcut_grinding',
    'msg_shortcut_moving_away',
    'msg_shortcut_running',
    'msg_shortcut_staying',
    'msg_thank_you_trader',
    'msg_thanks',
    'msg_threat_entry',
    'msg_total_cost',
    'msg_trading_disabled',
    'msg_trainer_can_learn',
    'msg_trainer_learned',
    'msg_trainer_too_expensive',
    'msg_updating_outfit',
    'msg_use_on_self',
    'msg_use_on_target',
    'msg_use_on_traded_item',
    'msg_welcome',
    'msg_will_craft_using_reagents',
    'msg_will_loot',
    'msg_will_not_craft',
    'msg_wont_loot',
    'use_command_go',
    'use_command_on'
);

-- Fill remaining empty text_loc4 from whisper + upstream gaps
UPDATE `ai_playerbot_texts` SET `text_loc4` = '距离太远，不跟随' WHERE `name` = 'area_trigger_follow_too_far_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '等等我' WHERE `name` = 'area_trigger_wait_for_me' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我已在攻击 %target。' WHERE `name` = 'attack_already_attacking_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '飞行中无法攻击' WHERE `name` = 'attack_in_flight_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法攻击无效目标。' WHERE `name` = 'attack_invalid_target_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我没有目标' WHERE `name` = 'attack_no_target_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '在禁PVP区域无法攻击其他玩家。' WHERE `name` = 'attack_pvp_prohibited_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%target 已死亡。' WHERE `name` = 'attack_target_dead_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%target 对我友好。' WHERE `name` = 'attack_target_friendly_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%target 不在我视线内。' WHERE `name` = 'attack_target_not_in_sight_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%target 已不在世界中。' WHERE `name` = 'attack_target_not_in_world_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '攻击中' WHERE `name` = 'attacking' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '附近没有银行职员' WHERE `name` = 'bank_no_banker_nearby_error' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '自己' WHERE `name` = 'cast_self' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '交易物品' WHERE `name` = 'cast_trade_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '该挂会儿机了……' WHERE `name` = 'emote_afk_break' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '不代表你要这么混蛋。' WHERE `name` = 'emote_ass_about_it' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '唉……' WHERE `name` = 'emote_awwwww' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '对……你……后排那位……' WHERE `name` = 'emote_back_of_class' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '随便求，我啥也不给。' WHERE `name` = 'emote_beg_nothing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '巴拉巴拉……' WHERE `name` = 'emote_blah_blah' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '啥？休息结束了？行吧……' WHERE `name` = 'emote_break_over' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已经休息时间了？' WHERE `name` = 'emote_break_time' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '啥？！我也会！' WHERE `name` = 'emote_can_do_too' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '看谁先怂！' WHERE `name` = 'emote_chicken' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '开扁时间到！' WHERE `name` = 'emote_clobbering' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我干啥了要受这个？' WHERE `name` = 'emote_deserve_that' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '是我干的吗？' WHERE `name` = 'emote_did_i_do_that' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '那我到底该咋办？' WHERE `name` = 'emote_do_about_that' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '别杀我！' WHERE `name` = 'emote_dont_kill_me' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你去哪儿我就跟到哪儿……' WHERE `name` = 'emote_follow_you' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '噫！把你的细菌拿远点！' WHERE `name` = 'emote_germs' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '谁是好狗狗？你是好狗狗！' WHERE `name` = 'emote_good_doggy' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '哈呵……真搞笑……' WHERE `name` = 'emote_har_har' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '大力神！大力神！' WHERE `name` = 'emote_hercules' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '谢谢……谢谢……我整周都在。' WHERE `name` = 'emote_here_all_week' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '嗨！' WHERE `name` = 'emote_hey_there' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你伤我心了……' WHERE `name` = 'emote_hurt_feelings' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '是是是，我知道我很厉害。' WHERE `name` = 'emote_im_amazing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我的工作不包括逗你玩。' WHERE `name` = 'emote_job_description' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '别看我……我只是在这干活。' WHERE `name` = 'emote_just_work_here' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '收敛点，老板……' WHERE `name` = 'emote_keep_pants' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '呃，我不该笑这么早？' WHERE `name` = 'emote_laugh_soon' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '等等……我们在笑啥来着？' WHERE `name` = 'emote_laughing_at' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '医护！快！' WHERE `name` = 'emote_medic' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '现在可不是慌的时候！' WHERE `name` = 'emote_not_panic' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '哎哟！疼死了！' WHERE `name` = 'emote_ouch' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '不客气！' WHERE `name` = 'emote_quite_welcome' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '嗷！' WHERE `name` = 'emote_rawr' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我这边也好了！' WHERE `name` = 'emote_ready_here' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '原样奉还，伙计！' WHERE `name` = 'emote_right_back' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '扭起来！' WHERE `name` = 'emote_shake_what' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你自己走！' WHERE `name` = 'emote_shoo_yourself' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '至少有人同意我！' WHERE `name` = 'emote_someone_agrees' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '啥？你眼里进东西了？' WHERE `name` = 'emote_something_eye' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你别哭，不然我也要哭了！' WHERE `name` = 'emote_start_crying' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '行……我就待在这儿……' WHERE `name` = 'emote_stay_here' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '嘿！别弄了！' WHERE `name` = 'emote_stop_that' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '谢了……' WHERE `name` = 'emote_thanks_dots' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '谢了……我会需要的……' WHERE `name` = 'emote_thanks_need_it' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '啥？你想来点？' WHERE `name` = 'emote_want_some' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '不是我！随便说说……' WHERE `name` = 'emote_wasnt_me' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '呃……等等！你去哪儿？！' WHERE `name` = 'emote_where_going' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '希望你带够了全班份……' WHERE `name` = 'emote_whole_class' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '唉……为啥不行？！' WHERE `name` = 'emote_why_not' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '耶！' WHERE `name` = 'emote_yay' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你……' WHERE `name` = 'emote_you_dots' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你当然该道歉！' WHERE `name` = 'emote_youre_sorry' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你不能对替补机器人使用自动换装。' WHERE `name` = 'error_autogear_no_alt' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '不允许自动换装命令，请检查配置。' WHERE `name` = 'error_autogear_not_allowed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法施放 %spell' WHERE `name` = 'error_cannot_cast' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法从 %name 获得 %item，背包已满' WHERE `name` = 'error_cannot_get_bags_full' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法与任务NPC对话' WHERE `name` = 'error_cannot_talk_quest_giver' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法解锁该物品。' WHERE `name` = 'error_cannot_unlock_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我去不了那里' WHERE `name` = 'error_cant_go_there' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '施放 %spell 失败' WHERE `name` = 'error_cast_failed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无效的分散距离 %value' WHERE `name` = 'error_disperse_invalid' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '施放开锁失败。' WHERE `name` = 'error_failed_cast_pick_lock' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '用法：flag cloak/helm/pvp on/set/off/clear/toggle/?' WHERE `name` = 'error_flag_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无效阵型：%formation' WHERE `name` = 'error_formation_invalid' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '没有足够的空插槽来装备所有雕文。' WHERE `name` = 'error_glyph_no_sockets' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我需要先对话' WHERE `name` = 'error_gossip_need_talk' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未知的对话选项' WHERE `name` = 'error_gossip_unknown' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法将 %item 放入公会银行，我没有第一页的存入权限' WHERE `name` = 'error_guild_cant_put' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我在公会里很开心 :)' WHERE `name` = 'error_guild_happy' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你还不能雇佣我，我们不够熟。请确保至少有 %money 作为交易折扣' WHERE `name` = 'error_hire_cannot' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '不能雇佣等级高于你的角色' WHERE `name` = 'error_hire_level' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你的角色数量已达上限' WHERE `name` = 'error_hire_max_chars' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '目标在水中' WHERE `name` = 'error_in_water' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无效任务 %text' WHERE `name` = 'error_invalid_quest_whisper' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '开锁技能不足（%current/%required）无法解锁：%item' WHERE `name` = 'error_lockpick_skill_too_low' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '只有在你公会里我才会用这种拾取方式 :/' WHERE `name` = 'error_loot_guild_only' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%action：我不会这个操作' WHERE `name` = 'error_mail_unknown_action' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '不允许维护命令，请检查配置。' WHERE `name` = 'error_maintenance_not_allowed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '附近没有公会银行' WHERE `name` = 'error_no_guild_bank_nearby' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '“禁止交易”栏位没有物品。' WHERE `name` = 'error_no_item_do_not_trade_slot' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '附近没有邮箱' WHERE `name` = 'error_no_mailbox_nearby' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '没有宠物！' WHERE `name` = 'error_no_pet' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '找不到可传送的传送门' WHERE `name` = 'error_no_portal_teleport' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '附近没有可修理的NPC' WHERE `name` = 'error_no_repair_npc' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '附近没人卖 %item' WHERE `name` = 'error_nobody_sells' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '背包空间不足' WHERE `name` = 'error_not_enough_bag_space' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我不在你的公会里！' WHERE `name` = 'error_not_in_guild' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我不是灵魂状态' WHERE `name` = 'error_not_spirit' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无处可逃' WHERE `name` = 'error_nowhere_flee' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '弹药不足！' WHERE `name` = 'error_out_of_ammo' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '所有路径不在视线内' WHERE `name` = 'error_paths_not_los' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '宠物不开心！' WHERE `name` = 'error_pet_unhappy' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我已经在公会中' WHERE `name` = 'error_petition_in_guild' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我已经在这样的队伍中' WHERE `name` = 'error_petition_in_team' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我已经被邀请加入公会' WHERE `name` = 'error_petition_invited' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我已经签署了这份请愿书' WHERE `name` = 'error_petition_signed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法解锁，缺少开锁法术。' WHERE `name` = 'error_pick_lock_spell_missing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未找到任务模板。' WHERE `name` = 'error_quest_template_not_found' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我看不到我的RTI攻击目标' WHERE `name` = 'error_rti_no_target' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '用法：s gray/*/vendor/[物品链接]' WHERE `name` = 'error_sell_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无效阵型：%stance' WHERE `name` = 'error_stance_invalid' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '除拾取和采集外，其他策略均可修改' WHERE `name` = 'error_strategy_except' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '逃跑时卡住了' WHERE `name` = 'error_stuck_fleeing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '跟随时卡住了' WHERE `name` = 'error_stuck_following' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '太远了' WHERE `name` = 'error_too_far_away' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未知法术 %text' WHERE `name` = 'error_unknown_spell' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '逃跑中' WHERE `name` = 'fleeing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '距离太远，无法和你一起逃跑' WHERE `name` = 'fleeing_far' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '再见！' WHERE `name` = 'goodbye' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我已经在公会中' WHERE `name` = 'guild_accept_already_in_guild' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '抱歉，我不想加入你的公会 :(' WHERE `name` = 'guild_accept_declined' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你不在公会中！' WHERE `name` = 'guild_accept_inviter_not_in_guild' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你好' WHERE `name` = 'hello' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '嗨！' WHERE `name` = 'hi' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已取消登出！' WHERE `name` = 'logout_cancel' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我要登出了！' WHERE `name` = 'logout_start' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%name：失败' WHERE `name` = 'msg_action_failed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%name：不可能' WHERE `name` = 'msg_action_impossible' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%name：未知指令' WHERE `name` = 'msg_action_unknown' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%name：没用' WHERE `name` = 'msg_action_useless' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已添加 BWL 策略！' WHERE `name` = 'msg_add_bwl_strategies' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已添加 NAXX 策略！' WHERE `name` = 'msg_add_naxx_strategies' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已添加副本策略 %strategy' WHERE `name` = 'msg_added_instance_strategy' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = 'Aedm 法术已训练。' WHERE `name` = 'msg_aedm_trained' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '始终拾取：%list' WHERE `name` = 'msg_always_loot_items' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%num.%name' WHERE `name` = 'msg_attacker_line' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 攻击者 ---' WHERE `name` = 'msg_attackers_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在攻击 %target' WHERE `name` = 'msg_attacking_target' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '光环信息 - 名称: %name 施法者: %caster 类型: %type 所有者: %owner 距离: %distance 范围: %isArea 持续: %duration 法术ID: %spellId 有益: %isPositive' WHERE `name` = 'msg_aura_info' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 光环 ---' WHERE `name` = 'msg_auras_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我在自动换装' WHERE `name` = 'msg_auto_gearing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '可驯服宠物种族：%list' WHERE `name` = 'msg_available_pet_families' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '光环' WHERE `name` = 'msg_avoid_type_aura' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '陷阱' WHERE `name` = 'msg_avoid_type_trap' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '单位触发' WHERE `name` = 'msg_avoid_type_unit_trigger' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我在躲 %name（%id）范围 %radius - [%type]' WHERE `name` = 'msg_avoiding_aoe' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== 银行 ===' WHERE `name` = 'msg_bank_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 药剂 ---' WHERE `name` = 'msg_buff_elixir' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 附魔 ---' WHERE `name` = 'msg_buff_enchant' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 合剂 ---' WHERE `name` = 'msg_buff_flask' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 食物 ---' WHERE `name` = 'msg_buff_food' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 卷轴 ---' WHERE `name` = 'msg_buff_scroll' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '购买 %item' WHERE `name` = 'msg_buying_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%item 计算得分：%score' WHERE `name` = 'msg_calculated_score' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我无法解锁该物品。' WHERE `name` = 'msg_cant_unlock_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 对 ' WHERE `name` = 'msg_cast_on' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在施放 %spell' WHERE `name` = 'msg_casting' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '（剩余 %n 次）' WHERE `name` = 'msg_casting_left' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '频道已设为 %chat' WHERE `name` = 'msg_chat_set_to' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '选择奖励：%list' WHERE `name` = 'msg_choose_reward' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已清除行进目标' WHERE `name` = 'msg_clearing_travel_target' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '#%idx %command' WHERE `name` = 'msg_cs_line' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== %name ===' WHERE `name` = 'msg_cs_section' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '当前频道 %chat' WHERE `name` = 'msg_current_chat' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== 自定义策略 ===' WHERE `name` = 'msg_custom_strategies_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '用法：cs <名称> <索引> <命令>' WHERE `name` = 'msg_custom_strategy_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '机器人：%name，' WHERE `name` = 'msg_debug_bot' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '施法中 %spell' WHERE `name` = 'msg_debug_casting_spell' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '施放载具法术 %spell' WHERE `name` = 'msg_debug_casting_vehicle_spell' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '错误码：%code（0x%hex），' WHERE `name` = 'msg_debug_error_code' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%quest_link - %item_link %quest_obj_available/%quest_obj_required' WHERE `name` = 'msg_debug_quest_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '施法失败 - ' WHERE `name` = 'msg_debug_spell_cast_failed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '法术ID：%id（%spell），' WHERE `name` = 'msg_debug_spell_id' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '目标：游戏对象（低GUID：%low，高GUID：%high），' WHERE `name` = 'msg_debug_target_go' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '目标：物品（低GUID：%low，高GUID：%high），' WHERE `name` = 'msg_debug_target_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '目标：单位（%name，低GUID：%low，高GUID：%high），' WHERE `name` = 'msg_debug_target_unit' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '交易模式：进行中，' WHERE `name` = 'msg_debug_trade_active' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '交易模式：未激活，' WHERE `name` = 'msg_debug_trade_inactive' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '交易物品：%entry（低GUID：%low，高GUID：%high），' WHERE `name` = 'msg_debug_trade_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '交易物品：无，' WHERE `name` = 'msg_debug_trade_item_none' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未找到目的地 %dest。' WHERE `name` = 'msg_dest_not_found' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '折扣最多：%money' WHERE `name` = 'msg_discount_up_to' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '（当前分散距离：%value）' WHERE `name` = 'msg_disperse_current' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已关闭分散' WHERE `name` = 'msg_disperse_disable' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '请先启用分散' WHERE `name` = 'msg_disperse_enable_first' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已启用分散距离 %value' WHERE `name` = 'msg_disperse_enabled' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '分散距离已增至 %value' WHERE `name` = 'msg_disperse_increased' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '分散距离已设为 %value' WHERE `name` = 'msg_disperse_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '用法：disperse [enable|disable|increase|decrease|set 距离]' WHERE `name` = 'msg_disperse_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '动态物体信息 - 名称: %name 半径: %radius 法术ID: %spellId 持续: %duration' WHERE `name` = 'msg_dynobj_info' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== 纹章 ===' WHERE `name` = 'msg_emblems_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '装备 %item' WHERE `name` = 'msg_equipping' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '装备 %item' WHERE `name` = 'msg_equipping_generic' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '装备 %item 到主手' WHERE `name` = 'msg_equipping_mainhand' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '装备 %item 到副手' WHERE `name` = 'msg_equipping_offhand' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '装备 %item 到远程栏位' WHERE `name` = 'msg_equipping_ranged' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '预估小队 DPS：%dps' WHERE `name` = 'msg_estimated_group_dps' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%type 状态：%value' WHERE `name` = 'msg_flag_is' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我快到了，等我！' WHERE `name` = 'msg_follow_close' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在前往你的位置。' WHERE `name` = 'msg_follow_heading' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在赶往你的位置。' WHERE `name` = 'msg_follow_traveling' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '当前阵型：%name' WHERE `name` = 'msg_formation_current' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '阵型已设为：%formation' WHERE `name` = 'msg_formation_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '请设置为：|cffffffff chaos（默认）、near、queue、circle、line、shield、arrow、melee、far' WHERE `name` = 'msg_formation_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '用法：glyph equip <6个雕文物品ID>（3个主要，3个次要）。' WHERE `name` = 'msg_glyph_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '雕文：%list' WHERE `name` = 'msg_glyphs' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '雕文已更新。' WHERE `name` = 'msg_glyphs_updated' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '，长度 ' WHERE `name` = 'msg_go_of_length' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '路径类型：' WHERE `name` = 'msg_go_path_is' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '私聊「go x,y」「go [游戏对象]」「go 单位名」或「go 位置名」我会过去' WHERE `name` = 'msg_go_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '，偏移 ' WHERE `name` = 'msg_go_with_offset' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '从 %name 获得 %item' WHERE `name` = 'msg_got_from' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '从银行取出 %item' WHERE `name` = 'msg_got_from_bank' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '获得任务 %quest' WHERE `name` = 'msg_got_quest' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '前往：%target' WHERE `name` = 'msg_heading_to' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '可用策略（co/nc/dead 命令）：%list' WHERE `name` = 'msg_help_strategies' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '可私聊：%list，或 [物品][任务][对象] 链接' WHERE `name` = 'msg_help_whisper_any' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '下次重登时我会加入你' WHERE `name` = 'msg_hire_will_join' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我在 %x,%y' WHERE `name` = 'msg_i_am_at' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 已忽略（离主人太远）。' WHERE `name` = 'msg_ignored_far_from_master' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '忽略法术列表：%list' WHERE `name` = 'msg_ignored_spell_list' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '忽略法术列表为空' WHERE `name` = 'msg_ignored_spell_list_empty' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 护甲 ---' WHERE `name` = 'msg_inv_armor' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 消耗品 ---' WHERE `name` = 'msg_inv_consumable' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 容器 ---' WHERE `name` = 'msg_inv_container' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 钥匙 ---' WHERE `name` = 'msg_inv_keys' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 其他 ---' WHERE `name` = 'msg_inv_other' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 弹药 ---' WHERE `name` = 'msg_inv_projectile' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 任务物品 ---' WHERE `name` = 'msg_inv_quest_items' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 箭袋 ---' WHERE `name` = 'msg_inv_quiver' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 材料 ---' WHERE `name` = 'msg_inv_reagent' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 配方 ---' WHERE `name` = 'msg_inv_recipe' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 商品 ---' WHERE `name` = 'msg_inv_trade_goods' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 武器 ---' WHERE `name` = 'msg_inv_weapon' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== 背包 ===' WHERE `name` = 'msg_inventory_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%item 已销毁' WHERE `name` = 'msg_item_destroyed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%item 已作为奖励' WHERE `name` = 'msg_item_rewarded' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '以 %role 加入。' WHERE `name` = 'msg_joining_as' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '以 %role 加入，还剩 %spots 个 %role 位。' WHERE `name` = 'msg_joining_as_spots' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已学法术：' WHERE `name` = 'msg_learned_spells' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我又活了！' WHERE `name` = 'msg_live_again' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '当前日志等级 %level' WHERE `name` = 'msg_log_level_is' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '日志等级已设为 %level' WHERE `name` = 'msg_log_level_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已加入常拾取列表' WHERE `name` = 'msg_loot_added_to_always' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已从常拾取列表中移除' WHERE `name` = 'msg_loot_removed_from_always' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '拾取策略：%name' WHERE `name` = 'msg_loot_strategy' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '拾取策略已设为 %name' WHERE `name` = 'msg_loot_strategy_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 尸体 ---' WHERE `name` = 'msg_los_corpses' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 友好玩家 ---' WHERE `name` = 'msg_los_friendly_players' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 游戏对象 ---' WHERE `name` = 'msg_los_game_objects' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- NPC ---' WHERE `name` = 'msg_los_npcs' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 目标 ---' WHERE `name` = 'msg_los_targets' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 目标（全部）---' WHERE `name` = 'msg_los_targets_all' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 触发器 ---' WHERE `name` = 'msg_los_triggers' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%n 天' WHERE `name` = 'msg_mail_days' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%subject 已删除' WHERE `name` = 'msg_mail_deleted' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%subject，%detail 已处理' WHERE `name` = 'msg_mail_processed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== 邮箱 ===' WHERE `name` = 'msg_mailbox_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '主手已升级，将 %item 移至副手' WHERE `name` = 'msg_mainhand_move_offhand' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我在维护中' WHERE `name` = 'msg_maintaining' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '省蓝等级：%value' WHERE `name` = 'msg_mana_save_level' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '省蓝等级已设为：%value' WHERE `name` = 'msg_mana_save_level_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '最多等待 20 秒！' WHERE `name` = 'msg_max_wait_20s' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在前往 %x,%y' WHERE `name` = 'msg_moving_to_coords' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在前往 %target' WHERE `name` = 'msg_moving_to_go' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在前往位置 %name' WHERE `name` = 'msg_moving_to_position' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在前往 %name' WHERE `name` = 'msg_moving_to_unit' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未装备雕文' WHERE `name` = 'msg_no_glyphs_equipped' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未找到法术。' WHERE `name` = 'msg_no_spells_found' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '该节点无法移除。' WHERE `name` = 'msg_node_cannot_remove' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '节点 %name 已创建。' WHERE `name` = 'msg_node_created' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '节点已移除。' WHERE `name` = 'msg_node_removed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '没有可去之处，先待命。' WHERE `name` = 'msg_nowhere_travel_idle' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已打开物品：%item' WHERE `name` = 'msg_opened_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '|cffff0000|关闭宠物自动施法：%spell' WHERE `name` = 'msg_pet_autocast_disabling' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '|cFF00ff00|开启宠物自动施法：%spell' WHERE `name` = 'msg_pet_autocast_enabling' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '宠物自动施法已切换。' WHERE `name` = 'msg_pet_autocast_toggled' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '宠物姿态已设置为 %stance（已应用到所有宠物/守护）。' WHERE `name` = 'msg_pet_stance_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在对交易物品开锁：%item' WHERE `name` = 'msg_picking_lock_on_traded' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正与 %name 游玩' WHERE `name` = 'msg_playing_with' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '请快点 %name' WHERE `name` = 'msg_please_hurry' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '位置 %name 已重置' WHERE `name` = 'msg_position_is_reset' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '位置 %name 已设置' WHERE `name` = 'msg_position_is_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '位置 %name 未设置' WHERE `name` = 'msg_position_not_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '位置 %name 未设置' WHERE `name` = 'msg_position_not_set_qual' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '位置 %name 已设为 %x,%y' WHERE `name` = 'msg_position_set_to' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '私聊 position <名称> ?/set/reset' WHERE `name` = 'msg_position_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%item 已存入银行' WHERE `name` = 'msg_put_to_bank' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%item 已放入公会银行' WHERE `name` = 'msg_put_to_guild_bank' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已接取 %quest' WHERE `name` = 'msg_quest_accepted_short' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '[进行中] 前往 %title，距离 %dist' WHERE `name` = 'msg_quest_active_travel' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已完成' WHERE `name` = 'msg_quest_already_completed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已接取' WHERE `name` = 'msg_quest_already_on' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '背包已满' WHERE `name` = 'msg_quest_bags_full' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法接取 %quest' WHERE `name` = 'msg_quest_cannot_accept_quest' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法接取' WHERE `name` = 'msg_quest_cant_take' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务完成 %quest' WHERE `name` = 'msg_quest_completed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务完成 %quest' WHERE `name` = 'msg_quest_completed_tell' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 已完成任务 ---' WHERE `name` = 'msg_quest_completed_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务：%quest 确认接取' WHERE `name` = 'msg_quest_confirm_accept' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%n 个拥挤。' WHERE `name` = 'msg_quest_crowded' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 拥挤' WHERE `name` = 'msg_quest_crowded_suffix' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%dist 到 %title %points 点。%extra' WHERE `name` = 'msg_quest_dest_line' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%avail%/%tot% 个目标 %ap%/%tp% 点。%crowded%%range%' WHERE `name` = 'msg_quest_dest_summary' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务 %quest 已超时，放弃' WHERE `name` = 'msg_quest_failed_timer' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 未完成任务 ---' WHERE `name` = 'msg_quest_incompleted_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%item %current/%required %quest' WHERE `name` = 'msg_quest_item_progress' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务日志已满' WHERE `name` = 'msg_quest_log_full' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 未激活' WHERE `name` = 'msg_quest_not_active' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未找到任务 %id。' WHERE `name` = 'msg_quest_not_found' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%n 个超出范围。' WHERE `name` = 'msg_quest_out_of_range' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%name %current/%required %quest' WHERE `name` = 'msg_quest_progress' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '|c0000FF00已完成|r ---' WHERE `name` = 'msg_quest_query_completed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 未激活' WHERE `name` = 'msg_quest_query_not_active' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '|c00FF0000未完成|r ---' WHERE `name` = 'msg_quest_query_not_completed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '等待选择奖励' WHERE `name` = 'msg_quest_reward_pending' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已选奖励 %item' WHERE `name` = 'msg_quest_rewarded_short' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务 |cff00ff00可接|r：%quest' WHERE `name` = 'msg_quest_status_available' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务 |cffff0000失败|r：%quest' WHERE `name` = 'msg_quest_status_failed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务 |cffff0000未完成|r：%quest' WHERE `name` = 'msg_quest_status_incomplete' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务 |cffff0000已领奖|r：%quest' WHERE `name` = 'msg_quest_status_rewarded' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 摘要 ---' WHERE `name` = 'msg_quest_summary_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '共 %total/25 个任务（未完成：%incomplete，已完成：%complete）' WHERE `name` = 'msg_quest_total' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '找不到合适奖励，请求帮助……' WHERE `name` = 'msg_quest_unable_reward' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%type 范围：%value' WHERE `name` = 'msg_range_is' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%type 范围：%value（默认）' WHERE `name` = 'msg_range_is_default' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%qualifier 范围已设为 %value' WHERE `name` = 'msg_range_set_to' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '食' WHERE `name` = 'msg_ready_food' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '血' WHERE `name` = 'msg_ready_hp' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '蓝' WHERE `name` = 'msg_ready_mp' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '水' WHERE `name` = 'msg_ready_water' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '修理：%cost（%npc）' WHERE `name` = 'msg_repair_cost' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在走位脱离视线目标！' WHERE `name` = 'msg_reposition_los' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== 声望 ===' WHERE `name` = 'msg_reputations_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '钓一会儿。' WHERE `name` = 'msg_reveal_fish' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '去看看。' WHERE `name` = 'msg_reveal_look' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '要靠近点吗？' WHERE `name` = 'msg_reveal_nearer' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我看到 %obj。' WHERE `name` = 'msg_reveal_see' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '------%target------' WHERE `name` = 'msg_rpg_target_header' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%type 设为：' WHERE `name` = 'msg_rti_set_to' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = 'RTS 控制已启用。' WHERE `name` = 'msg_rts_control_enabled' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = 'RTS 控制法术已移除。' WHERE `name` = 'msg_rts_spell_removed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已保存：%list。' WHERE `name` = 'msg_rtsc_saved' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务 [ %quest ] 已完成' WHERE `name` = 'msg_say_quest_completed' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '出售 %item 价格 %money' WHERE `name` = 'msg_selling_for' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在出售 %item' WHERE `name` = 'msg_selling_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '全力输出！' WHERE `name` = 'msg_shortcut_max_dps' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你离我太远了！我马上到。' WHERE `name` = 'msg_shortcut_too_far' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' (已绑定)' WHERE `name` = 'msg_soulbound' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%spell 已加入忽略列表' WHERE `name` = 'msg_spell_added_to_ignored' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法在此施法' WHERE `name` = 'msg_spell_failed_area' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无法施放' WHERE `name` = 'msg_spell_failed_default' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '需要法术焦点' WHERE `name` = 'msg_spell_failed_focus' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '被中断' WHERE `name` = 'msg_spell_failed_interrupted' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '需要物品' WHERE `name` = 'msg_spell_failed_item' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '尚未就绪' WHERE `name` = 'msg_spell_failed_not_ready' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '需要武器' WHERE `name` = 'msg_spell_failed_weapon' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%spell 已从忽略列表中移除' WHERE `name` = 'msg_spell_removed_from_ignored' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== 法术 ===' WHERE `name` = 'msg_spells_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '阵型：%name' WHERE `name` = 'msg_stance_current' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '阵型已设为：%stance' WHERE `name` = 'msg_stance_set' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '请设置为：|cffffffff near（默认）、tank、turnback、behind' WHERE `name` = 'msg_stance_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '开始做任务 %id' WHERE `name` = 'msg_start_do_quest' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '留守点太远无法返回，我留在当前位置' WHERE `name` = 'msg_stay_too_far' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已激活第一套天赋' WHERE `name` = 'msg_talents_active_first' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已激活第二套天赋' WHERE `name` = 'msg_talents_active_second' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已应用 %param' WHERE `name` = 'msg_talents_applying' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已自动选择天赋' WHERE `name` = 'msg_talents_auto_pick' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '无效链接 %param' WHERE `name` = 'msg_talents_invalid_link' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我当前天赋专精：' WHERE `name` = 'msg_talents_my_spec' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '正在切换至 %spec' WHERE `name` = 'msg_talents_picking' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未找到专精 %param' WHERE `name` = 'msg_talents_spec_not_found' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '共找到 %n 个专精' WHERE `name` = 'msg_talents_spec_total' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '未知命令。' WHERE `name` = 'msg_talents_unknown_cmd' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '天赋用法：talents switch <1/2>，talents autopick，talents spec list，talents spec <专精名>，talents apply <链接>。' WHERE `name` = 'msg_talents_usage' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '=== 飞行 ===' WHERE `name` = 'msg_taxi_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '使用 %name 传送' WHERE `name` = 'msg_teleporting_using' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '感谢邀请！' WHERE `name` = 'msg_thanks_invite' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 仇恨 ---' WHERE `name` = 'msg_threat_title' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '继续 ' WHERE `name` = 'msg_travel_continuing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '跟随队伍 ' WHERE `name` = 'msg_travel_following_group' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '探索 ' WHERE `name` = 'msg_travel_for_exploration' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '刷钱 ' WHERE `name` = 'msg_travel_for_grinding' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '打宝 ' WHERE `name` = 'msg_travel_for_loot' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '任务 %quest ' WHERE `name` = 'msg_travel_for_quest' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '修理装备 ' WHERE `name` = 'msg_travel_for_repairing' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '闲逛 ' WHERE `name` = 'msg_travel_for_rpg' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '出售物品 ' WHERE `name` = 'msg_travel_for_selling' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '（强制） ' WHERE `name` = 'msg_travel_forced' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' 到 %dest' WHERE `name` = 'msg_travel_to' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '前往 ' WHERE `name` = 'msg_travel_traveling' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '前往 %dest' WHERE `name` = 'msg_traveling_to' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%item 已卸下' WHERE `name` = 'msg_unequipped' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '已对 %item 使用开锁' WHERE `name` = 'msg_used_pick_lock_on' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '使用传送门：%name' WHERE `name` = 'msg_using_portal' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '等等我！' WHERE `name` = 'msg_wait_for_me' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '稍等 %name 一下' WHERE `name` = 'msg_waiting_for' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '欢迎回来！' WHERE `name` = 'msg_welcome_back' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我马上到。' WHERE `name` = 'msg_will_there_soon' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我愿以 %money 收购 %item' WHERE `name` = 'msg_wts_buy' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%player 正在移动以阻挡蓝色光束！' WHERE `name` = 'netherspite_beam_blocking_blue' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%player 正在移动以阻挡绿色光束！' WHERE `name` = 'netherspite_beam_blocking_green' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%player 正在移动以阻挡红色光束！' WHERE `name` = 'netherspite_beam_blocking_red' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%player 离开蓝色光束——下一个挡光者上！' WHERE `name` = 'netherspite_beam_leaving_blue' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%player 离开绿色光束——下一个挡光者上！' WHERE `name` = 'netherspite_beam_leaving_green' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '附近没有商人' WHERE `name` = 'no_vendors_nearby' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '[PVP] %bracket：<%team_name>（等级 %team_rating）' WHERE `name` = 'pvp_arena_team' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '[PVP] 竞技场点数：%arena_points | 荣誉点数：%honor_points' WHERE `name` = 'pvp_currency' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '[PVP] 我没有竞技场队伍。' WHERE `name` = 'pvp_no_arena_team' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我当前是队长，需要的话可以邀请你。' WHERE `name` = 'security_am_leader' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我现在有点忙' WHERE `name` = 'security_busy' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我做不到' WHERE `name` = 'security_cant_do' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我死了，稍后再做' WHERE `name` = 'security_dead' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我稍后再做' WHERE `name` = 'security_do_later' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我在满员队伍中，稍后再做' WHERE `name` = 'security_full_group' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '装备分数太低：|cffff0000%from|cffffffff/|cff00ff00%bot |cffff0000%diff%%|cffffffff/|cff00ff00%req%%' WHERE `name` = 'security_gearscore_low' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我已有主人' WHERE `name` = 'security_has_master' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我在战场队列中，稍后再做' WHERE `name` = 'security_in_bg_queue' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我和别人在一队，你可以找他邀请。' WHERE `name` = 'security_in_group_other' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我和 %name 在一队，你可以找他邀请。' WHERE `name` = 'security_in_group_with' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '我在副本队列中，稍后再做' WHERE `name` = 'security_in_lfg_queue' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '请先邀请我入队' WHERE `name` = 'security_invite_first' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你等级太低：|cffff0000%from|cffffffff/|cff00ff00%bot' WHERE `name` = 'security_low_level' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '请靠近些再邀请我入队。我在 %area' WHERE `name` = 'security_too_far' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你是机器人' WHERE `name` = 'security_you_bot' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '你是敌对' WHERE `name` = 'security_you_enemy' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '%name（%threat）' WHERE `name` = 'threat_entry' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '--- 可从 %name 学习 ---' WHERE `name` = 'trainer_can_learn' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' - 已学会' WHERE `name` = 'trainer_learned' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = ' - 太贵' WHERE `name` = 'trainer_too_expensive' AND `text_loc4` = '';
UPDATE `ai_playerbot_texts` SET `text_loc4` = '总花费：%cost' WHERE `name` = 'trainer_total_cost' AND `text_loc4` = '';
