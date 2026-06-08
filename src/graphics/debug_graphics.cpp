#include "debug_graphics.h"
#include "iostream"
#include "windows.h"

static void enableAnsi() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

using namespace std;

int DebugGraphics::_ = []() {
    enableAnsi();
    return 0;
}();

DebugGraphics::DebugGraphics(World* world, int) : game_grid_(&world->get_game_grid()), player_(&world->get_player()) {}

DebugGraphics::DebugGraphics(GameGrid *game_grid, Player *player, int) : game_grid_(game_grid), player_(player) {}

namespace Color {
    const char *RESET = "\033[0m";
    const char *RED = "\033[31m";
    const char *GREEN = "\033[32m";
    const char *YELLOW = "\033[33m";
    const char *BLUE = "\033[34m";
}

void DebugGraphics::window_update() {
    std::cout << "\033[2J\033[1;1H";
    auto pl_x = player_->get_x();
    auto pl_y = player_->get_y();
    auto &grid = game_grid_->get_grid();
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (i == pl_x && j == pl_y) {
                cout << Color::RED << 'p' << Color::RESET;
            } else {
                cout << (grid[i][j][0].type == CellType::EMPTY ? 0 : 1);
            }
        }
        cout << endl;
    }
}
