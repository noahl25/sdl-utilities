#pragma once

#include <atomic>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Debug.hpp"


#define OUT

namespace SDL_Globals {

	inline SDL_Renderer* renderer = nullptr;
	inline int TEX_WIDTH_PIXELS, TEX_HEIGHT_PIXELS;
	inline std::atomic<bool> running = false;

	//TODO: engine components run on this thread?
	//inline std::thread engineThread;

}

struct ID {

	size_t id;
	ID(size_t id) : id(id) {}
	ID(const ID& other) : id(other.id) {}
	ID() : id(0) {}
	
	inline size_t operator()() {
		return id;
	}


};

