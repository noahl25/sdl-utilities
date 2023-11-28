#define DEBUG_USE_SIMPLIFIED_NAMES

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <windows.h>

#include "Game.hpp"

int main(int argc, char* args[]){

	Game game;
	game.init("noname", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 840, 600, false);

	const int FPSTarget = 60;
	const int deltaTimeTarget = 1000/FPSTarget;

	Uint32 frameStart;
	int frameTime;

	while (SDL_Globals::running) {

		frameStart = SDL_GetTicks();
		game.handleEvents();
		game.update();
		game.render();

		frameTime = SDL_GetTicks() - frameStart;
 
		if (deltaTimeTarget > frameTime) {

			SDL_Delay(deltaTimeTarget - frameTime);

		}

	}

	game.clean();

	return 0;
}