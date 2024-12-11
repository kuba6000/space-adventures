#include "AluminiumWall.h"
#include "globals.h"

namespace GigaGra {

	AluminiumWall::AluminiumWall()
	{


	}

	AluminiumWall::~AluminiumWall()
	{
	}

	void AluminiumWall::draw(int x, int y, float frame_delta)
	{
		sf::Sprite sprite{};
		sprite.setTexture(assets->aluminiumWallTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}

}
