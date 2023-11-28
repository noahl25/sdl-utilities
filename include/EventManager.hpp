#pragma once

#include "Common.hpp"
#include "KeyboardController.hpp"
#include "ArrayAny.hpp"
#include <unordered_map>

class EventManager {

private:

    struct EventTypeHash {

        std::size_t operator()(const Uint8& eventType) const {
            return std::hash<std::size_t>()(static_cast<std::size_t>(eventType));
        }

    };

    SDL_Event event;
    

public: 

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


    std::unordered_map<Uint8, Callback, EventTypeHash> eventMap;

public:

    virtual void update();
    void addEventCall(Uint8 event, const Callback& call);



    
};

class EventKeyManager : public EventManager, public KeyboardController {

public:

    void update() override;
    EventKeyManager();


};

typedef EventKeyManager KeyEventManager;