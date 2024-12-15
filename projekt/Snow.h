#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Snow : public Tile
	{
	public:
		Snow();
		~Snow();
		void draw(int x, int y, float frame_delta) override;
		Snow* clone() const override { return new Snow(*this); };
	};
}

