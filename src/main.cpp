#include "SDL3/SDL_main.h"
#include "core/game.h"
#include "core/file_managers/logs_manager.h"
#include "core/file_managers/worlds_manager.h"

int main(int argc, char* argv[]) {
    LogsManager::init();
    WorldsManager::init();

    Game game;
    game.run();

    return 0;
}
