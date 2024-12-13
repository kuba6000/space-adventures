#include "aluminiumdoor.h"
#include "Assets.h"
#include "globals.h"
#include "Map.h"
#include "game.h"

namespace GigaGra {
	AluminiumDoor::AluminiumDoor()
	{
	}
	AluminiumDoor::~AluminiumDoor()
	{
	}
	void AluminiumDoor::draw(int x, int y, float frame_delta)
	{
		if (game->isInSpace())
			isOpen = false;
		sf::Sprite sprite{};
		if (isOpen)
			sprite.setTexture(assets->aluminiumFloorTexture);
		else
			sprite.setTexture(assets->aluminiumDoorTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
	bool AluminiumDoor::isWalkable() const
	{
		return isOpen;
	}
	bool AluminiumDoor::onInteract(Map* map, int x, int y, void* passingData)
	{
		if (game->isInSpace())
			return false;
		isOpen = !isOpen;
		if (!passingData) {
			for (int lx = -1; lx <= 1; lx++) {
				for (int ly = -1; ly <= 1; ly++) {
					if (lx == 0 && ly == 0)
						continue;
					Tile* tile = map->getTile(x + lx, y + ly);
					if (!tile)
						continue;
					if (typeid(AluminiumDoor) != typeid(*tile))
						continue;
					tile->onInteract(map, x + lx, y + lx, this);
				}
			}
		}
		return true;
	}
}