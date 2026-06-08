#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>






struct Button
{
    SDL_FRect rect;          // Прямоугольник кнопки (x, y, w, h)
    std::string text;        // Текст на кнопке
    SDL_Color normalColor;   // Цвет в обычном состоянии
    SDL_Color hoverColor;    // Цвет при наведении
    SDL_Color pressColor;    // Цвет при нажатии
    SDL_Color textColor;     // Цвет текста
    bool isHovered;          // Наведена ли мышь
    bool isPressed;          // Нажата ли кнопка

    void (*onClick)(void* userData);
    void* userData;

    Button(SDL_FRect r, const std::string& t)
            : rect(r), text(t), isHovered(false), isPressed(false),
              onClick(nullptr), userData(nullptr) {
        normalColor = {100, 100, 200, 255};    // серо-синий
        hoverColor  = {150, 150, 250, 255};    // светлее
        pressColor  = {50, 50, 150, 255};      // темнее
        textColor   = {255, 255, 255, 255};    // белый
    }

    Button() : Button({1, 1, 1, 1}, "Nothing") {}

    bool handleEvent(const SDL_Event& event) {

        switch (event.type) {
            case SDL_EVENT_MOUSE_MOTION: {
                float mx = event.motion.x;
                float my = event.motion.y;
                isHovered = (mx >= rect.x && mx <= rect.x + rect.w &&
                             my >= rect.y && my <= rect.y + rect.h);
                return false;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                if (event.button.button == SDL_BUTTON_LEFT && isHovered) {
                    isPressed = true;
                    return true;
                }
                return false;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (isPressed && isHovered && onClick) {
                        onClick(userData);
                    }
                    isPressed = false;
                }
                return false;
            }
        }
        return false;
    }

    void render(SDL_Renderer* renderer, TTF_Font* font) const {
        SDL_Color currentColor = normalColor;
        if (isPressed) currentColor = pressColor;
        else if (isHovered) currentColor = hoverColor;

        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderRect(renderer, &rect);

        if (font && !text.empty()) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    float textX = rect.x + (rect.w - surface->w) / 2;
                    float textY = rect.y + (rect.h - surface->h) / 2;
                    SDL_FRect dstRect = {textX, textY, (float)surface->w, (float)surface->h};
                    SDL_RenderTexture(renderer, texture, nullptr, &dstRect);
                    SDL_DestroyTexture(texture);
                }
                SDL_DestroySurface(surface);
            }
        }
    }
};




struct Buttons
{
private:
    std::vector<Button*> buttons_;

public:
    void add_button(Button& button)
    {
        buttons_.push_back(&button);
    }

    void pop_button()
    {
        buttons_.pop_back();
    }

    std::string buttons_processing()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            for (Button* a : buttons_)
            {
                if (a->handleEvent(event))
                {
                    return a->text;
                }
            }
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                return "ESC";
            }
        }
        return "None";
    }

    void buttons_render(SDL_Renderer* renderer, TTF_Font* font) const
    {
        for (Button* a : buttons_)
        {
            a->render(renderer, font);
        }
    }
};
