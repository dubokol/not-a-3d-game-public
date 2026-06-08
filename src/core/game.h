#pragma once

#include "game_logic/world.h"
#include <tuple>

enum class State {
    MENU,
    GAME,
    WIN
};

class Game {
    State state_;
    World world_;
    bool is_running_;

    std::tuple<int, int, int, int, bool, int> run_menu();

public:
    Game();

    void run();
};
