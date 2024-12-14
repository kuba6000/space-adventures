#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Gravel : public Tile
	{
	public:
		Gravel();
		~Gravel();

		void draw(int x, int y, float frame_delta) override;
		Gravel* clone() const override { return new Gravel(*this); };
	};
}

