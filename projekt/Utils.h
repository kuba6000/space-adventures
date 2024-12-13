#pragma once
#include <SFML/Graphics.hpp>
#include <stdlib.h>
namespace GigaGra {
	namespace Utils {
#define PI 3.14159265359f
#define RAD_TO_DEG(a) ((a) * 180.f / PI)
#define DEG_TO_RAD(a) ((a) * PI / 180.f)

		float randomFloat(float min, float max);
		sf::Vector2f mousePos();
	}
}
