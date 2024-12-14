#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Grass.h"
#include "Stone.h"
#include "AluminiumFloor.h"
#include "AluminiumWall.h"
#include "aluminiumdoor.h"
#include "ControlPanel.h"
#include "Tree.h"
#include "Wood.h"
#include "Water.h"
#include "Bed.h"

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
		sf::Texture treeTexture{};
		sf::Texture woodTexture{};
		sf::Texture waterTexture{};
		sf::Texture bedTexture{};
		sf::Texture jackTexture{};

		sf::Texture planet1Texture{};


		sf::Texture hand{};
		sf::Texture handPistol{};
		sf::Texture crosshair{};

		Tile* availableTiles[10];
	};



	extern Assets* assets;
}
