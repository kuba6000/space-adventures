#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "globals.h"
#include "game.h"
#include "menu.h"
#include "UI.h"
#include "Settings.h"
#include "InputHelper.h"
#include "editor.h"
#include "Assets.h"

using namespace GigaGra;

int main() {

    sf::RenderWindow window(sf::VideoMode(g.gameWidth, g.gameHeight), "gaming");
	window.setVerticalSyncEnabled(true);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    /* std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (std::size_t i = 0; i < modes.size(); ++i)
    {
        sf::VideoMode mode = modes[i];
        std::cout << "Mode #" << i << ": "
                << mode.width << "x" << mode.height << " - "
                << mode.bitsPerPixel << " bpp" << std::endl;
    }*/


    std::chrono::steady_clock::time_point frame_timing = std::chrono::steady_clock::now();

    g.window = &window;

    window.clear();

    ui = new UI{};

    sf::Text t{ "Loading", ui->Roboto };
	t.setCharacterSize(24);
	t.setStyle(sf::Text::Bold);
	t.setFillColor(sf::Color::White);
	t.setPosition(g.gameWidth / 2 - t.getLocalBounds().width / 2, g.gameHeight / 2 - t.getLocalBounds().height / 2);
	window.draw(t);
    window.display();

    assets = new Assets{};
    settings = new Settings{};
	menu = new Menu{};
	game = new Game{};
	editor = new Editor{};

	// main loop
    while (window.isOpen())
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		long long frame_time = std::chrono::duration_cast<std::chrono::microseconds>(now - frame_timing).count();
        frame_timing = now;
		//std::cout << frame_time << std::endl;

		// frame delta for physics
		float frame_delta = frame_time / 10000.f;

        // calculate fps
        float fps = 1000000.f / frame_time;

        window.clear();
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			InputHelper::processEvents(event);
            if (g.gameState == 0) {
                // menu

                menu->processEvents(event);
            }
            else if (g.gameState == 1) {
                // settings

                game->processEvents(event);
            }
            else if (g.gameState == 2) {
                // 

				editor->processEvents(event);
            }
            
        }

        if (g.gameState == 0) {
			// menu

            menu->render(frame_delta);
		}
		else if (g.gameState == 1) {
			// settings

			game->render(frame_delta);
		}
        else if (g.gameState == 2) {
            // 

            editor->render(frame_delta);
        }

        // ui
		
        if (settings->showFPS)
            ui->showFPS(fps);

        window.display();

        InputHelper::afterFrame();
    }

    delete assets;
	delete ui;
    delete settings;
    delete menu;
    delete game;
    delete editor;

    return 0;
}