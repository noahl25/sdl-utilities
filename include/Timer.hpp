#pragma once

#include "Common.hpp"
#include <vector>
#include "mwthreading/mingw.thread.h"
#include "mwthreading/mingw.mutex.h"

class CallbackDelegate;

namespace Util {

	class Timer {

		public:

			struct CallbackDelegate {

				friend class Timer;

				private:

					CallbackDelegate(void (*f)(), int bt, int ftd, int ltd, bool lp, bool ofl) : 
						func(f), beginTime(bt), firstTimeDelay(ftd), loopTimeDelay(ltd), loop(lp), onFirstLoop(ofl) {}

					CallbackDelegate(CallbackDelegate* other) {

						func = other->func;
						firstTimeDelay = other->firstTimeDelay;
						beginTime = SDL_GetTicks();
						loopTimeDelay = other->loopTimeDelay;
						loop = other->loop;
						onFirstLoop = other->onFirstLoop;
						
					}
					
					void (*func)();
					int beginTime;
					int firstTimeDelay;
					int loopTimeDelay;
					bool loop;
					bool onFirstLoop;

			};

		private:

			std::vector<int> activeStopwatches;
			std::vector<CallbackDelegate*> activeCallbacks;
			std::thread timerThread;

		public:

			Timer();
			~Timer();
			uint8_t beginStopwatch() {
				activeStopwatches.emplace_back(SDL_GetTicks());
				return activeStopwatches.size();
			}
			uint8_t beginWithCallback(CallbackDelegate* delegate);
			inline int getTime(uint8_t id) {return SDL_GetTicks() - activeStopwatches.at(id); return 1;}
			inline int endStopwatch(uint8_t id) {activeStopwatches.erase(activeStopwatches.begin() + id); return 1;}

			void update();
			[[nodiscard]] Util::Timer::CallbackDelegate* makeNewLoopedDelegate(void (*func)(), int firstTimeDelay, int loopTimeDelay=-1);
			[[nodiscard]] Util::Timer::CallbackDelegate* makeNewDelegate(void (*func)(), int delay);

		
	};

}

