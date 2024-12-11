#pragma once
#include "Tile.h"

namespace GigaGra {

    class AluminiumFloor : public Tile
    {
    public:
		AluminiumFloor();
		~AluminiumFloor();

		void draw(int x, int y, float frame_delta) override;

        AluminiumFloor* clone() const override { return new AluminiumFloor(*this); };
    };

}
