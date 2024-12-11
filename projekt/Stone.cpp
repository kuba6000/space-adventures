#include "Stone.h"
#include "globals.h"
namespace GigaGra {
	Stone::Stone()
	{
	}
	Stone::~Stone()
	{
	}
	void Stone::draw(int x, int y, float frame_delta)
	{
		sf::Sprite sprite{};
		sprite.setTexture(assets->stoneTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}