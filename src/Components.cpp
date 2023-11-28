#include "Components.hpp"
#include "TextureManager.hpp"

PositionComponent::PositionComponent() : loc({0, 0}), vel({0, 0}) {}
PositionComponent::PositionComponent(const Vector2D& loc, const Vector2D& vel) : loc(loc), vel(vel) {}

void PositionComponent::update() {

	loc += vel;

}

TextureComponent::TextureComponent() : PositionComponent(), tex(nullptr), texBounds({0, 0, 0, 0}), texSrc({0, 0, 0, 0}) {}
TextureComponent::TextureComponent(SDL_Texture* tex, const SDL_Rect& texBounds, int texW, int texH) : PositionComponent(), tex(tex), texBounds(texBounds), texSrc({0, 0, texW, texH}) {}
TextureComponent::TextureComponent(SDL_Texture* tex, const SDL_Rect& texBounds, int texW, int texH, const Vector2D& loc, const Vector2D& vel, const SDL_Rect& bounds) : PositionComponent(loc, vel), tex(tex), texBounds(texBounds), texSrc({0, 0, texW, texH}) {}

void TextureComponent::update() {

	PositionComponent::update();
	TextureManager::renderTexture(tex, &texSrc, &texBounds);

}

AnimationComponent::AnimationComponent() {}
AnimationComponent::AnimationComponent(std::unordered_map<std::string, AnimationComponent::Animation>&& animations) : animations(std::move(animations)) {}
AnimationComponent::AnimationComponent(std::unordered_map<std::string, AnimationComponent::Animation>&& animations, const Vector2D& loc, const Vector2D& vel, const SDL_Rect& bounds) : PositionComponent(loc, vel), animations(std::move(animations)), bounds(bounds) {}

AnimationComponent::Animation::Animation() : animSheet(nullptr) {}
AnimationComponent::Animation::Animation(SDL_Texture* animSheet, int animSheetX, int fpu, int y, SDL_RendererFlip flip, int texW, int texH, int delayFrames) : animSheet(animSheet), animSheetX(animSheetX), texSrc(SDL_Rect{0, y, texW, texH}), fpu(fpu), flip(flip), delayFrames(delayFrames) {}

void AnimationComponent::update() {

	PositionComponent::update();

	bounds.x = loc.x;
	bounds.y = loc.y;

	if (!playingSingular) {
		Animation& a = animations[activeAnimation];
		a.update();
		TextureManager::renderTexture(a.animSheet, &a.texSrc, &bounds, 0, NULL, a.flip);
	}

	else {

		Animation& a = animations[activeAnimation];
		a.update();
		TextureManager::renderTexture(a.animSheet, &a.texSrc, &bounds, 0, NULL, a.flip);

		if (a.done) {
			activeAnimation = std::move(underlyingAnimation);
			underlyingAnimation = "";
			playingSingular = false;

			a.reset();
		}

	}
}

void AnimationComponent::Animation::update() {

	frameCounter++;

	if (delayFrames > 0 && done) {
		f++;
		if (f == delayFrames) {
			done = false;
			f = 0;
		}
		return;
	}

	if (frameCounter % fpu == 0) {
		texSrc.x += texSrc.w;

		if (texSrc.x >= animSheetX * texSrc.w) {

			texSrc.x = 0;
			frameCounter = 0;
			done = true;

		}
	}


}

void AnimationComponent::playAnimation(const std::string&& anim) {

	if (!playingSingular) { 
		underlyingAnimation = std::move(activeAnimation);
		activeAnimation = std::move(anim);
		playingSingular = true;
	}

}