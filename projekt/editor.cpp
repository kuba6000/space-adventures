#include "editor.h"
#include "Utils.h"
#include "globals.h"
#include "UI.h"
#include <string>
#include <format>
#include <iostream>

namespace GigaGra {
	Editor::Editor()
	{
		editorView = g.window->getDefaultView();
		editorView.setCenter(0, 0);
	}
	Editor::~Editor()
	{

	}
	void Editor::processEvents(sf::Event event)
	{

	}
	void Editor::render(float frame_delta)
	{
		g.window->setView(editorView);

		sf::Vector2f mousePos = Utils::mousePos();

		int x = floor(mousePos.x / 32) * 32;
		int y = floor(mousePos.y / 32) * 32;

		sf::RectangleShape rect(sf::Vector2f(32, 32));
		rect.setPosition(x, y);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1);
		g.window->draw(rect);



		g.window->setView(g.window->getDefaultView());

		sf::Text t{"", ui->Roboto };
		
		t.setString(std::format("Grid pos: [{}, {}]", x / 32, y / 32));
		t.setPosition(g.gameWidth - t.getLocalBounds().width - 5, 0);
		g.window->draw(t);
		t.setString(std::format("Grid world pos: [{}, {}]", x, y));
		t.setPosition(g.gameWidth - t.getLocalBounds().width - 5, 25);
		g.window->draw(t);
		t.setString(std::format("Mouse world pos: [{}, {}]", mousePos.x, mousePos.y));
		t.setPosition(g.gameWidth - t.getLocalBounds().width - 5, 50);
		g.window->draw(t);
	}


	Editor* editor;
}