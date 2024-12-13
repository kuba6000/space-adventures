#pragma once
#include <memory>
namespace GigaGra{

	class Map;

	class Tile
	{
	public:
		Tile();
		~Tile();

		virtual void draw(int x, int y, float frame_delta);
		virtual Tile* clone() const = 0;

		virtual bool isWalkable() const { return true; };
		virtual bool onInteract(Map* map, int x, int y, void* passingData) { return false; };
	};

}
