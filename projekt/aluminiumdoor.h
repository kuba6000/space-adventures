#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"


namespace GigaGra
{
	class Map;
	class AluminiumDoor : public Tile
	{
	public:
		AluminiumDoor();
		~AluminiumDoor();

		void draw(int x, int y, float frame_delta) override;
		bool isWalkable() const override;
		AluminiumDoor* clone() const override { return new AluminiumDoor(*this); };

		bool onInteract(Map*, int, int, void*) override;
		bool isOpen = false;
	};
}



