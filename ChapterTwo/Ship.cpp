#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Ship::Ship(Game* game)
	: Actor(game)
	, mRightSpeed(0.0f)
	, mDownSpeed(0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
		game->GetTexture("Assets/Ship03.png"),
		game->GetTexture("Assets/Ship04.png")
	};
	asc->SetAnimTextures(anims, "Ship Fly");
}

void Ship::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	// Update position based on speed and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;

	// Limit movement to the left half of the screen
	if (pos.x < 25.0f) {
		pos.x = 25.0f;
	}
	else if (pos.x > 500.0f) {
		pos.x = 500.0f;
	}
	else if (pos.y < 25.0f) {
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f) {
		pos.y = 743.0f;
	}

	// Set position
	SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state) {
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;

	// right/left
	if (state[SDL_SCANCODE_D]) {
		mRightSpeed += 250.0f;
	}
	
	if (state[SDL_SCANCODE_A]) {
		mRightSpeed -= 250.0f;
	}

	if (state[SDL_SCANCODE_S]) {
		mDownSpeed += 250.0f;
	}
	
	if (state[SDL_SCANCODE_W]) {
		mDownSpeed -= 250.0f;
	}
}