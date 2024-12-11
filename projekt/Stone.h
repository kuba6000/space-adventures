#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
    class Stone : public Tile
    {
    public:
		Stone();
		~Stone();
		void draw(int x, int y, float frame_delta) override;
		Stone* clone() const override { return new Stone(*this); };
    };
}

