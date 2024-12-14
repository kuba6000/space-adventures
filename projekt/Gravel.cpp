#include "Gravel.h"
#include "globals.h"
namespace GigaGra {
	Gravel::Gravel()
	{
	}
	Gravel::~Gravel()
	{
	}
	void Gravel::draw(int x, int y, float frame_delta)
	{
		// Draw grass
		sf::Sprite sprite{};
		sprite.setTexture(assets->gravelTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}