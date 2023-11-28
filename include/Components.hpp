#pragma once

#include "Vector2D.hpp"
#include "Common.hpp"
#include <unordered_map>

class Component {

public:

	enum Type {

		None              = 0,
		PositionComponent = 2,
		KeyboardComponent = 4,
		TextureComponent  = 8,
		PhysicsComponent  = 16

	};

public:

	virtual void update() = 0;
	virtual inline int getType() {
		return Component::Type::None;;
	}
	uint8_t id;

};

class PositionComponent : public Component {

public:

	static const int type = Component::Type::PositionComponent;

public:

	PositionComponent();
	PositionComponent(const Vector2D& loc, const Vector2D& vel);

	virtual void update() override;
	virtual inline int getType() override {
		return Component::Type::PositionComponent;
	}

	Vector2D loc;
	Vector2D vel;

};

class TextureComponent : public PositionComponent {

public:

	static const int type = Component::Type::PositionComponent | Component::Type::TextureComponent;

public:

	TextureComponent();
	TextureComponent(SDL_Texture* tex, const SDL_Rect& texBounds, int texW, int texH);
	TextureComponent(SDL_Texture* tex, const SDL_Rect& texBounds, int texW, int texH, const Vector2D& loc, const Vector2D& vel, const SDL_Rect& bounds);

	virtual void update() override;
	virtual inline int getType() override {
		return Component::Type::PositionComponent | Component::Type::TextureComponent;;
	}


	SDL_Texture* tex;
	SDL_Rect texBounds;
	SDL_Rect texSrc;

};

class AnimationComponent : public PositionComponent {

public:

	struct Animation {

	private:

		int f; //num of frames since done

	public:

		SDL_Texture* animSheet;
		SDL_Rect texSrc;
		int animSheetX;
		int fpu; 

		int frameCounter = 0;

		bool done = false;
		int delayFrames;

		SDL_RendererFlip flip;


		Animation();
		Animation(SDL_Texture* animSheet, int animSheetX, int fpu, int y = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, int texW = SDL_Globals::TEX_WIDTH_PIXELS, int texH = SDL_Globals::TEX_HEIGHT_PIXELS, int delayFrames = 0);

		void update();
		inline void reset() {frameCounter = 0; done = false; texSrc.x = 0;};
	
	};

	static const int type = Component::Type::PositionComponent | Component::Type::TextureComponent;

private:

	SDL_Rect bounds;
	std::unordered_map<std::string, AnimationComponent::Animation> animations;

	std::string activeAnimation;
	std::string underlyingAnimation;

	bool playingSingular;
	

public:

	AnimationComponent();
	AnimationComponent(std::unordered_map<std::string, AnimationComponent::Animation>&& animations);
	AnimationComponent(std::unordered_map<std::string, AnimationComponent::Animation>&& animations, const Vector2D& loc, const Vector2D& vel, const SDL_Rect& bounds);

	inline void setAnimationState(const std::string&& anim) { 
		animations[activeAnimation].reset();
		activeAnimation = std::move(anim); 
	}

	inline bool setAnimationStateIfNotActive(const std::string&& anim) {

		if (activeAnimation != anim) {
			animations[activeAnimation].reset();
			activeAnimation = std::move(anim); 
			return true;
		}

		return false;

	}


	inline std::string& getAnimationState() {
		return activeAnimation;
	}
	virtual void update() override;
	virtual inline int getType() override {
		return Component::Type::PositionComponent | Component::Type::TextureComponent;
	}

	void playAnimation(const std::string&& anim);


};

class PhysicsComponent : public PositionComponent {

	Vector2D& loc; //reference to vector from positioncomponent (or not idrc, i cant stop you)
	SDL_Rect bounds;
	bool active;

	virtual inline int getType() override {
		return Component::Type::PhysicsComponent;
	}

	virtual void update() override;

private:

	void nextFrame();

};