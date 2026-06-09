#include <algorithm>
#include <queue>
#include "game_grid.h"
#include "logger.h"
#include "utils.h"
#include <random>

using std::to_string;

static std::string to_string(const Bonus &bonus) {
    std::string str;
    switch (bonus) {
        case Bonus::NONE:
            str = "NONE";
            break;
        case Bonus::BONUS1:
            str = "BONUS1";
            break;
        case Bonus::BONUS2:
            str = "BONUS2";
            break;
    }
    return str;
}

static std::string to_string(const CellType& type) {
    std::string str;
    switch (type) {
        case CellType::EMPTY:
            str = "EMPTY";
            break;
        case CellType::BLOCKED:
            str = "BLOCKED";
            break;
        case CellType::OUTSIDE:
            str = "OUTSIDE";
            break;
    }
    return str;
}

GameGrid::GameGrid() : probability_of_empty_cell_(), seed_(), size_x_(), size_y_(), size_z_(), start_x_(), start_y_(), start_z_() {
    set_default_probabilities();
}

void GameGrid::set_default_probabilities() {
    probability_of_empty_cell_ = 0.55;
    prob_of_bonus_[Bonus::BONUS1] = 0.05;
    prob_of_bonus_[Bonus::BONUS2] = 0.50;
}

void GameGrid::set_size(int size_x, int size_y, int size_z) {
    size_x_ = size_x;
    size_y_ = size_y;
    size_z_ = size_z;
    if (size_x < 1 || size_y < 1 || size_z < 1) ERR("Minimum size of a 3d game_grid is 1.");
    grid_.assign(size_x, std::vector(size_y, std::vector<Cell>(size_z, {CellType::EMPTY, Bonus::NONE})));
}

void GameGrid::reset(int size_x, int size_y, int size_z, bool generate_bonus2) {
    set_default_probabilities();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10000);
    reset(size_x, size_y, size_z, generate_bonus2, dist(gen));
}

void GameGrid::reset(int size_x, int size_y, int size_z, bool generate_bonus2, int seed) {
    set_size(size_x, size_y, size_z);

    if (!generate_bonus2) prob_of_bonus_[Bonus::BONUS2] = 0.0;

    generate(seed);
}

const std::vector<std::vector<std::vector<Cell>>> &GameGrid::get_grid() const {
    return grid_;
}

Cell GameGrid::get_cell(int x, int y, int z) const {
    return Cell{get_type(x, y, z), get_bonus(x, y, z)};
}

CellType GameGrid::get_type(int x, int y, int z) const {
    if (is_outside(x, y, z)) {
        return CellType::OUTSIDE;
    }
    return grid_[x][y][z].type;
}

Bonus GameGrid::get_bonus(int x, int y, int z) const {
    if (is_outside(x, y, z)) {
        return Bonus::NONE;
    }
    return grid_[x][y][z].bonus;
}

int GameGrid::get_start_x() const {
    return start_x_;
}

int GameGrid::get_start_y() const {
    return start_y_;
}

int GameGrid::get_start_z() const {
    return start_z_;
}

int GameGrid::get_seed() const {
    return seed_;
}

int GameGrid::get_size() const {
    return get_size_x() * get_size_y() * get_size_z();
}

int GameGrid::get_size_x() const {
    return size_x_;
}

int GameGrid::get_size_y() const {
    return size_y_;
}

int GameGrid::get_size_z() const {
    return size_z_;
}

void GameGrid::set_cell(int x, int y, int z, const Cell &cell) {
    if (is_outside(x, y, z)) ERR("Not a valid position.");
    grid_[x][y][z] = cell;
}

void GameGrid::set_bonus(int x, int y, int z, const Bonus &bonus) {
    if (is_outside(x, y, z)) ERR("Not a valid position.");
    grid_[x][y][z].bonus = bonus;
}

void GameGrid::set_type(int x, int y, int z, const CellType &type) {
    if (is_outside(x, y, z)) ERR("Not a valid position.");
    grid_[x][y][z].type = type;
}

bool GameGrid::is_outside(int x, int y, int z) const {
    if (x < 0 || x >= get_size_x() ||
        y < 0 || y >= get_size_y() ||
        z < 0 || z >= get_size_z()) {
        return true;
    }
    return false;
}

bool GameGrid::is_start(int x, int y, int z) const {
    return x == start_x_ && y == start_y_ && z == start_z_;
}

bool GameGrid::is_reachable(int x, int y, int z) const {
    if (is_outside(x, y, z)) return true;
    return can_visit_[x][y][z];
}

bool GameGrid::dfs(int x, int y, int z, std::vector<std::vector<std::vector<bool>>> &used, std::mt19937 &gen) {
    if (is_outside(x, y, z)) return true;

    Cell prev = grid_[x][y][z];
    grid_[x][y][z].type = CellType::EMPTY;
    used[x][y][z] = true;

    std::vector<std::tuple<int, int, int>> directions = {
            {1,  0,  0},
            {-1, 0,  0},
            {0,  1,  0},
            {0,  -1, 0}
    };

    if (size_z_ > 1) {
        directions.emplace_back(0,  0,  1);
        directions.emplace_back(0,  0,  -1);
    }

    std::ranges::shuffle(directions, gen);

    for (const auto& [dx, dy, dz]: directions) {
        int nx = x + dx;
        int ny = y + dy;
        int nz = z + dz;

        if (is_outside(nx, ny, nz) || !used[nx][ny][nz]) {
            if (dfs(nx, ny, nz, used, gen)) {
                return true;
            }
        }
    }

    grid_[x][y][z] = prev;
    used[x][y][z] = false;

    return false;
}

void GameGrid::compute_reachability() {
    int size_x = get_size_x();
    int size_y = get_size_y();
    int size_z = get_size_z();

    can_visit_.assign(size_x, std::vector(size_y, std::vector(size_z, false)));

    if (is_outside(start_x_, start_y_, start_z_) ||
        grid_[start_x_][start_y_][start_z_].type != CellType::EMPTY) {
        ERR("Please delete the game, it's broken");
    }

    std::queue<std::tuple<int, int, int>> q;
    q.emplace(start_x_, start_y_, start_z_);
    can_visit_[start_x_][start_y_][start_z_] = true;

    std::vector<std::tuple<int, int, int>> directions = {
            {1,  0,  0},
            {-1, 0,  0},
            {0,  1,  0},
            {0,  -1, 0},
            {0,  0,  1},
            {0,  0,  -1}
    };

    while (!q.empty()) {
        auto [x, y, z] = q.front();
        q.pop();

        for (const auto& [dx, dy, dz]: directions) {
            int nx = x + dx, ny = y + dy, nz = z + dz;
            if (!is_outside(nx, ny, nz) && grid_[nx][ny][nz].type == CellType::EMPTY && !can_visit_[nx][ny][nz]) {
                can_visit_[nx][ny][nz] = true;
                q.emplace(nx, ny, nz);
            }
        }
    }
}

void GameGrid::bonus_by_prob(const Bonus &bonus, std::mt19937 &gen) {
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    int size_x = get_size_x();
    int size_y = get_size_y();
    int size_z = get_size_z();
    for (int x = 0; x < size_x; x++) {
        for (int y = 0; y < size_y; y++) {
            for (int z = 0; z < size_z; z++) {
                if (grid_[x][y][z].type == CellType::EMPTY && grid_[x][y][z].bonus == Bonus::NONE &&
                    !is_start(x, y, z) && is_reachable(x, y, z)) {
                    bool good_chance = prob_dist(gen) < prob_of_bonus_[bonus];
                    grid_[x][y][z].bonus = good_chance ? bonus : Bonus::NONE;
                }
            }
        }
    }
}

void GameGrid::generate(int seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    int size_x = get_size_x();
    int size_y = get_size_y();
    int size_z = get_size_z();

    int start_x = size_x / 2;
    int start_y = size_y / 2;
    int start_z = size_z / 2;

    start_x_ = start_x;
    start_y_ = start_y;
    start_z_ = start_z;
    seed_ = seed;

    for (int x = 0; x < size_x; x++) {
        for (int y = 0; y < size_y; y++) {
            for (int z = 0; z < size_z; z++) {
                bool empty = prob_dist(gen) < probability_of_empty_cell_;
                grid_[x][y][z].type = empty ? CellType::EMPTY : CellType::BLOCKED;
            }
        }
    }

    std::vector used(size_x, std::vector(size_y, std::vector(size_z, false)));

    dfs(start_x, start_y, start_z, used, gen);

    compute_reachability();

    for (auto &[bonus, prob]: prob_of_bonus_) {
        bonus_by_prob(bonus, gen);
    }
}

std::map<std::string, std::string> GameGrid::get_state() const {
    std::map<std::string, std::string> state;
    state[TO_STR(probability_of_empty_cell_)] = to_string(probability_of_empty_cell_);
    state[TO_STR(seed_)] = to_string(seed_);
    state[TO_STR(size_x_)] = to_string(size_x_);
    state[TO_STR(size_y_)] = to_string(size_y_);
    state[TO_STR(size_z_)] = to_string(size_z_);
    for (const auto& [bonus, d] : prob_of_bonus_) state[to_string(bonus)] = to_string(d);

    return state;
}

void GameGrid::set_state(const std::map<std::string, std::string>& state) {
    double probability_of_empty_cell = std::stod(state.at(TO_STR(probability_of_empty_cell_)));
    int seed = std::stoi(state.at(TO_STR(seed_)));
    int size_x = std::stoi(state.at(TO_STR(size_x_)));
    int size_y = std::stoi(state.at(TO_STR(size_y_)));
    int size_z = std::stoi(state.at(TO_STR(size_z_)));

    probability_of_empty_cell_ = probability_of_empty_cell;
    for (auto& [bonus, d] : prob_of_bonus_) d = std::stod(state.at(to_string(bonus)));

    reset(size_x, size_y, size_z, std::stoi(state.at(TO_STR(hard_mode_))), seed);
}
