#include "Timer.hpp"

#include <chrono>
#include <thread>
#include <mutex>

Util::Timer::Timer() {

	timerThread = std::thread(&Timer::update, this);

}

Util::Timer::~Timer() {

	if (timerThread.joinable()) {
		timerThread.join();
	}

}

[[nodiscard]] Util::Timer::CallbackDelegate* Util::Timer::makeNewLoopedDelegate(void (*func)(), int firstTimeDelay, int loopTimeDelay) {

	if (loopTimeDelay == -1) {
		loopTimeDelay = firstTimeDelay;
	}

	return new CallbackDelegate(func, SDL_GetTicks(), firstTimeDelay, loopTimeDelay, true, true);
}

[[nodiscard]] Util::Timer::CallbackDelegate* Util::Timer::makeNewDelegate(void (*func)(), int delay) {

	return new CallbackDelegate(func, SDL_GetTicks(), delay, 0, false, true);

}

void Util::Timer::update() {

	while (true) {

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		std::vector<int> deprecated;

		for (int i = 0; i < activeCallbacks.size(); i++) {
			
			CallbackDelegate* delegate = activeCallbacks[i];
			int del = (delegate->onFirstLoop ? delegate->firstTimeDelay : delegate->loopTimeDelay);
			
			if (SDL_GetTicks() - delegate->beginTime >= del) {

				delegate->func();

				if (delegate->loop) {
					delegate->beginTime = SDL_GetTicks();
					delegate->onFirstLoop = false;
				}

				else {
					deprecated.push_back(i);
				}

			}

		}

		for (int i = deprecated.size() - 1; i >= 0; i--) {;
	        activeCallbacks.erase(activeCallbacks.begin() + deprecated[i]);

	    }

	}
}


uint8_t Util::Timer::beginWithCallback(CallbackDelegate* delegate) {

	activeCallbacks.push_back(delegate);
	return activeCallbacks.size() - 1;

}