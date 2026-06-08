#pragma once

#include "game_logic/game_grid/game_grid.h"
#include "game_logic/player/player.h"
#include "game_logic/world.h"

class DebugGraphics {
    static int _;
    const GameGrid* game_grid_;
    const Player* player_;

public:
    DebugGraphics(GameGrid* game_grid, Player* player, int);
    DebugGraphics(World* world, int);
    void window_update();
};
