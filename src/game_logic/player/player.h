#pragma once

#include <map>
#include "game_logic/game_grid/game_grid.h"

class Player {
    int x_, y_, z_;                 // Координаты игрока
    bool escaped_;                  // Прошел ли игрок лабиринт
    int viewing_radius_;            // Радиус видимости
    std::vector<int> key_set_;      // no comment

    void set_default_key_set();
public:
    Player();                                        // Конструктор

    void reset(int start_x, int start_y, int start_z, bool hard_mode);
    void set_position(int x, int y, int z);          // Задает позицию игрока
    bool has_escaped() const;                        // Проверяет, прошел ли игрок лабиринт
    void set_escaped(bool escaped);                  // no comment
    int get_x() const;                               // Возвращает X
    int get_y() const;                               // Возвращает Y
    int get_z() const;                               // Возвращает Z
    void set_viewing_radius(int viewing_radius);     // Задает радиус видимости
    void change_key_set();                           // no comment
    int get_viewing_radius() const;                  // no comment
    const std::vector<int>& get_key_set() const;     // no comment

    std::map<std::string, std::string> get_state() const;
    void set_state(const std::map<std::string, std::string>& state);
};
