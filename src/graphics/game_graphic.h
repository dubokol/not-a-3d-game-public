#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>
#include "game_logic/game_grid/game_grid.h"
#include "game_logic/player/player.h"
#include "game_logic/world.h"
#include "button.h"

class MenuWindow
{
private:
    void mb(int x, int y, int w, int h, Button& b, std::string t);

//    int ib();

    SDL_Window* mwindow_;

//    int button_pressed_;

    const int WIDTH_ = 800, HEIGHT_ = 800;

public:
    SDL_Renderer* mrender_;
    TTF_Font* mfont_ = TTF_OpenFont("resources/vgafixr.fon", 16);
    Button Minus_h;
    Button Minus_t;
    Button Minus_o;
    Button Plus_o;
    Button Plus_t;
    Button Plus_h;
    Button xMinus_h;
    Button xMinus_t;
    Button xMinus_o;
    Button xPlus_o;
    Button xPlus_t;
    Button xPlus_h;
    Button yMinus_h;
    Button yMinus_t;
    Button yMinus_o;
    Button yPlus_o;
    Button yPlus_t;
    Button yPlus_h;
    Button zMinus_h;
    Button zMinus_t;
    Button zMinus_o;
    Button zPlus_o;
    Button zPlus_t;
    Button zPlus_h;
    Button Hard_Mode;
    Button Game_Start;
    Button Test_p;
    Button Test_m;
    Button Test_set_zero;



    MenuWindow();
    ~MenuWindow();

    void update(Buttons& buttons);

    void link_buttons(Buttons& buttons);
};

class GameWindow
{
    SDL_Window* window_;
    SDL_Renderer* render_;

    const int WIDTH_ = 23, HEIGHT_ = 16;
    int viewing_radius_ = 2;
    int multiplier_ = 1;

    void minimap() const;
    void draw_wall(int x, int y, CellType idk) const;
    void draw_space(int x, int y, Bonus bonus) const;
    void draw_player(int x, int y) const;

public:
    const GameGrid* mygrid;
    const Player* myplayer;

    GameWindow(World* world, int multiplier);
    ~GameWindow();

    void change_size(int nmultiplier);
    void update();
};
