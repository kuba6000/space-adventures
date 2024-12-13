#include "ControlPanel.h"
#include "game.h"
#include "globals.h"

namespace GigaGra {
	ControlPanel::ControlPanel()
	{
	}
	ControlPanel::~ControlPanel()
	{
	}
	void ControlPanel::draw(int x, int y, float frame_delta)
	{
		sf::Sprite sprite{};
		sprite.setTexture(assets->controlPanelTexture);
		sprite.setPosition(x, y);
		g.window->draw(sprite);
	}
	bool ControlPanel::onInteract(Map*, int, int, void*)
	{
		

		return true;
	}
}