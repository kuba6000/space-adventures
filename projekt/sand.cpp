#include "sand.h"
#include "globals.h"
namespace GigaGra {
	Sand::Sand()
	{
	}
	Sand::~Sand()
	{
	}
	void Sand::draw(int x, int y, float frame_delta)
	{
		// Draw grass
		sf::Sprite sprite{};
		sprite.setTexture(assets->sandTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}