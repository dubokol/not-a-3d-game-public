#pragma once

#include <random>
#include <map>
#include <vector>
#include <string>

enum class CellType {
    EMPTY,       // Пустая клетка
    BLOCKED,     // Заблокированная клетка
    OUTSIDE,     // Клетка за границами сетки
};

enum class Bonus {
    NONE,        // Нет бонуса
    BONUS1,      // Бонус 1
    BONUS2       // Бонус 2
};

struct Cell {
    CellType type = CellType::OUTSIDE; // Значение по умолчанию
    Bonus bonus = Bonus::NONE;         // Значение по умолчанию
};

class GameGrid {
    double probability_of_empty_cell_;                      // Вероятность генерации пустой клетки
    int seed_;                                              // Seed для генератора случайных чисел
    int size_x_, size_y_, size_z_;
    int start_x_, start_y_, start_z_;                       // Координаты стартовой позиции (x, y, z)
    std::vector<std::vector<std::vector<Cell>>> grid_;      // 3D-сетка клеток [x][y][z]
    std::vector<std::vector<std::vector<bool>>> can_visit_; // Можно ли попасть в клетку из старта

    void compute_reachability();                            // Вычисляет достижимость клеток из старта с помощью BFS
    bool dfs(int x, int y, int z, std::vector<std::vector<std::vector<bool>>> &used,
             std::mt19937 &gen);                            // Поиск в глубину для генерации проходимого пути
    void bonus_by_prob(const Bonus &bonus, double prob,
                       std::mt19937 &gen);                  // Генерирует бонусы в пустых клетках с указанным шансом

    void generate(int seed);                                // Генерация сетки с указанным seed
    void reset(int size_x, int size_y, int size_z, bool generate_bonus2, int seed);   // Пересоздание сетки с заданным размером
public:
    std::map<Bonus, double> prob_of_bonus_;                 // Вероятность генерации бонуса в пустой клетке

    GameGrid();                                             // Конструктор, задает шансы появления бонусов и пустых клеток

    void reset(int size_x, int size_y, int size_z, bool generate_bonus2);   // Пересоздание сетки с заданным размером

    const std::vector<std::vector<std::vector<Cell>>> &get_grid() const;    // Возвращает константную ссылку на сетку
    Cell get_cell(int x, int y, int z) const;                               // Возвращает клетку по координатам
    CellType get_type(int x, int y, int z) const;                           // Возвращает тип клетки по координатам
    Bonus get_bonus(int x, int y, int z) const;                             // Возвращает бонус в клетке по координатам

    int get_start_x() const;                                   // Возвращает X стартовой позиции
    int get_start_y() const;                                   // Возвращает Y стартовой позиции
    int get_start_z() const;                                   // Возвращает Z стартовой позиции
    int get_seed() const;                                      // Возвращает seed генерации
    int get_size() const;                                      // Возвращает объем поля
    int get_size_x() const;                                    // Возвращает размер по X
    int get_size_y() const;                                    // Возвращает размер по Y
    int get_size_z() const;                                    // Возвращает размер по Z

    bool is_reachable(int x, int y, int z) const;              // Проверяет достижимость клетки из старта
    bool is_outside(int x, int y, int z) const;                // Проверяет снаружи ли клетка
    bool is_start(int x, int y, int z) const;                  // Проверяет совпадает ли клетка со стартовой

    void set_cell(int x, int y, int z, const Cell &cell);      // Устанавливает тип клетки
    void set_bonus(int x, int y, int z, const Bonus &bonus);   // no comment
    void set_type(int x, int y, int z, const CellType &type);  // no comment

    std::map<std::string, std::string> get_state() const;
    void set_state(const std::map<std::string, std::string>& state);
};
