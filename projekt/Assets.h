#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Grass.h"
#include "Stone.h"
#include "AluminiumFloor.h"
#include "AluminiumWall.h"
#include "aluminiumdoor.h"
#include "ControlPanel.h"

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
		sf::Texture aluminiumDoorTexture{};
		sf::Texture controlPanelTexture{};
		sf::Texture hand{};
		sf::Texture handPistol{};
		sf::Texture crosshair{};

		Tile* availableTiles[6];
	};



	extern Assets* assets;
}
