#include "editor.h"
#include "Utils.h"
#include "globals.h"
#include "UI.h"
#include <string>
#include <format>
#include <iostream>
#include <fstream>

#include "menu.h"

#include "InputHelper.h"
#include "Tile.h"
#include "Grass.h"
#include "Stone.h"
#include "AluminiumWall.h"
#include "AluminiumFloor.h"
#include <Windows.h>

namespace GigaGra {

	Tile* tiles[1000][1000] = { {} };
	Tile* availableTiles[] = { new Grass{}, new Stone{}, new AluminiumFloor{}, new AluminiumWall{} };
	Tile* selectedTile = availableTiles[0];

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
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				g.gameState = 0;
			}

		}
	}
	void Editor::render(float frame_delta)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			editorView.move(0, -10 * frame_delta);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			editorView.move(0, 10 * frame_delta);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			editorView.move(-10 * frame_delta, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			editorView.move(10 * frame_delta, 0);
		}


		g.window->setView(editorView);

		sf::Vector2f viewCenter = editorView.getCenter();
		sf::Vector2f viewSize = editorView.getSize();

		int startX = floor((viewCenter.x - viewSize.x / 2) / 32);
		int startY = floor((viewCenter.y - viewSize.y / 2) / 32);
		int endX = floor((viewCenter.x + viewSize.x / 2) / 32);
		int endY = floor((viewCenter.y + viewSize.y / 2) / 32);

		for (int x = startX; x < endX; x++) {
			for (int y = startY; y < endY; y++) {
				if (x < -500 || y < -500 || x >= 500 || y >= 500) continue;
				if (tiles[x + 500][y + 500])
					tiles[x + 500][y + 500]->draw(x * 32, y * 32, frame_delta);
			}
		}


		sf::Vector2f mousePos = Utils::mousePos();

		int x = floor(mousePos.x / 32) * 32;
		int y = floor(mousePos.y / 32) * 32;

		sf::RectangleShape rect(sf::Vector2f(31, 31));

		// center
		rect.setPosition(0, 0);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(1);
		g.window->draw(rect);

		rect.setPosition(x, y);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1);
		g.window->draw(rect);


		g.window->setView(g.window->getDefaultView());

		sf::Text t{ "", ui->Roboto };

		t.setString(std::format("Grid pos: [{}, {}]", x / 32, y / 32));
		t.setPosition(g.gameWidth - t.getLocalBounds().width - 5, 0);
		g.window->draw(t);
		t.setString(std::format("Grid world pos: [{}, {}]", x, y));
		t.setPosition(g.gameWidth - t.getLocalBounds().width - 5, 25);
		g.window->draw(t);
		t.setString(std::format("Mouse world pos: [{}, {}]", mousePos.x, mousePos.y));
		t.setPosition(g.gameWidth - t.getLocalBounds().width - 5, 50);
		g.window->draw(t);

		// tile selector

		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1);

		int availableTilesCount = sizeof(availableTiles) / sizeof(Tile*);

		int rx = g.gameWidth / 2 - 32 * availableTilesCount;
		int ry = g.gameHeight - 32;

		for (int i = 0; i < availableTilesCount; i++) {
			Tile* tile = availableTiles[i];
			rect.setPosition(rx + 32 * i, ry);
			tile->draw(rx + 32 * i, ry, frame_delta);
			if (tile == selectedTile)
				rect.setOutlineColor(sf::Color::Red);
			else
				rect.setOutlineColor(sf::Color::White);
			g.window->draw(rect);
		}

		if (menu->button({ "Load", ui->Roboto }, { rx + 32.f * (availableTilesCount + 1), (float)ry })) {
			OPENFILENAMEA ofn{};
			ofn.lStructSize = sizeof(OPENFILENAMEA);
			ofn.lpstrFilter = "Map files\0*.map\0";
			ofn.lpstrFile = new char[1000] {0};
			ofn.nMaxFile = 1000;

			if (GetOpenFileNameA(&ofn)) {
				std::cout << ofn.lpstrFile << std::endl;

				std::ifstream file(ofn.lpstrFile);
				if (file.is_open()) {
					for (int x = 0; x < 1000; x++) {
						for (int y = 0; y < 1000; y++) {
							if (tiles[x][y]) delete tiles[x][y];
							unsigned char id;
							file >> id;
							if (id == 0) {
								tiles[x][y] = nullptr;
							}
							else {
								tiles[x][y] = availableTiles[id - 1]->clone();
							}
						}
					}
				}
			}
			
		}
		if (menu->button({ "Save", ui->Roboto }, { rx + 32.f * (availableTilesCount + 1) + 70.f, (float)ry })) {
			OPENFILENAMEA ofn{};
			ofn.lStructSize = sizeof(OPENFILENAMEA);
			ofn.lpstrFilter = "Map files\0*.map\0";
			ofn.lpstrFile = new char[1000] {0};
			ofn.nMaxFile = 1000;

			if (GetOpenFileNameA(&ofn)) {
				std::cout << ofn.lpstrFile << std::endl;
				std::ofstream file(ofn.lpstrFile);
				for (int x = 0; x < 1000; x++) {
					for (int y = 0; y < 1000; y++) {
						if (tiles[x][y]) {
							for (int i = 0; i < availableTilesCount; i++) {
								if (typeid(*tiles[x][y]) == typeid(*availableTiles[i])) {
									file << (unsigned char)(i+1);
									break;
								}
							}
						}
						else {
							file << (unsigned char)0;
						}
					}
				}
			}
		}

		mousePos = Utils::mousePos();
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			int mousebutton = sf::Mouse::isButtonPressed(sf::Mouse::Left) ? 0 : 1;
			if (mousePos.y > ry && mousePos.y < ry + 32) {
				int id = (mousePos.x - rx) / 32;
				if (id >= 0 && id < availableTilesCount) {
					selectedTile = availableTiles[id];
				}
			}
			else {
				if (mousebutton == 0) {
					if (!tiles[x / 32 + 500][y / 32 + 500] || typeid(*tiles[x / 32 + 500][y / 32 + 500]) != typeid(*selectedTile))
					{
						if (tiles[x / 32 + 500][y / 32 + 500])
							delete tiles[x / 32 + 500][y / 32 + 500];
						tiles[x / 32 + 500][y / 32 + 500] = selectedTile->clone();
					}
				}
				else {
					if (tiles[x / 32 + 500][y / 32 + 500])
					{
						delete tiles[x / 32 + 500][y / 32 + 500];
						tiles[x / 32 + 500][y / 32 + 500] = nullptr;
					}
				}
			}
		}

	}


	Editor* editor;
}