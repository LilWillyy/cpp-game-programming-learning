#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game 
{
public:
	Game();
	bool Initialise();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// Load Texture
	SDL_Texture* GetTexture(const std::string& fileName);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// Maps of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// Active actors
	std::vector<class Actor*> mActors;
	// Actors that are added whilst iterating through mActors
	std::vector<class Actor*> mPendingActors;
	// Sprites
	std::vector<class SpriteComponent*> mSprites;

	// Window created by SDL
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	// Are the actors being updated
	bool mUpdatingActors;

	// Game specific
	class Ship* mShip; // Player's ship
};