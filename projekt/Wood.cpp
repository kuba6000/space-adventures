#include "Wood.h"
#include "globals.h"
namespace GigaGra {
	Wood::Wood()
	{
	}
	Wood::~Wood()
	{
	}
	void Wood::draw(int x, int y, float frame_delta)
	{
		sf::Sprite sprite{};
		sprite.setTexture(assets->woodTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}