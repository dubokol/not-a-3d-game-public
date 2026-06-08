#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>

class InputHandler {
    std::unordered_map<SDL_Keycode, bool> keyDown;
    bool quit_ = false;

public:
    void update();

    bool isKeyDown(SDL_Keycode key) const;

    bool isQuit() const;
};
