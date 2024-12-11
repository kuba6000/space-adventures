#pragma once
#include <memory>
namespace GigaGra{

	class Tile
	{
	public:
		Tile();
		~Tile();

		virtual void draw(int x, int y, float frame_delta);
		virtual Tile* clone() const = 0;

		
	};

}
