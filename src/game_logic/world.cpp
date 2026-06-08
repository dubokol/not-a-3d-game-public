#include "world.h"
#include "utils.h"
#include "logger.h"

bool World::player_can_enter(int x, int y, int z) const {
    return game_grid_.get_type(x, y, z) != CellType::BLOCKED;
}

World::World() : game_grid_(), player_(), timer_(0), hard_mode_(false), name_() {}

void World::reset(int max_x, int max_y, int max_z, bool hard_mode) {
    hard_mode_ = hard_mode;
    game_grid_.reset(max_x, max_y, max_z, hard_mode_);
    player_.reset(game_grid_.get_start_x(), game_grid_.get_start_y(), game_grid_.get_start_z(), hard_mode_);
    timer_ = 0;
    LOG("World has been reset");
}

void World::move_player(int dx, int dy, int dz) {
    int nx = player_.get_x() + dx;
    int ny = player_.get_y() + dy;
    int nz = player_.get_z() + dz;

    if (player_can_enter(nx, ny, nz)) {
        player_.set_position(nx, ny, nz);

        if (game_grid_.get_type(nx, ny, nz) == CellType::OUTSIDE) {
            player_.set_escaped(true);
        }

        switch (game_grid_.get_bonus(nx, ny, nz)) {
            case Bonus::BONUS1:
                game_grid_.set_bonus(nx, ny, nz, Bonus::NONE);
                timer_ = 10;
                break;
            case Bonus::BONUS2:
                player_.change_key_set();
                game_grid_.set_bonus(nx, ny, nz, Bonus::NONE);
                break;
            default:
                break;
        }

        if (timer_ > 0) {
            player_.set_viewing_radius(3);
            timer_ -= 1;
        } else if (timer_ == 0) {
            player_.set_viewing_radius(2);
        }
    }
}

const GameGrid& World::get_game_grid() const {
    return game_grid_;
}

const Player& World::get_player() const {
    return player_;
}

const std::vector<int>& World::get_player_key_set() const {
    return player_.get_key_set();
}

bool World::has_player_escaped() const {
    return player_.has_escaped();
}

static std::map<std::string, std::string> unite(const std::map<std::string, std::string>& m1, const std::map<std::string, std::string>& m2) {
    std::map<std::string, std::string> m;
    for (auto &[a, b] : m1) m[a] = b;
    for (auto &[a, b] : m2) m[a] = b;
    return m;
}

std::map<std::string, std::string> World::get_state() const {
    auto state = game_grid_.get_state();
    state[TO_STR(timer_)] = std::to_string(timer_);
    state[TO_STR(hard_mode_)] = std::to_string((int) hard_mode_);
    return unite(state, player_.get_state());
}

void World::set_state(const std::map<std::string, std::string>& state) {
    timer_ = std::stoi(state.at(TO_STR(timer_)));
    hard_mode_ = std::stoi(state.at(TO_STR(hard_mode_)));
    game_grid_.set_state(state);
    player_.set_state(state);
}
