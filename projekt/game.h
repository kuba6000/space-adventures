#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"

namespace GigaGra {
	class Game
	{
	public:
		Game();
		~Game();
		void processEvents(sf::Event);
		void update();
		void render(float frame_delta);
	private:
		sf::View gameView;
		sf::Sprite playerSprite{};
		sf::Vector2f playerPos{};

		Tile* map[1000][1000]{};
	};
	extern Game *game;
}

