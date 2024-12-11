#pragma once
#include <SFML/Graphics.hpp>

class Globals
{
public:
	sf::RenderWindow *window = 0;
	float gameWidth = 1366.f;
	float gameHeight = 768.f;
	int gameState = 0;
};

inline Globals g;