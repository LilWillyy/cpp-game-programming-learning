#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* mRenderer) {
	if (mTexture) {
		SDL_Rect r;
		// Scale width/height by owner's scale
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// Draw
		SDL_RenderCopyEx(
			mRenderer,		// Renderer
			mTexture,		// Texture to draw
			nullptr,		// Source rectangle
			&r,				// Destination rectangle
			-Math::ToDegrees(mOwner->GetRotation()),		// (Convert angle)
			nullptr,		// Point of rotation
			SDL_FLIP_NONE	// Flip behaviour	
		);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture;
	// Get width / height of texture
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}