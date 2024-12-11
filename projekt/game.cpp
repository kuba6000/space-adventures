#include "game.h"
#include "globals.h"
namespace GigaGra {
	Game::Game()
	{
		gameView = g.window->getDefaultView();

		// load textures
		grassTexture.loadFromFile("assets\\grass.png");
		stoneTexture.loadFromFile("assets\\stone.png");
		ziomalTexture.loadFromFile("assets\\ziomal.png");

		ziomalPos = { 200.f, 200.f };
		ziomalSprite.setTexture(ziomalTexture);
		ziomalSprite.setScale(5.f, 5.f);

		grassSprite.setTexture(grassTexture);
		stoneSprite.setTexture(stoneTexture);
	}
	Game::~Game()
	{
	}
	void Game::processEvents(sf::Event event)
	{
		if (event.type == sf::Event::MouseWheelScrolled) {
			sf::Vector2f size = gameView.getSize();
			float zoomFactor = size.x / g.gameWidth;
			if (event.mouseWheelScroll.delta > 0)
			{
				if (zoomFactor > 0.3f)
					zoomFactor -= 0.1f;
			}
			else
			{
				if (zoomFactor < 1.5f)
					zoomFactor += 0.1f;
			}
			gameView.setSize(g.gameWidth * zoomFactor, g.gameHeight * zoomFactor);
		}
	}
	void Game::update()
	{
	}
	void Game::render(float frame_delta)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			ziomalPos.y -= 10 * frame_delta;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			ziomalPos.y += 10 * frame_delta;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			ziomalPos.x -= 10 * frame_delta;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			ziomalPos.x += 10 * frame_delta;
		}

		ziomalSprite.setPosition(ziomalPos);

		sf::FloatRect fr = ziomalSprite.getGlobalBounds();
		gameView.setCenter(fr.getPosition() + (fr.getSize() / 2.f));

		g.window->setView(gameView);
		sf::Vector2f gameViewSize = gameView.getSize();
		sf::Vector2f gameViewCenter = gameView.getCenter();
		sf::FloatRect gameViewPort = sf::FloatRect(gameViewCenter.x - gameViewSize.x / 2.f, gameViewCenter.y - gameViewSize.y / 2.f, gameViewSize.x, gameViewSize.y);

		//std::cout << gameViewSize.x << " " << gameViewSize.y << std::endl;
		for (int x = -6400; x < 6400; x += 32)
		{
			for (int y = -6400; y < 6400; y += 32)
			{
				if (gameViewPort.intersects(sf::FloatRect(x, y, 32, 32))) {
					sf::Sprite& sp = (x % 64 == 0 && y % 64 == 0) ? stoneSprite : grassSprite;
					sp.setPosition(x, y);
					g.window->draw(sp);
				}
			}
		}

		//window.draw(pouSprite);

		g.window->draw(ziomalSprite);
	}

	Game *game;
}