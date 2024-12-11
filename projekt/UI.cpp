#include "UI.h"
#include "globals.h"
namespace GigaGra {
	UI::UI()
	{
		Arial = sf::Font();
		Arial.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
		Roboto = sf::Font();
		Roboto.loadFromFile("assets\\Roboto-Regular.ttf");
	}
	void UI::showFPS(float fps)
	{
		fps_counter.setString("FPS: " + std::to_string(fps));
		fps_counter.setFont(Arial);
		g.window->setView(g.window->getDefaultView());

		fps_counter.setPosition(0, 0);
		fps_counter.setString("FPS: " + std::to_string(fps));
		g.window->draw(fps_counter);
	}

	UI *ui;
}