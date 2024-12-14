#include "Water.h"
#include "globals.h"
namespace GigaGra {
	Water::Water()
	{
	}
	Water::~Water()
	{
	}
	void Water::draw(int x, int y, float frame_delta)
	{
		sf::Sprite sprite{};
		sprite.setTexture(assets->waterTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}