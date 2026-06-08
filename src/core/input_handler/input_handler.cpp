#include <SDL3/SDL.h>
#include "input_handler.h"

void InputHandler::update() {
    for (auto& [key, value] : keyDown) {
        value = false;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (!event.key.repeat) {
                keyDown[event.key.key] = true;
            }
        }

        if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            quit_ = true;
        }
    }
}

bool InputHandler::isKeyDown(SDL_Keycode key) const {
    auto it = keyDown.find(key);
    return it != keyDown.end() && it->second;
}

bool InputHandler::isQuit() const {
    return quit_;
}
