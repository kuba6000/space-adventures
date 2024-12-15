#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Ice : public Tile
	{
	public:
		Ice();
		~Ice();
		void draw(int x, int y, float frame_delta) override;
		Ice* clone() const override { return new Ice(*this); };
	};
}

