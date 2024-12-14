#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Wood : public Tile
	{
	public:
		Wood();
		~Wood();
		void draw(int x, int y, float frame_delta) override;
		Wood* clone() const override { return new Wood(*this); };
	};
}

