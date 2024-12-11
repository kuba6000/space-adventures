#include "Utils.h"
#include "globals.h"

namespace GigaGra {
	float Utils::randomFloat(float min, float max) {
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	sf::Vector2f Utils::mousePos() {
		return g.window->mapPixelToCoords(sf::Mouse::getPosition(*g.window));
	}


}