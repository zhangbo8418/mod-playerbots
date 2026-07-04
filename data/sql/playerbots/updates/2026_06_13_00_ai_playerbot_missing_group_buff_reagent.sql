DELETE FROM ai_playerbot_texts_chance
WHERE name IN (
  'rp_missing_reagent_greater_blessing',
  'rp_missing_reagent_gift_of_the_wild',
  'rp_missing_reagent_arcane_brilliance',
  'rp_missing_reagent_generic',
  'missing_group_buff_reagent'
);

DELETE FROM ai_playerbot_texts
WHERE name IN (
  'rp_missing_reagent_greater_blessing',
  'rp_missing_reagent_gift_of_the_wild',
  'rp_missing_reagent_arcane_brilliance',
  'rp_missing_reagent_generic',
  'missing_group_buff_reagent'
);

INSERT INTO ai_playerbot_texts
  (id, name, text, say_type, reply_type, text_loc1, text_loc2, text_loc3, text_loc4, text_loc5, text_loc6, text_loc7, text_loc8)
VALUES
  (2529, 'missing_group_buff_reagent',
   'I am out of reagents for %group_spell and am casting %base_spell instead.',
   0, 0,
   '%group_spell 재료가 없어서 %base_spell(을) 시전합니다.',
   'Je n''ai plus de composants pour %group_spell, je lance %base_spell à la place.',
   'Mir fehlen die Reagenzien für %group_spell, ich wirke stattdessen %base_spell.',
   '%group_spell 的材料用完了，改施放 %base_spell。',
   '%group_spell 的材料用完了，改施放 %base_spell。',
   'Me quedé sin componentes para %group_spell, lanzo %base_spell en su lugar.',
   'Me quedé sin componentes para %group_spell, lanzo %base_spell en su lugar.',
   'У меня кончились реагенты для %group_spell, вместо этого кастую %base_spell.');

INSERT INTO ai_playerbot_texts_chance
  (name, probability)
VALUES
  ('missing_group_buff_reagent', 100);
