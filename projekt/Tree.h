#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Tree : public Tile
	{
	public:
		Tree();
		~Tree();
		void draw(int x, int y, float frame_delta) override;
		bool isWalkable() const override { return false; };
		Tree* clone() const override { return new Tree(*this); };
	};
}

