#pragma once

#include "Common.hpp"
#include "ArrayAny.hpp"

#include <unordered_map>
#include <type_traits>
#include <functional>

class KeyboardController {

public:
    
    struct KeyPress {
        
        SDL_Keycode key;
        enum PressType {
            KeyUp = 0,
            KeyDown = 1
        } pressType;

        inline bool operator==(const KeyPress& other) const {
            return key == other.key && pressType == other.pressType;
        }
    };

    struct KeyPressHash {
        inline std::size_t operator()(const KeyPress& obj) const {
            
           
            return std::hash<int>()(static_cast<int>(obj.key)) ^ std::hash<int>()(static_cast<int>(obj.pressType));
        }
    };
    
    struct Callback {

        std::function<void(ArrayAny*)> func;
        ArrayAny* arr;

        Callback() : func(nullptr), arr(nullptr) {} 

        Callback(const std::function<void(ArrayAny*)>& func, ArrayAny* arr) : func(func), arr(arr) {}
        Callback(const std::function<void(ArrayAny*)>& func) : func(func), arr(nullptr) {}

        ~Callback() {
            if (arr != nullptr) delete arr;
        }

    };

    friend class Callback;

private:
     
    std::unordered_map<KeyPress, Callback, KeyPressHash> keyMap;
    const Uint8* keyboardState;

    Uint8 prevKeyboardState[SDL_NUM_SCANCODES];

    inline auto addKeyCallbackFromVariadic(SDL_KeyCode key, int pressType, const std::function<void(ArrayAny*)>& func) {
        keyMap[KeyPress{key, static_cast<decltype(std::declval<KeyPress>().pressType)>(pressType)}] = Callback{func};
    }

public:

    KeyboardController();
    
    void addKeyCallback(const KeyPress& keyPress, const Callback& callback);
    void addKeyCallback(SDL_Keycode key, int pressType, const std::function<void(ArrayAny*)>& func, ArrayAny* arr = nullptr);
    
    template<typename... TArgs>
    inline void addKeyCallbacks(int pressType, const std::function<void(ArrayAny*)>& func, const TArgs...args) {

        static_assert(std::conjunction_v<std::is_same<TArgs, SDL_KeyCode>...>, "must be SDL_KeyCode");
        (addKeyCallbackFromVariadic(args, pressType, func), ...);

    }


    virtual void update();

    
};
