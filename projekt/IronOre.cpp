#include "IronOre.h"
#include "globals.h"
namespace GigaGra {
	IronOre::IronOre()
	{
	}
	IronOre::~IronOre()
	{
	}
	void IronOre::draw(int x, int y, float frame_delta)
	{
		// Draw grass
		sf::Sprite sprite{};
		sprite.setTexture(assets->ironOreTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}