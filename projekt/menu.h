#pragma once
#include <SFML/Graphics.hpp>

namespace GigaGra
{
	class Menu
	{
	public:
		Menu();
		~Menu();
		void processEvents(sf::Event);
		void update();
		void render(float frame_delta);
	private:
		int menuState = 0;
		int rndSeed = 0;
		sf::View backgroundView;

		bool button(sf::Text text, sf::Vector2f pos);
		bool buttonCentered(sf::Text text, sf::Vector2f pos);
		bool checkBox(sf::Text text, sf::Vector2f pos, bool *checked);
		bool checkBoxCentered(sf::Text text, sf::Vector2f pos, bool* checked);
		bool sliderInt(sf::Text text, sf::Vector2f pos, int* value, int min, int max);
		bool silderIntCentered(sf::Text text, sf::Vector2f pos, int* value, int min, int max);
	};
	extern Menu *menu;
}


