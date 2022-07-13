#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "BGSpriteComponent.h"
#include "AnimSpriteComponent.h"

Game::Game() :
	mWindow(nullptr),
	mRenderer(nullptr),
	mIsRunning(true),
	mUpdatingActors(false)
{}


bool Game::Initialise() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 2)",
		100,
		100,
		1024,
		768,
		0
	);

	if (!mWindow) {
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer) {
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialise SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;

	// While there are still events in the que
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame() {
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	
	// Update tick count (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update all actors
	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move the actors from mPendingActors to mActors
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// Draw all sprite components
	for (auto sprite : mSprites) {
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData() {
	// Create the player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2{ 100.0f, 384.0f });
	mShip->SetScale(1.5f);

	// Create actor for the background
	// (This doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2{ 512.0f, 284.0f });

	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2{ 1024.0f, 768.0f });
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};

	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);

	// Create skeleton
	Actor* skele = new Actor(this);
	skele->SetPosition(Vector2{ 400.0f, 184.0f });
	skele->SetScale(1.5f);

	AnimSpriteComponent* asc = new AnimSpriteComponent(skele);
	std::vector<SDL_Texture*> skeleWalk = {
		GetTexture("Assets/Skeleton/Character01.png"),
		GetTexture("Assets/Skeleton/Character02.png"),
		GetTexture("Assets/Skeleton/Character03.png"),
		GetTexture("Assets/Skeleton/Character04.png"),
		GetTexture("Assets/Skeleton/Character05.png"),
		GetTexture("Assets/Skeleton/Character06.png"),
	};

	asc->SetAnimTextures(skeleWalk, "Skeleton Walk");

	std::vector<SDL_Texture*> skeleJump = {
		GetTexture("Assets/Skeleton/Character07.png"),
		GetTexture("Assets/Skeleton/Character08.png"),
		GetTexture("Assets/Skeleton/Character09.png"),
		GetTexture("Assets/Skeleton/Character10.png"),
		GetTexture("Assets/Skeleton/Character11.png"),
		GetTexture("Assets/Skeleton/Character12.png"),
		GetTexture("Assets/Skeleton/Character13.png"),
		GetTexture("Assets/Skeleton/Character14.png"),
		GetTexture("Assets/Skeleton/Character15.png"),
	};

	asc->SetAnimTextures(skeleJump, "Skeleton Jump");
}

void Game::UnloadData() {
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty()) {
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures) {
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;

	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);

	if (iter != mTextures.end()) {
		tex = iter->second;
	}
	else {
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());

		if (!surf) {
			SDL_Log("Failed to loadtexture file: %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);

		if (!tex) {
			SDL_Log("Failed to convert surface to texture for: %s", fileName.c_str());
			return nullptr;
		}
	}

	return tex;
}

void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	// If updating actors, add to pending actors
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	// If updating actors, remove from pending actors
	if (mUpdatingActors) {
		auto it = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
		int index = it - mPendingActors.begin();
		mPendingActors.erase(mPendingActors.begin() + index);
	}
	else {
		auto it = std::find(mActors.begin(), mActors.end(), actor);
		int index = it - mActors.begin();
		mActors.erase(mActors.begin() + index);
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	// Find the insertion point in the vector
	// ( The first element with a higher sort order than sprite )
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();

	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	// Insert element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
