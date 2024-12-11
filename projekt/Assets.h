#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Grass.h"
#include "Stone.h"
#include "AluminiumFloor.h"
#include "AluminiumWall.h"

namespace GigaGra {
	class Assets
	{
	public:
		Assets();
		~Assets();

		sf::Texture playerTexture{};
		sf::Texture grassTexture{};
		sf::Texture stoneTexture{};
		sf::Texture aluminiumWallTexture{};
		sf::Texture aluminiumFloorTexture{};

		Tile* availableTiles[4];
	};



	extern Assets* assets;
}
