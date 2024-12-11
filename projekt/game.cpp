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

		std::ifstream file("assets\\map1.map");
		if (file.is_open()) {
			for (int x = 0; x < 1000; x++) {
				for (int y = 0; y < 1000; y++) {
					if (map[x][y]) delete map[x][y];
					unsigned char id;
					file >> id;
					if (id == 0) {
						map[x][y] = nullptr;
					}
					else {
						map[x][y] = assets->availableTiles[id - 1]->clone();
					}
				}
			}
		}
		else
		{
			MessageBox(NULL, L"Failed to load map1.map", L"Error", MB_ICONERROR || MB_OK);
			exit(1);
		}
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
			
			sf::FloatRect pBounds = playerSprite.getGlobalBounds();

			if (moveBy.x > 0) {
				sf::Vector2f cornerTR = { playerPos.x + pBounds.width, playerPos.y };
				sf::Vector2f cornerBR = { playerPos.x + pBounds.width, playerPos.y + pBounds.height };
				sf::Vector2f cornerTRNew = { cornerTR.x + moveBy.x, cornerTR.y };
				sf::Vector2f cornerBRNew = { cornerBR.x + moveBy.x, cornerBR.y };
				int tileX = floor(cornerTRNew.x / 32) + 500;
				int tileY = floor(cornerTRNew.y / 32) + 500;
				if (map[tileX][tileY] && !map[tileX][tileY]->isWalkable()) {
					moveBy.x = std::fminf(moveBy.x, (tileX - 500) * 32.f - cornerTR.x - 1);
				}
				tileX = floor(cornerBRNew.x / 32) + 500;
				tileY = floor(cornerBRNew.y / 32) + 500;
				if (map[tileX][tileY] && !map[tileX][tileY]->isWalkable()) {
					moveBy.x = std::fminf(moveBy.x, (tileX - 500) * 32.f - cornerTR.x - 1);
				}
			}
			if (moveBy.x < 0) {
				sf::Vector2f cornerTL = { playerPos.x, playerPos.y };
				sf::Vector2f cornerBL = { playerPos.x, playerPos.y + pBounds.height };
				sf::Vector2f cornerTLNew = { cornerTL.x + moveBy.x, cornerTL.y };
				sf::Vector2f cornerBLNew = { cornerBL.x + moveBy.x, cornerBL.y };
				int tileX = floor(cornerTLNew.x / 32) + 500;
				int tileY = floor(cornerTLNew.y / 32) + 500;
				if (map[tileX][tileY] && !map[tileX][tileY]->isWalkable()) {
					moveBy.x = std::fmaxf(moveBy.x, ((tileX - 500) * 32.f + 31) - cornerTL.x + 1);
				}
				tileX = floor(cornerBLNew.x / 32) + 500;
				tileY = floor(cornerBLNew.y / 32) + 500;
				if (map[tileX][tileY] && !map[tileX][tileY]->isWalkable()) {
					moveBy.x = std::fmaxf(moveBy.x, ((tileX - 500) * 32.f + 31) - cornerTL.x + 1);
				}
			}
			if (moveBy.y > 0) {
				sf::Vector2f cornerBL = { playerPos.x, playerPos.y + pBounds.height };
				sf::Vector2f cornerBR = { playerPos.x + pBounds.width, playerPos.y + pBounds.height };
				sf::Vector2f cornerBLNew = { cornerBL.x, cornerBL.y + moveBy.y };
				sf::Vector2f cornerBRNew = { cornerBR.x, cornerBR.y + moveBy.y };
				int tileX = floor(cornerBLNew.x / 32) + 500;
				int tileY = floor(cornerBLNew.y / 32) + 500;
				if (map[tileX][tileY] && !map[tileX][tileY]->isWalkable()) {
					moveBy.y = std::fminf(moveBy.y, (tileY - 500) * 32.f - cornerBL.y - 1);
				}
				tileX = floor(cornerBRNew.x / 32) + 500;
				tileY = floor(cornerBRNew.y / 32) + 500;
				if (map[tileX][tileY] && !map[tileX][tileY]->isWalkable()) {
					moveBy.y = std::fminf(moveBy.y, (tileY - 500) * 32.f - cornerBR.y - 1);
				}
			}
			if (moveBy.y < 0) {
				sf::Vector2f cornerTL = { playerPos.x, playerPos.y };
				sf::Vector2f cornerTR = { playerPos.x + pBounds.width, playerPos.y };
				sf::Vector2f cornerTLNew = { cornerTL.x, cornerTL.y + moveBy.y };
				sf::Vector2f cornerTRNew = { cornerTR.x, cornerTR.y + moveBy.y };
				int tileX = floor(cornerTLNew.x / 32) + 500;
				int tileY = floor(cornerTLNew.y / 32) + 500;
				if (map[tileX][tileY] && !map[tileX][tileY]->isWalkable()) {
					moveBy.y = std::fmaxf(moveBy.y, ((tileY - 500) * 32.f + 31) - cornerTL.y + 1);
				}
				tileX = floor(cornerTRNew.x / 32) + 500;
				tileY = floor(cornerTRNew.y / 32) + 500;
				if (map[tileX][tileY] && !map[tileX][tileY]->isWalkable()) {
					moveBy.y = std::fmaxf(moveBy.y, ((tileY - 500) * 32.f + 31) - cornerTR.y + 1);
				}
			}
			playerPos += moveBy;
		}

		playerSprite.setPosition(playerPos);

		sf::FloatRect fr = playerSprite.getGlobalBounds();
		gameView.setCenter(fr.getPosition() + (fr.getSize() / 2.f));

		g.window->setView(gameView);
		sf::Vector2f gameViewSize = gameView.getSize();
		sf::Vector2f gameViewCenter = gameView.getCenter();
		sf::FloatRect gameViewPort = sf::FloatRect(gameViewCenter.x - gameViewSize.x / 2.f, gameViewCenter.y - gameViewSize.y / 2.f, gameViewSize.x, gameViewSize.y);

		//std::cout << gameViewSize.x << " " << gameViewSize.y << std::endl;
		int startX = floor((gameViewCenter.x - gameViewSize.x / 2) / 32);
		int startY = floor((gameViewCenter.y - gameViewSize.y / 2) / 32);
		int endX = floor((gameViewCenter.x + gameViewSize.x / 2) / 32) + 1;
		int endY = floor((gameViewCenter.y + gameViewSize.y / 2) / 32) + 1;

		for (int x = startX; x < endX; x++) {
			for (int y = startY; y < endY; y++) {
				if (x < -500 || y < -500 || x >= 500 || y >= 500) continue;
				if (map[x + 500][y + 500])
					map[x + 500][y + 500]->draw(x * 32, y * 32, frame_delta);
			}
		}

		g.window->draw(playerSprite);
	}

	Game *game;
}