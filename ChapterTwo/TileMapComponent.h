#pragma once
#include "SpriteComponent.h"
#include <vector>

class TileMapComponent : public SpriteComponent {
public:
	TileMapComponent(Actor* owner, int drawOrder);

	void Draw(SDL_Renderer* renderer) override;
	// Read a csv file and place the values into a 2D array
	void LoadMap(const char* csv_file);

private:
	std::vector<std::vector<int>>* mTileMap;
	SDL_Texture* mTileSet;
};