#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Krzak : public Tile
	{
	public:
		Krzak();
		~Krzak();
		void draw(int x, int y, float frame_delta) override;
		Krzak* clone() const override { return new Krzak(*this); };
	};
}

