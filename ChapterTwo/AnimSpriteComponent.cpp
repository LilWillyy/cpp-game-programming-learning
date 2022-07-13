#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)

{}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mCurrAnimation->size() > 0) {
		// Update the current frame based on the frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		// Wrap current frame if needed
		while (mCurrFrame >= mCurrAnimation->size()) {
			mCurrFrame -= mCurrAnimation->size();
		}

		// Set the texture to current frame (casted to int)
		SetTexture((*mCurrAnimation)[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures, const char* animName) {
	mAnimTextures[animName] = textures;

	if (mAnimTextures.size() == 1) {
		// Set the active texture to the first frame, if this is the first animation
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[animName][0]);
		SetCurrAnimation(animName);
	}
}

/*
AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {
		// Update the current frame based on the frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		// Wrap current frame if needed
		while (mCurrFrame >= mAnimTextures.size()) {
			mCurrFrame -= mAnimTextures.size();
		}


		// Set the texture to current frame (casted to int)
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures) {
	mAnimTextures = textures;

	if (mAnimTextures.size() > 0) {
		// Set the active texture to the first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}
*/