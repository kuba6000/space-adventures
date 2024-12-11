#include "AluminiumFloor.h"
#include "Assets.h"
#include "globals.h"

namespace GigaGra {
	AluminiumFloor::AluminiumFloor()
	{
	}
	AluminiumFloor::~AluminiumFloor()
	{
	}
	void AluminiumFloor::draw(int x, int y, float frame_delta)
	{
		sf::Sprite sprite{};
		sprite.setTexture(assets->aluminiumFloorTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}