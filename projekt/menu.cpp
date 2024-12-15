#include "menu.h"
#include "globals.h"
#include "UI.h"
#include "Utils.h"
#include "Settings.h"
#include "InputHelper.h"
#include <iostream>
#include "game.h"
#include <Windows.h>

namespace GigaGra {
	Menu::Menu()
	{
		rndSeed = time(0);
		backgroundView = g.window->getDefaultView();
	}

	Menu::~Menu()
	{

	}

	void Menu::processEvents(sf::Event event)
	{
	}

	void Menu::update()
	{
	}

	void Menu::render(float frame_delta)
	{
		sf::Font& font = ui->Roboto;
		g.window->setView(backgroundView);
		srand(rndSeed);
		sf::Vector2f backgroundSize = backgroundView.getSize();
		sf::Vector2f backgroundCenter = backgroundView.getCenter();
		sf::FloatRect backgroundRect = sf::FloatRect(backgroundCenter.x - backgroundSize.x / 2.f, backgroundCenter.y - backgroundSize.y / 2.f, backgroundSize.x, backgroundSize.y);
		for (int x = 0; x < g.gameWidth / 30 * 2; x++) {
			for (int y = 0; y < g.gameWidth / 30 * 2; y++) {
				if (rand() % 100 < 3) {
					if (backgroundRect.intersects(sf::FloatRect{x * 30.f, y * 30.f, 30.f, 30.f})) {
						sf::CircleShape shape(Utils::randomFloat(1.f, 15.f));
						shape.setFillColor(sf::Color{ (sf::Uint8)(rand() % 256), (sf::Uint8)(rand() % 256), (sf::Uint8)(rand() % 256) });
						shape.setPosition(x * 30.f, y * 30.f);
						g.window->draw(shape);
					}
					else {
						rand(); rand(); rand(); rand();
					}
				}
			}
		}
		backgroundView.move(1.f * frame_delta, 1.f * frame_delta);
		if (backgroundRect.left > g.gameWidth * 1.5f) {
			rndSeed = time(0);
			backgroundView.setCenter(g.gameWidth / 2, g.gameHeight / 2);
		}

		g.window->setView(g.window->getDefaultView());

		sf::Text title{ "Space Adventures", font };
		title.setCharacterSize(48);
		title.setLetterSpacing(2);
		title.setFillColor(sf::Color::White);
		title.setStyle(sf::Text::Bold);
		sf::FloatRect titleRect = title.getLocalBounds();

		sf::Text t{"Gg", font };
		sf::FloatRect r = t.getLocalBounds();

		float h = r.height;

		float x = g.gameWidth / 2;
		float y = g.gameHeight / 2 - ((h + 5) * 4) + titleRect.height;

		title.setPosition(x - titleRect.width / 2, y - titleRect.height - 10);
		g.window->draw(title);

		if (menuState == 0) {

			if (buttonCentered({ "Continue", font }, { x, y += h + 5 })) {
				if (!game->load()) {
					MessageBox(nullptr, L"No savefile", L"Error", MB_OK);
				}
				else {
					g.gameState = 1;
				}
			}
			if (buttonCentered({ "New game", font }, { x, y += h + 5 })) {
				game->newGame();
				g.gameState = 1;
			}
			if (buttonCentered({ "Settings", font }, { x, y += h + 5 })) {
				menuState = 1;
			}
			if (buttonCentered({ "Map Editor", font }, { x, y += h + 5 })) {
				g.gameState = 2;
			}
			if (buttonCentered({ "Exit", font }, { x, y += h + 5 })) {
				g.window->close();
			}

		}
		else if (menuState == 1) {
			checkBoxCentered({ "Show FPS", font }, { x, y += h + 5 }, &settings->showFPS);
			if (checkBoxCentered({ "Vertical Sync", font }, { x, y += h + 5 }, &settings->VSync)) {
				g.window->setVerticalSyncEnabled(settings->VSync);
			}
			if (checkBoxCentered({ "Fullscreen", font }, { x, y += h + 5 }, &settings->fullscreen)) {
				if (settings->fullscreen) {
					g.window->create(sf::VideoMode(g.gameWidth, g.gameHeight), "gaming", sf::Style::Fullscreen);
				}
				else {
					g.window->create(sf::VideoMode(g.gameWidth, g.gameHeight), "gaming");
				}
			}
			if (silderIntCentered({ "FPS Limit", ui->Roboto }, { x, y += h + 5 }, &settings->maxFPS, 30, 500)) {
				g.window->setFramerateLimit(settings->maxFPS);
			}
			if (button({ "Back", ui->Roboto }, { x, y += h + 5 })) {
				menuState = 0;
			}
		}


	}

	bool Menu::button(sf::Text text, sf::Vector2f pos)
	{
		text.setPosition(pos);
		sf::FloatRect rect = text.getGlobalBounds();
		if (rect.contains(Utils::mousePos())) {
			text.setFillColor(sf::Color::Red);
			if (InputHelper::wasMousePressed(sf::Mouse::Left)) {
				g.window->draw(text);
				return true;
			}
		}
		else {
			//text.setFillColor(sf::Color::White);
		}

		g.window->draw(text);
		return false;
	}

	bool Menu::buttonCentered(sf::Text text, sf::Vector2f pos)
	{
		sf::FloatRect rect = text.getLocalBounds();
		return button(text, { pos.x - rect.width / 2, pos.y - rect.height / 2 });
	}

	bool Menu::checkBox(sf::Text text, sf::Vector2f pos, bool* checked)
	{
		sf::FloatRect textrect = text.getLocalBounds();
		textrect.height = 25;

		sf::RectangleShape rect{ {textrect.height - textrect.top, textrect.height - textrect.top} };
		rect.setFillColor( *checked ? sf::Color::White : sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(2);
		rect.setPosition(pos);
		g.window->draw(rect);
		text.setPosition(pos.x + textrect.height - textrect.top + 5, pos.y - textrect.height / 2);
		sf::FloatRect checkRect = text.getGlobalBounds();
		checkRect.left -= textrect.height + 5;
		if (checkRect.contains(Utils::mousePos())) {
			text.setFillColor(sf::Color::Red);
			if (InputHelper::wasMousePressed(sf::Mouse::Left)) {
				*checked = !*checked;
				g.window->draw(text);
				return true;
			}
		}
		else {
			text.setFillColor(sf::Color::White);
		}
		g.window->draw(text);
		return false;
	}

	bool Menu::checkBoxCentered(sf::Text text, sf::Vector2f pos, bool* checked)
	{
		sf::FloatRect rect = text.getLocalBounds();
		return checkBox(text, { pos.x - rect.width / 2, pos.y - rect.height / 2 }, checked);
	}

	bool Menu::sliderInt(sf::Text text, sf::Vector2f pos, int* value, int min, int max)
	{
		sf::RectangleShape rect{ {200, 25} };
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(2);
		rect.setPosition(pos);
		g.window->draw(rect);
		bool didChange = false;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2f mousePos = Utils::mousePos();
			sf::FloatRect rect = sf::FloatRect{ pos.x, pos.y, 200, 25 };
			if (rect.contains(mousePos)) {
				int test = *value;
				*value = (int)((mousePos.x - pos.x) / 200.f * (max - min) + min);
				didChange = test != *value;
			}
		}
		sf::RectangleShape slider{ {5, 25} };
		slider.setFillColor(sf::Color::White);
		slider.setPosition(pos.x + 200 * (*value - min) / (max - min), pos.y);
		g.window->draw(slider);
		text.setString(text.getString() + " " + std::to_string(*value));
		text.setPosition(pos.x + 205, pos.y - 25 / 2);
		g.window->draw(text);

		return didChange;
	}

	bool Menu::silderIntCentered(sf::Text text, sf::Vector2f pos, int* value, int min, int max)
	{
		sf::FloatRect rect = text.getLocalBounds();
		rect.width += 205;
		return sliderInt(text, { pos.x - rect.width / 2, pos.y - rect.height / 2 }, value, min, max);
	}









	Menu *menu;
}