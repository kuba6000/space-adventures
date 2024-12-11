#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
    class Grass : public Tile
    {
	public:
		Grass();
		~Grass();

		void draw(int x, int y, float frame_delta) override;
		Grass* clone() const override { return new Grass(*this); };
    };
}

