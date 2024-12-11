#pragma once
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class UI
	{
	public:
		UI();
		void showFPS(float fps);
		sf::Font Arial;
		sf::Font Roboto;
	private:
		sf::Text fps_counter{};
	};
	extern UI *ui;
}

