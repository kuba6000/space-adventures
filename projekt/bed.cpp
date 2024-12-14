#include "bed.h"
#include "globals.h"
#include "game.h"
namespace GigaGra {
	Bed::Bed()
	{
	}
	Bed::~Bed()
	{
	}
	void Bed::draw(int x, int y, float frame_delta)
	{
		sf::Sprite sprite{};
		sprite.setTexture(assets->bedTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
	bool Bed::onInteract(Map*, int, int, void*)
	{
		game->save();
		return true;
	}
}