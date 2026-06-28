DELETE FROM `ai_playerbot_texts` WHERE `name` = 'error_strategy_except';

INSERT INTO `ai_playerbot_texts`
    (`id`, `name`, `text`, `say_type`, `reply_type`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`)
VALUES
    (1817, 'error_strategy_except', 'You can change any strategy except loot', 0, 0,
     '루팅 빼고 전략은 바꿀 수 있어.',
     'Tu peux changer toute stratégie sauf loot.',
     'Alle Strategien außer Loot änderbar.',
     '除拾取外，其他策略均可修改',
     '除拾取外，其他策略均可修改',
     'Puedes cambiar cualquier estrategia excepto loot.',
     'Puedes cambiar cualquier estrategia excepto loot.',
     'Можно менять любую стратегию, кроме loot.');
