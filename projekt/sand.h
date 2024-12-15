#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Sand : public Tile
	{
	public:
		Sand();
		~Sand();
		void draw(int x, int y, float frame_delta) override;
		Sand* clone() const override { return new Sand(*this); };
	};
}

