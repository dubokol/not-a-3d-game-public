#pragma once

#include "game_grid/game_grid.h"
#include "player/player.h"
#include <vector>
#include <string>
#include <map>

class World {
    GameGrid game_grid_;
    Player player_;
    int timer_;
    bool hard_mode_;
    std::string name_;

    bool player_can_enter(int x, int y, int z) const;                 // Проверяет может ли игрок быть в клетке

public:
    World();                                                          // Создает мир

    void reset(int max_x, int max_y, int max_z, bool hard_mode);      // Пересоздает мир
    void move_player(int dx, int dy, int dz);                         // Двигает игрока

    const GameGrid& get_game_grid() const;
    const Player& get_player() const;
    const std::vector<int>& get_player_key_set() const;
    bool has_player_escaped() const;

    std::map<std::string, std::string> get_state() const;
    void set_state(const std::map<std::string, std::string>& state);
};
