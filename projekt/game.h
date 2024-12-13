#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Map.h"

namespace GigaGra {
	class Game
	{
	public:
		Game();
		~Game();
		void processEvents(sf::Event);
		void update();
		void render(float frame_delta);
		bool isInSpace();
	private:
		sf::View gameView;
		sf::Sprite playerSprite{};
		sf::Sprite handSprite{};
		sf::Sprite handPistolSprite{};
		sf::Vector2f playerPos{};

		Map map{};
		Map map2{};
	};
	extern Game *game;
}

