#include "krzak.h"
#include "globals.h"
namespace GigaGra {
	Krzak::Krzak()
	{
	}
	Krzak::~Krzak()
	{
	}
	void Krzak::draw(int x, int y, float frame_delta)
	{
		// Draw grass
		sf::Sprite sprite{};
		sprite.setTexture(assets->krzakTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}