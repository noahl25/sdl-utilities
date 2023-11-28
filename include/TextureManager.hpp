#pragma once

#include "Common.hpp"

namespace TextureManager {

	SDL_Texture* loadTexture(const char* filePath);
	int renderTexture(SDL_Texture* tex, const SDL_Rect* const src, const SDL_Rect* const dest, const double angle = NULL, const SDL_Point* center = NULL, const SDL_RendererFlip flip = SDL_FLIP_NONE);

}