#include "Grass.h"
#include "globals.h"
namespace GigaGra {
	Grass::Grass()
	{
	}
	Grass::~Grass()
	{
	}
	void Grass::draw(int x, int y, float frame_delta)
	{
		// Draw grass
		sf::Sprite sprite{};
		sprite.setTexture(assets->grassTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}