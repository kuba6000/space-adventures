#pragma once
#include <SFML/Graphics.hpp>
#include <stdlib.h>
namespace GigaGra {
	namespace InputHelper {
		void processEvents(sf::Event event);
		void afterFrame();

		bool wasKeyPressed(sf::Keyboard::Key key);
		bool wasMousePressed(sf::Mouse::Button button);
	}
}