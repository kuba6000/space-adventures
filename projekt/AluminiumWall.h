#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {

	class AluminiumWall : public Tile
	{
	public:
		AluminiumWall();
		~AluminiumWall();

		void draw(int x, int y, float frame_delta) override;
		bool isWalkable() const override { return false; };
		AluminiumWall* clone() const override { return new AluminiumWall(*this); };
	};

}
