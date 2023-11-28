#include "Common.hpp"

#include "Game.hpp"
#include "TextureManager.hpp"
#include "Entity.hpp"
#include <unordered_map>
#include <ArrayAny.hpp>

static void func(ArrayAny*) {
	std::cout << "hi";
}

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int x, int y, int width, int height, bool fullScreen) {

	using namespace SDL_Globals;
	paused = false;
	TEX_WIDTH_PIXELS = 32;
	TEX_HEIGHT_PIXELS = 32;

	int flags;
	if (fullScreen) {flags = SDL_WINDOW_FULLSCREEN;}
	else {flags = 0;}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		_ASSERT("SDL SUCCESSFULLY INITIALIZED");
		window = SDL_CreateWindow(title, x, y, width, height, flags);
		if (window) {
			_ASSERT("WINDOW INITIALIZED");
		}
 		
 		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_Globals::renderer = renderer;
			_ASSERT("RENDERER INITIALIZED");
		}
		if ( TTF_Init() >= 0 ) {
			_ASSERT("TTF INITIALIZED");
		}

		SDL_Globals::running = true;
	}

	else {
		SDL_Globals::running = false;
		_ASSERT_FATAL("SDL FAILED TO INITIALIZE");
	}

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	fullScreen = true;

	ID ent = manager.addEntity();
	SDL_Texture* main = TextureManager::loadTexture("res/assets/test.png");

	std::unordered_map<std::string, AnimationComponent::Animation> animations;
	animations[std::string("Idle")] = AnimationComponent::Animation(main, 12, 12);
	animations[std::string("Attack")] = AnimationComponent::Animation(main, 7, 5);
	animations[std::string("Walk_R")] = AnimationComponent::Animation(main, 7, 5, SDL_Globals::TEX_HEIGHT_PIXELS);
	animations[std::string("Walk_L")] = AnimationComponent::Animation(main, 7, 5, SDL_Globals::TEX_HEIGHT_PIXELS, SDL_FLIP_HORIZONTAL);
	comp = manager.getEntityByID(ent)->addComponent<AnimationComponent>(std::move(animations), Vector2D{0, 0}, Vector2D{0, 0}, SDL_Rect{0, 0, 128, 128});
	comp->setAnimationState(std::string("Idle"));

	keyEventContr.addKeyCallback(KeyboardController::KeyPress{SDLK_d, KeyboardController::KeyPress::KeyDown}, KeyboardController::Callback{ [this] (ArrayAny*) mutable -> void { this->comp->setAnimationStateIfNotActive(std::string("Walk_R")); this->comp->vel.x = 2; }});
	keyEventContr.addKeyCallback(KeyboardController::KeyPress{SDLK_a, KeyboardController::KeyPress::KeyDown}, KeyboardController::Callback{ [this] (ArrayAny*) mutable -> void { this->comp->setAnimationStateIfNotActive(std::string("Walk_L")); this->comp->vel.x = -2;}});
	keyEventContr.addKeyCallbacks(KeyboardController::KeyPress::KeyUp, [this] (ArrayAny*) mutable -> void { this->comp->setAnimationStateIfNotActive(std::string("Idle")); this->comp->vel.x = 0; }, SDLK_d, SDLK_a);

	
}

void Game::handleEvents() {

	keyEventContr.update();
	
}

void Game::update() {

	if (!paused) {

		
 
	
	}

}

void Game::render() {

	SDL_RenderClear(renderer);
 
	manager.update();

	SDL_RenderPresent(renderer);

}

void Game::clean() {

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	_ASSERT("GAME DESTROYED");

}

