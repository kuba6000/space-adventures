#include "Tree.h"
#include "globals.h"
namespace GigaGra {
	Tree::Tree()
	{
	}
	Tree::~Tree()
	{
	}
	void Tree::draw(int x, int y, float frame_delta)
	{
		sf::Sprite sprite{};
		sprite.setTexture(assets->treeTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
}