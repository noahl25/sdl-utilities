#pragma once

#include "Timer.hpp"
#include "Register.hpp"
#include "EventManager.hpp"

class Game {

public:

	Game();
	~Game();

	void init(const char* title, int x, int y, int width, int height, bool fullScreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	inline bool running() {return isRunning;}
	inline void walk() {comp->setAnimationState(std::string("Walk"));}

	SDL_Renderer* renderer;


private:
	
	SDL_Window* window;

	SDL_Event event;

	bool isRunning;

	bool paused;

	bool fullScreen;

	Util::Timer timer;
	Util::Timer::CallbackDelegate* dele;

	Register manager;

	AnimationComponent* comp;
	EventKeyManager keyEventContr;

	ArrayAny mouseState{0, 0};
 
	
};