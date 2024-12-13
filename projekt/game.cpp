#include "game.h"
#include "globals.h"
#include "Assets.h"
#include <fstream>
#include <Windows.h>

namespace GigaGra {
	Game::Game()
	{
		gameView = g.window->getDefaultView();

		playerPos = { 0.f, 0.f };
		playerSprite.setTexture(assets->playerTexture);
		//ziomalSprite.setScale(5.f, 5.f);

		map.load("assets\\map1.map");
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
		float speed = 3.f;

		sf::Vector2f moveBy = { 0.f, 0.f };
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			moveBy.y -= speed * frame_delta;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			moveBy.y += speed * frame_delta;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			moveBy.x -= speed * frame_delta;
			sf::Vector2u v = assets->playerTexture.getSize();
			playerSprite.setTextureRect(sf::IntRect(v.x, 0, -(int)v.x, v.y));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			moveBy.x += speed * frame_delta;
			sf::Vector2u v = assets->playerTexture.getSize();
			playerSprite.setTextureRect(sf::IntRect(0, 0, v.x, v.y));
		}

		if (moveBy.x != 0.f || moveBy.y != 0.f) {
			
			// check collision
			
			sf::FloatRect pBounds = playerSprite.getGlobalBounds();
			sf::FloatRect pRect{ playerPos.x, playerPos.y, pBounds.width, pBounds.height };
			map.limitCollision(pRect, moveBy);

			playerPos += moveBy;
		}

		playerSprite.setPosition(playerPos);

		sf::FloatRect fr = playerSprite.getGlobalBounds();
		gameView.setCenter(fr.getPosition() + (fr.getSize() / 2.f));

		gameView.setViewport({ 0.75f, 0.f, 0.25f, 0.25f });
		//gameView.setSize(g.gameWidth / 4, g.gameHeight / 4);

		g.window->setView(gameView);
		sf::Vector2f gameViewSize = gameView.getSize();
		sf::Vector2f gameViewCenter = gameView.getCenter();
		sf::FloatRect gameViewPort = sf::FloatRect(gameViewCenter.x - gameViewSize.x / 2.f, gameViewCenter.y - gameViewSize.y / 2.f, gameViewSize.x, gameViewSize.y);

		//std::cout << gameViewSize.x << " " << gameViewSize.y << std::endl;
		
		map.draw(0,0, frame_delta);

		g.window->draw(playerSprite);
	}

	Game *game;
}