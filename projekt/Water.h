#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Water : public Tile
	{
	public:
		Water();
		~Water();
		void draw(int x, int y, float frame_delta) override;
		bool isWalkable() const override { return false; };
		Water* clone() const override { return new Water(*this); };
	};
}

