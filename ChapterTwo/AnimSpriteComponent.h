#pragma once
#include "SpriteComponent.h"
#include <map>
#include <string>
#include <vector>

class AnimSpriteComponent : public SpriteComponent 
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation (overriden from component)
	void Update(float deltaTime) override;
	// Set the textures used for an animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures, const char* animName);
	// Set/Get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; };
	void SetAnimFPS(float fps) { mAnimFPS = fps; };
	// Set the current animation
	void SetCurrAnimation(std::string animName) { mCurrAnimation = &mAnimTextures[animName]; };
private:
	// All textures in the animation
	std::map<std::string, std::vector<SDL_Texture*>> mAnimTextures;
	// Current animation
	std::vector<SDL_Texture*>* mCurrAnimation;
	// Current frame displayed
	float mCurrFrame;
	// Animation framerate
	float mAnimFPS;
};



/*
class AnimSpriteComponent : public SpriteComponent 
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation (overriden from component)
	void Update(float deltaTime) override;
	// Set the textures used for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// Set/Get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; };
	void SetAnimFPS(float fps) { mAnimFPS = fps; };

private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	// Current frame displayed
	float mCurrFrame;
	// Animation framerate
	float mAnimFPS;
};
*/