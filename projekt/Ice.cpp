#include "Ice.h"
#include "globals.h"
namespace GigaGra {
	Ice::Ice()
	{
	}
	Ice::~Ice()
	{
	}
	void Ice::draw(int x, int y, float frame_delta)
	{
		// Draw grass
		sf::Sprite sprite{};
		sprite.setTexture(assets->iceTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}