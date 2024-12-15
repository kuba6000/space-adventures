#include "Snow.h"
#include "globals.h"
namespace GigaGra {
	Snow::Snow()
	{
	}
	Snow::~Snow()
	{
	}
	void Snow::draw(int x, int y, float frame_delta)
	{
		// Draw grass
		sf::Sprite sprite{};
		sprite.setTexture(assets->snowTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}