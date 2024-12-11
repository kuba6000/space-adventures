#include "InputHelper.h"
#include "globals.h"

namespace GigaGra {

	std::vector<sf::Keyboard::Key> keysPressed{};
	std::vector<sf::Mouse::Button> mouseButtonsPressed{};

	void InputHelper::processEvents(sf::Event event)
	{
		if (event.type == sf::Event::KeyPressed) {
			keysPressed.push_back(event.key.code);
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			mouseButtonsPressed.push_back(event.mouseButton.button);
		}

	}

	void InputHelper::afterFrame()
	{
		keysPressed.clear();
		mouseButtonsPressed.clear();
	}

	bool InputHelper::wasKeyPressed(sf::Keyboard::Key key)
	{
		return std::find(keysPressed.begin(), keysPressed.end(), key) != keysPressed.end();
	}

	bool InputHelper::wasMousePressed(sf::Mouse::Button button)
	{
		return std::find(mouseButtonsPressed.begin(), mouseButtonsPressed.end(), button) != mouseButtonsPressed.end();
	}

}
