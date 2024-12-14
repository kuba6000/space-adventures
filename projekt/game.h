#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Map.h"

namespace GigaGra {

	struct PlayerData {
		int hp = 100;
		int coins = 0;
		int parts1 = 0;
		int parts2 = 0;
		int parts3 = 0;
		int parts4 = 0;
	};

	class Game
	{
	public:
		Game();
		~Game();
		void processEvents(sf::Event);
		void update();
		void render(float frame_delta);
		bool isInSpace();
		void openControlPanel();
		void save();
		bool load();
		void pushFloatingText(sf::Text, sf::Vector2f, float);
	private:
		sf::View gameView;
		sf::Sprite playerSprite{};
		sf::Sprite handSprite{};
		sf::Sprite handPistolSprite{};
		sf::Vector2f playerPos{};
		PlayerData playerData{};

		std::vector<std::pair<sf::Text, float>> floatingTexts{};

		sf::Sprite npcSprite{};

		void travelTo(int);

		Map map{};
		Map map2{};
		Map map3{};
		Map map4{};
	};
	extern Game *game;
}

