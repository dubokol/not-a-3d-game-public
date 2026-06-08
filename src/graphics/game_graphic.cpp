#include "game_graphic.h"




void MenuWindow::mb(int x, int y, int w, int h, Button& b, std::string t)
{
    SDL_FRect rct = {(float)x, (float)y, (float)w, (float)h};
    b = Button(rct, t);
}

MenuWindow::MenuWindow()
{
    mwindow_ = SDL_CreateWindow("game", WIDTH_, HEIGHT_, 0);
    if (!mwindow_) {
        std::cerr << "w������ �������� ����: " << SDL_GetError() << std::endl;
        return;
    }

    mrender_ = SDL_CreateRenderer(mwindow_, NULL);
    if (!mrender_) {
        std::cerr << "������ �������� ���������: " << SDL_GetError() << std::endl;
    }
    mb(300, 600, 200, 100, Game_Start, "START");
    Game_Start.normalColor = {100, 200, 100, 255};
    Game_Start.normalColor = {150, 250, 150, 255};
    Game_Start.normalColor = {50, 150, 50, 255};
    mb(700, 700, 50, 50, Hard_Mode, "OFF");
    mb(100, 200, 100, 50, Plus_h, "s+100");
    mb(200, 200, 100, 50, Plus_t, "s+10");
    mb(300, 200, 100, 50, Plus_o, "s+1");
    mb(400, 200, 100, 50, Minus_o, "s-1");
    mb(500, 200, 100, 50, Minus_t, "s-10");
    mb(600, 200, 100, 50, Minus_h, "s-100");
    mb(100, 300, 100, 50, xPlus_h, "x+100");
    mb(200, 300, 100, 50, xPlus_t, "x+10");
    mb(300, 300, 100, 50, xPlus_o, "x+1");
    mb(400, 300, 100, 50, xMinus_o, "x-1");
    mb(500, 300, 100, 50, xMinus_t, "x-10");
    mb(600, 300, 100, 50, xMinus_h, "x-100");
    mb(100, 400, 100, 50, yPlus_h, "y+100");
    mb(200, 400, 100, 50, yPlus_t, "y+10");
    mb(300, 400, 100, 50, yPlus_o, "y+1");
    mb(400, 400, 100, 50, yMinus_o, "y-1");
    mb(500, 400, 100, 50, yMinus_t, "y-10");
    mb(600, 400, 100, 50, yMinus_h, "y-100");
    mb(100, 500, 100, 50, zPlus_h, "z+100");
    mb(200, 500, 100, 50, zPlus_t, "z+10");
    mb(300, 500, 100, 50, zPlus_o, "z+1");
    mb(400, 500, 100, 50, zMinus_o, "z-1");
    mb(500, 500, 100, 50, zMinus_t, "z-10");
    mb(600, 500, 100, 50, zMinus_h, "z-100");
    mb(250, 100, 100, 50, Test_m, "T+1");
    mb(450, 100, 100, 50, Test_p, "T-1");
    mb(350, 100, 100, 50, Test_set_zero, "T=0");
}


void MenuWindow::update(Buttons& buttons)
{

    buttons.buttons_render(mrender_, mfont_);
    SDL_RenderPresent(mrender_);
    SDL_RenderClear(mrender_);
}

void MenuWindow::link_buttons(Buttons& buttons)
{
    buttons.add_button(Game_Start);
    buttons.add_button(Hard_Mode);
    buttons.add_button(Plus_h);
    buttons.add_button(Plus_t);
    buttons.add_button(Plus_o);
    buttons.add_button(Minus_o);
    buttons.add_button(Minus_t);
    buttons.add_button(Minus_h);
    buttons.add_button(xPlus_h);
    buttons.add_button(xPlus_t);
    buttons.add_button(xPlus_o);
    buttons.add_button(xMinus_o);
    buttons.add_button(xMinus_t);
    buttons.add_button(xMinus_h);
    buttons.add_button(yPlus_h);
    buttons.add_button(yPlus_t);
    buttons.add_button(yPlus_o);
    buttons.add_button(yMinus_o);
    buttons.add_button(yMinus_t);
    buttons.add_button(yMinus_h);
    buttons.add_button(zPlus_h);
    buttons.add_button(zPlus_t);
    buttons.add_button(zPlus_o);
    buttons.add_button(zMinus_o);
    buttons.add_button(zMinus_t);
    buttons.add_button(zMinus_h);
    buttons.add_button(Test_p);
    buttons.add_button(Test_m);
    buttons.add_button(Test_set_zero);
}

MenuWindow::~MenuWindow()
{
    if (mrender_) {
        SDL_DestroyRenderer(mrender_);
    }
    if (mwindow_) {
        SDL_DestroyWindow(mwindow_);
    }
}



GameWindow::GameWindow(World* world, int multiplier)
{
    mygrid = &world->get_game_grid();
    myplayer = &world->get_player();
    multiplier_ = multiplier;

    window_ = SDL_CreateWindow("game", WIDTH_ * multiplier_, HEIGHT_ * multiplier_, 0);
    if (!window_) {
        std::cerr << "w������ �������� ����: " << SDL_GetError() << std::endl;
        return;
    }

    render_ = SDL_CreateRenderer(window_, NULL);
    if (!render_) {
        std::cerr << "������ �������� ���������: " << SDL_GetError() << std::endl;
    }
}

GameWindow::~GameWindow()
{
    if (render_) {
        SDL_DestroyRenderer(render_);
    }
    if (window_) {
        SDL_DestroyWindow(window_);
    }
}

void GameWindow::change_size(int nmultiplier)
{
    multiplier_ = nmultiplier;
    SDL_SetWindowSize(window_, WIDTH_ * multiplier_, HEIGHT_ * multiplier_);
}

void GameWindow::draw_wall(int x, int y, CellType idk) const
{
    switch (idk) {
        case CellType::BLOCKED:
            SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
            break;
        case CellType::OUTSIDE:
            SDL_SetRenderDrawColor(render_, 255, 0, 0, 255);
            break;
    }
    SDL_FRect tmp = {
            (float) (x * multiplier_),
            (float) (y * multiplier_),
            (float) multiplier_,
            (float) multiplier_
    };
    SDL_RenderFillRect(render_, &tmp);
}

void GameWindow::draw_space(int x, int y, Bonus bonus) const
{
    switch (bonus) {
        case Bonus::BONUS1:
            SDL_SetRenderDrawColor(render_, 0, 255, 0, 255);
            break;
        case Bonus::BONUS2:
            SDL_SetRenderDrawColor(render_, 0, 0, 255, 255);
            break;
        case Bonus::NONE:
            SDL_SetRenderDrawColor(render_, 255, 255, 255, 255);
            break;
    }
    SDL_FRect tmp1 = {
            (float)(x * multiplier_),
            (float)(y * multiplier_),
            (float)multiplier_,
            (float)multiplier_
    };
    SDL_RenderFillRect(render_, &tmp1);
}

void GameWindow::draw_player(int x, int y) const
{
        SDL_SetRenderDrawColor(render_, 255, 0, 204, 255);
        SDL_FRect tmp = {
            (float)(x * multiplier_),
            (float)(y * multiplier_),
            (float)multiplier_,
            (float)multiplier_
        };
        SDL_RenderFillRect(render_, &tmp);
}

void GameWindow::update()
{
    viewing_radius_ = myplayer->get_viewing_radius();
    SDL_SetRenderDrawColor(render_, 100, 100, 100, 255);
    SDL_RenderClear(render_);

    minimap();
    SDL_RenderPresent(render_);
}

void GameWindow::minimap() const
{
    int x = myplayer->get_x();
    int y = myplayer->get_y();
    int z = myplayer->get_z();

    int x1 = 4;
    int y1 = 4;
    int x2 = 11;
    int y2 = 11;
    int x3 = 18;
    int y3 = 4;

    for (int ix = -viewing_radius_; ix <= viewing_radius_; ix++)
    {
        for (int iy = -viewing_radius_; iy <= viewing_radius_; iy++)
        {
            if (mygrid->get_type(x + ix, y + iy, z) == CellType::EMPTY)
            {
                draw_space(ix + x1, iy + y1, mygrid->get_bonus(x + ix, y + iy, z));

            }
            else
            {
                draw_wall(ix + x1, iy + y1, mygrid->get_type(x + ix, y + iy, z));
            }
        }
    }
    draw_player(x1, y1);

    for (int ix = -viewing_radius_; ix <= viewing_radius_; ix++)
    {
        for (int iz = -viewing_radius_; iz <= viewing_radius_; iz++)
        {
            if (mygrid->get_type(x + ix, y, z + iz) == CellType::EMPTY)
            {
                draw_space(ix + x2, iz + y2, mygrid->get_bonus(x + ix, y, z + iz));
            }
            else
            {
                draw_wall(ix + x2, iz + y2, mygrid->get_type(x + ix, y, z + iz));
            }
        }
    }
    draw_player(x2, y2);

    for (int iz = -viewing_radius_; iz <= viewing_radius_; iz++)
    {
        for (int iy = -viewing_radius_; iy <= viewing_radius_; iy++)
        {
            if (mygrid->get_type(x, y + iy, z + iz) == CellType::EMPTY)
            {
                draw_space(iz + x3, iy + y3, mygrid->get_bonus(x, y + iy, z + iz));
            }
            else
            {
                draw_wall(iz + x3, iy + y3, mygrid->get_type(x, y + iy, z + iz));
            }
        }
    }
    draw_player(x3, y3);
}
