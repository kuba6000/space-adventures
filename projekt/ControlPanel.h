#pragma once
#include "Tile.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

namespace GigaGra {
	class ControlPanel : public Tile
	{
	public:
		ControlPanel();
		~ControlPanel();

		void draw(int x, int y, float frame_delta) override;
		ControlPanel* clone() const override { return new ControlPanel(*this); };
		bool onInteract(Map*, int, int, void*) override;
	};
}