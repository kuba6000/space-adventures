#pragma once
#include <SFML/Graphics.hpp>

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
		sf::Sprite ziomalSprite{};
		sf::Vector2f ziomalPos{};

		sf::Texture ziomalTexture{};
		sf::Texture grassTexture{};
		sf::Texture stoneTexture{};
		sf::Sprite grassSprite{};
		sf::Sprite stoneSprite{};
	};
	extern Game *game;
}

