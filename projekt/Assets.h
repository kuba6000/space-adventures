#pragma once
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Assets
	{
	public:
		Assets();
		~Assets();

		sf::Texture ziomalTexture{};
		sf::Texture grassTexture{};
		sf::Texture stoneTexture{};
		sf::Texture aluminiumWallTexture{};
		sf::Texture aluminiumFloorTexture{};
	};



	extern Assets* assets;
}
