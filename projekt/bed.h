#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class Bed : public Tile
	{
	public:
		Bed();
		~Bed();
		void draw(int x, int y, float frame_delta) override;
		bool isWalkable() const override { return false; };
		Bed* clone() const override { return new Bed(*this); };
		bool onInteract(Map*, int, int, void*) override;
		std::string getInteractionHint() const override { return "Press E to save"; };
	};
}

