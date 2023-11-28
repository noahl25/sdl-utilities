#include "KeyboardController.hpp"
#include <utility>

KeyboardController::KeyboardController() {
    keyboardState = SDL_GetKeyboardState(NULL);
}

void KeyboardController::addKeyCallback(const KeyPress& keyPress, const Callback& callback) {
    keyMap[keyPress] = callback;
}

void KeyboardController::addKeyCallback(SDL_Keycode key, int pressType, const std::function<void(ArrayAny*)>& func, ArrayAny *arr)
{

    keyMap[KeyPress{key, static_cast<decltype(std::declval<KeyPress>().pressType)>(pressType)}] = Callback{func, arr};

}

void KeyboardController::update() {

    SDL_PumpEvents();

    std::unordered_map<KeyPress, Callback, KeyPressHash>::iterator it;
    for (it = keyMap.begin(); it != keyMap.end(); it++)
    {
        SDL_Scancode code = SDL_GetScancodeFromKey(it->first.key);

        if (keyboardState[code] && it->first.pressType == KeyPress::KeyDown) {
            it->second.func(it->second.arr);
        }

        if (!keyboardState[code] && prevKeyboardState[code] && it->first.pressType == KeyPress::KeyUp) {
            it->second.func(it->second.arr);
        }
    }

    memcpy(prevKeyboardState, keyboardState, SDL_NUM_SCANCODES);

}
