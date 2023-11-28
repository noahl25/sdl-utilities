#include "EventManager.hpp"

void EventManager::update()
{
    SDL_PollEvent(&event);

    switch (event.type) {

        case SDL_QUIT:
            SDL_Globals::running = false;

        if (auto call = eventMap.find(event.type) ; call != eventMap.end()) {
            
            call->second.func(call->second.arr);

        } 

    }

}
void EventManager::addEventCall(Uint8 event, const Callback& call)
{
    eventMap[event] = call;
}

EventKeyManager::EventKeyManager() : KeyboardController(), EventManager()
{
}

void EventKeyManager::update()
{
    KeyboardController::update();
    EventManager::update();
}

