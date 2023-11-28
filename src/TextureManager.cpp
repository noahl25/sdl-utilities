#include "TextureManager.hpp"
#include "Common.hpp"

namespace TextureManager {

	SDL_Texture* loadTexture(const char* filePath) {

		if (SDL_Globals::renderer == nullptr) {
			_ASSERT_FATAL("RENDERER MUST BE INITIALIZED BEFORE TEXTURE ACTIONS ARE TAKEN");
		}

		SDL_Texture* tex = IMG_LoadTexture(SDL_Globals::renderer, filePath);
		if (tex == NULL) {
			_ASSERT(std::string("UNABLE TO LOAD TEXTURE: ") + filePath);
			return nullptr;
		}

		else {
			return tex;
		}

	}

	int renderTexture(SDL_Texture* tex, const SDL_Rect* const src, const SDL_Rect* const dest, const double angle, const SDL_Point *center, const SDL_RendererFlip flip) {

		if (SDL_Globals::renderer == nullptr) {
			_ASSERT_FATAL("RENDERER MUST BE INITIALIZED BEFORE TEXTURE ACTIONS ARE TAKEN");
		}
		
		return SDL_RenderCopyEx(SDL_Globals::renderer, tex, src, dest, angle, center, flip);

	}


}