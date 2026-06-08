#include <iostream>
#include "game.h"
#include "logger.h"
#include "graphics/game_graphic.h"
#include "core/input_handler/input_handler.h"
#include "core/file_managers/worlds_manager.h"

static constexpr int keys[] = {SDLK_Q, SDLK_W, SDLK_E, SDLK_R, SDLK_T, SDLK_Y, SDLK_U, SDLK_I, SDLK_O, SDLK_P,
                               SDLK_A, SDLK_S, SDLK_D, SDLK_F, SDLK_G, SDLK_H, SDLK_J, SDLK_K, SDLK_L,
                               SDLK_Z, SDLK_X, SDLK_C, SDLK_V, SDLK_B, SDLK_N, SDLK_M};

Game::Game()
        : state_(State::GAME), world_(), is_running_(false) {}

void check(int& n, const int x)
{
    if (n < x) n = x;
}

void try_to_move_player(const InputHandler& input, World& world_) {
    const std::vector<int>& key_set = world_.get_player_key_set();
    if (input.isKeyDown(keys[key_set[0]])) {
        world_.move_player(0, -1, 0);
    } if (input.isKeyDown(keys[key_set[1]])) {
        world_.move_player(0, 1, 0);
    } if (input.isKeyDown(keys[key_set[2]])) {
        world_.move_player(-1, 0, 0);
    } if (input.isKeyDown(keys[key_set[3]])) {
        world_.move_player(1, 0, 0);
    } if (input.isKeyDown(keys[key_set[4]])) {
        world_.move_player(0, 0, 1);
    } if (input.isKeyDown(keys[key_set[5]])) {
        world_.move_player(0, 0, -1);
    }
}

std::tuple<int, int, int, int, bool, int> Game::run_menu()
{
    MenuWindow menu_window;
    Buttons buttons;
    int x = 10, y = 10, z = 10, min_mult = 10, mult = 40;
    bool hard_mode = false;

    menu_window.link_buttons(buttons);
    std::string button_was_pressed = "None";
    SDL_FRect rct = {(float) 0, (float) 200, (float) 100, (float) 50};
    Button multiplier_readings(rct, std::to_string(mult));
    multiplier_readings.normalColor = {0, 0, 0, 255};
    rct = {(float) 0, (float) 300, (float) 100, (float) 50};
    Button x_readings(rct, std::to_string(mult));
    x_readings.normalColor = {0, 0, 0, 255};
    rct = {(float) 0, (float) 400, (float) 100, (float) 50};
    Button y_readings(rct, std::to_string(mult));
    y_readings.normalColor = {0, 0, 0, 255};
    rct = {(float) 0, (float) 500, (float) 100, (float) 50};
    Button z_readings(rct, std::to_string(mult));
    z_readings.normalColor = {0, 0, 0, 255};

    int test = 0;
    rct = {(float) 0, (float) 100, (float) 100, (float) 50};
    Button test_readings(rct, std::to_string(mult));
    test_readings.normalColor = {0, 0, 0, 255};

    while (is_running_) {
        menu_window.update(buttons);
        multiplier_readings.text = std::to_string(mult);
        x_readings.text = std::to_string(x);
        y_readings.text = std::to_string(y);
        z_readings.text = std::to_string(z);
        test_readings.text = std::to_string(test);
        multiplier_readings.render(menu_window.mrender_, menu_window.mfont_);
        x_readings.render(menu_window.mrender_, menu_window.mfont_);
        y_readings.render(menu_window.mrender_, menu_window.mfont_);
        z_readings.render(menu_window.mrender_, menu_window.mfont_);
        test_readings.render(menu_window.mrender_, menu_window.mfont_);
        button_was_pressed = buttons.buttons_processing();
        if (button_was_pressed == "START") break;
        else if (button_was_pressed == "OFF") {
            menu_window.Hard_Mode.text = "ON";
            hard_mode = true;
        } else if (button_was_pressed == "ON") {
            menu_window.Hard_Mode.text = "OFF";
            hard_mode = false;
        } else if (button_was_pressed == "s+100") mult += 100;
        else if (button_was_pressed == "s+10") mult += 10;
        else if (button_was_pressed == "s+1") mult += 1;
        else if (button_was_pressed == "s-1") mult -= 1;
        else if (button_was_pressed == "s-10") mult -= 10;
        else if (button_was_pressed == "s-100") mult -= 100;
        else if (button_was_pressed == "x+100") x += 100;
        else if (button_was_pressed == "x+10") x += 10;
        else if (button_was_pressed == "x+1") x += 1;
        else if (button_was_pressed == "x-1") x -= 1;
        else if (button_was_pressed == "x-10") x -= 10;
        else if (button_was_pressed == "x-100") x -= 100;
        else if (button_was_pressed == "y+100") y += 100;
        else if (button_was_pressed == "y+10") y += 10;
        else if (button_was_pressed == "y+1") y += 1;
        else if (button_was_pressed == "y-1") y -= 1;
        else if (button_was_pressed == "y-10") y -= 10;
        else if (button_was_pressed == "y-100") y -= 100;
        else if (button_was_pressed == "z+100") z += 100;
        else if (button_was_pressed == "z+10") z += 10;
        else if (button_was_pressed == "z+1") z += 1;
        else if (button_was_pressed == "z-1") z -= 1;
        else if (button_was_pressed == "z-10") z -= 10;
        else if (button_was_pressed == "z-100") z -= 100;
        else if (button_was_pressed == "T+1") test += 1;
        else if (button_was_pressed == "T-1") test -= 1;
        else if (button_was_pressed == "T=0") test = 0;
        else if (button_was_pressed == "ESC") is_running_ = false;
        check(x, 1);
        check(y, 1);
        check(z, 1);
        check(mult, min_mult);
    }
    return {mult, x, y, z, hard_mode, test};
}

void Game::run() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        ERR("SDL_Init error: " + std::string(SDL_GetError()));
    }
    if (!TTF_Init()) {
        ERR("TTF_Init error");
    }

    is_running_ = true;

    while (is_running_)
    {
        state_ = State::MENU;
        auto [mult, x, y, z, hard_mode, test] = run_menu();
        if (!is_running_) break;

        if (test <= 0) {
            world_.reset(x, y, z, hard_mode);
            if (test < 0) {
                WorldsManager::save(world_.get_state(), std::to_string(-test));
            }
        } else {
            if (WorldsManager::exists(std::to_string(test))) {
                world_.set_state(WorldsManager::load(std::to_string(test)));
            } else {
                LOG("Can't find save " + std::to_string(test));
                continue;
            }
        }

        InputHandler input;
        GameWindow game_window(&world_, mult);

        LOG("Game started!");
        LOG(world_.get_state());

        state_ = State::GAME;
        while (state_ == State::GAME) {
            input.update();

            if (input.isQuit()) break;

            switch (state_) {
                case (State::MENU):
                    break;
                case (State::GAME):
                    try_to_move_player(input, world_);

                    if (world_.has_player_escaped()) {
                        state_ = State::WIN;
                        LOG("Player won!");
                    }

                    game_window.update();
                    break;
                case (State::WIN):
                    break;
            }
        }

        int iterations = 15;
        while (iterations--) {
            game_window.update();
        }
    }

    SDL_Quit();
    TTF_Quit();
    LOG("Game stopped!");
}
