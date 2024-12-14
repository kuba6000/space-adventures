#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class IronOre : public Tile
	{
	public:
		IronOre();
		~IronOre();

		void draw(int x, int y, float frame_delta) override;
		IronOre* clone() const override { return new IronOre(*this); };
	};
}

