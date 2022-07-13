#include "TileMapComponent.h"
#include <fstream>
#include <string>
#include <sstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
{}

void TileMapComponent::LoadMap(const char* csv_file) {
	std::vector<int> row;
	std::string line, val;

	std::fstream file(csv_file, std::ios::in);
	if (file.is_open()) {
		while (std::getline(file, line)) {
			row.clear();
			std::stringstream str(line);

			while (std::getline(str, val, ',')) {
				row.push_back(std::stoi(val));
			}
			mTileMap->push_back(row);
		}
	}
	else {
		SDL_Log("Could not load TileMap: %s", csv_file);
	}
}