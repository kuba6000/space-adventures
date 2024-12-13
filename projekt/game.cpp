#include "game.h"
#include "globals.h"
#include "Assets.h"
#include <fstream>
#include <Windows.h>
#include "Utils.h"
#include "InputHelper.h"
#include <iostream>

namespace GigaGra {

	Map* activeMap = nullptr;
	int isLeavingEntering = 0;
	bool isMirrored = false;
	struct Bullet {
		sf::RectangleShape shape;
		sf::Vector2f vel;
	};
	std::vector<Bullet> bullets;
	struct LeavingEnteringSequence {
		float vel = 1.f;
		float acc = 0.06f;
		sf::View backView;
		sf::Vector2f viewCenter;
		sf::Vector2f viewSize;
	} leavingEnteringData;
	Game::Game()
	{
		gameView = g.window->getDefaultView();

		playerPos = { 0.f, 0.f };
		playerSprite.setTexture(assets->playerTexture);
		handSprite.setTexture(assets->handPistol);
		handPistolSprite.setTexture(assets->handPistol);
		handSprite.setOrigin({2, 2});

		map.load("assets\\map1.map");
		map2.load("assets\\map2.map");
		//activeMap = &map2;
	}
	Game::~Game()
	{
		
	}
	void Game::processEvents(sf::Event event)
	{
		switch (event.type)
		{
		case sf::Event::MouseWheelScrolled:
		{
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
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape) {
				g.gameState = 0;
			}
			else if (event.key.code == sf::Keyboard::E) {
				map.onAction(playerPos.x, playerPos.y);
				/*isLeavingEntering = 2;
				activeMap = &map2;
				leavingEnteringData.backView = g.window->getDefaultView();
				leavingEnteringData.viewCenter = {0, 0};
				leavingEnteringData.viewSize = leavingEnteringData.backView.getSize();
				leavingEnteringData.acc = 0.06f;
				leavingEnteringData.vel = 1.f;*/
			}
			else if (event.key.code == sf::Keyboard::Q) {
				isLeavingEntering = 1;
				activeMap = &map2;
				leavingEnteringData.acc = 0.06f;
				leavingEnteringData.vel = 1.f;
				leavingEnteringData.backView = g.window->getDefaultView();
				leavingEnteringData.viewCenter = { 0, 0 };
				leavingEnteringData.viewSize = leavingEnteringData.backView.getSize();

				float s = g.gameWidth * 1.8f;
				float s0 = g.gameWidth;
				float v0 = 1.f;
				float a = 0.06f;
				float t = (-2 * v0 + sqrtf(4 * (v0 * v0) - 4 * a * (2 * s0 - 2 * s))) / (2 * a);

				leavingEnteringData.viewCenter.y -= v0 * t + 0.5f * a * t * t;
				leavingEnteringData.viewSize += sf::Vector2f{ v0 * t + 0.5f * a * t * t, v0 * t + 0.5f * a * t * t };

				leavingEnteringData.vel = v0 + a * t;

			}
			break;
		}
		case sf::Event::MouseButtonPressed: {
			// shooting
			
			break;
		}
		default:
			break;
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
			v = assets->hand.getSize();
			handSprite.setTextureRect(sf::IntRect(v.x, 0, -(int)v.x, v.y));
			handSprite.setOrigin({11, 2});
			isMirrored = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			moveBy.x += speed * frame_delta;
			sf::Vector2u v = assets->playerTexture.getSize();
			playerSprite.setTextureRect(sf::IntRect(0, 0, v.x, v.y));
			v = assets->hand.getSize();
			handSprite.setTextureRect(sf::IntRect(0, 0, v.x, v.y));
			handSprite.setOrigin({ 2, 2 });
			isMirrored = false;
		}

		if (moveBy.x != 0.f || moveBy.y != 0.f) {
			
			// check collision
			
			sf::FloatRect pBounds = playerSprite.getGlobalBounds();
			sf::FloatRect pRect{ playerPos.x, playerPos.y, pBounds.width, pBounds.height };
			map.limitCollision(pRect, moveBy);

			playerPos += moveBy;
		}


		for (int i = 0; i < bullets.size(); i++) {
			Bullet& b = bullets[i];
			b.shape.move(b.vel * frame_delta);
			if (activeMap && !activeMap->isInBounds(b.shape.getPosition().x, b.shape.getPosition().y)) {
				bullets.erase(bullets.begin() + i);
				i--;
			}
		}

		playerSprite.setPosition(playerPos);
		sf::FloatRect fr = playerSprite.getGlobalBounds();
		gameView.setCenter(fr.getPosition() + (fr.getSize() / 2.f));

		if (isLeavingEntering) {

			float sign = isLeavingEntering == 1 ? -1.f : 1.f;

			leavingEnteringData.viewCenter.y -= leavingEnteringData.vel * frame_delta * sign;
			leavingEnteringData.viewSize += sf::Vector2f{	leavingEnteringData.vel * frame_delta * sign,
															leavingEnteringData.vel * frame_delta * sign };
			leavingEnteringData.vel += leavingEnteringData.acc * frame_delta * sign;
			leavingEnteringData.backView.setSize(leavingEnteringData.viewSize * (gameView.getSize().x / g.gameWidth));
			leavingEnteringData.backView.setCenter(fr.getPosition() + (fr.getSize() / 2.f) + leavingEnteringData.viewCenter);

			g.window->setView(leavingEnteringData.backView);

			activeMap->draw(0, 0, frame_delta);

			g.window->setView(g.window->getDefaultView());

			float m = g.gameWidth * 1.8f - g.gameWidth * 1.5f;
			float p = (leavingEnteringData.viewSize.x - g.gameWidth * 1.5f) / m;
			if (p > 1.f) {
				p = 1.f;
				if (isLeavingEntering == 2) {
					isLeavingEntering = 0;
					activeMap = nullptr;
				}
			}
			if (p < 0.f) {
				p = 0.f;
				if (isLeavingEntering == 1 && leavingEnteringData.viewSize.x <= g.gameWidth) {
					isLeavingEntering = 0;
				}
			}
			sf::RectangleShape fullScreen = sf::RectangleShape(sf::Vector2f(g.gameWidth, g.gameHeight));
			fullScreen.setFillColor(sf::Color(0, 0, 0, 255 * p));
			g.window->draw(fullScreen);

		}
		
			
		g.window->setView(gameView);
		//gameView.setViewport({ 0.75f, 0.f, 0.25f, 0.25f });
		//gameView.setSize(g.gameWidth / 4, g.gameHeight / 4);

		if (activeMap && !isLeavingEntering)
			activeMap->draw(0, 0, frame_delta);

		sf::Vector2f gameViewSize = gameView.getSize();
		sf::Vector2f gameViewCenter = gameView.getCenter();
		sf::FloatRect gameViewPort = sf::FloatRect(gameViewCenter.x - gameViewSize.x / 2.f, gameViewCenter.y - gameViewSize.y / 2.f, gameViewSize.x, gameViewSize.y);

		//std::cout << gameViewSize.x << " " << gameViewSize.y << std::endl;
		
		map.draw(0,0, frame_delta);

		
		handSprite.setPosition(playerPos.x + (isMirrored ? 6 : 9), playerPos.y + 14);

		sf::Vector2f mousePos = Utils::mousePos();
		mousePos -= handSprite.getPosition();
		float rot = RAD_TO_DEG(atan2f(mousePos.y, mousePos.x));
		

		if (InputHelper::wasMousePressed(sf::Mouse::Left)) {

			sf::Vector2f bulletPos = handSprite.getPosition();
			sf::Vector2f bulletDir = { cosf(DEG_TO_RAD(rot)), sinf(DEG_TO_RAD(rot)) };
			sf::Vector2f bulletVel = bulletDir * 10.f;
			sf::Vector2f bulletSize = { 4.f, 4.f };
			sf::Vector2f bulletOrigin = { 2.f, 2.f };
			sf::Color bulletColor = sf::Color::Red;
			sf::RectangleShape bullet = sf::RectangleShape(bulletSize);

			bullet.setOrigin(bulletOrigin);
			bullet.setPosition(bulletPos);
			bullet.setFillColor(bulletColor);
			bullet.setRotation(rot);
			bullets.push_back({ bullet, bulletVel });
		}

		if (isMirrored)
			rot -= 180.f;

		//std::cout << rot << '\n';
		

		handSprite.setRotation(rot);
		

		g.window->draw(playerSprite);
		g.window->draw(handSprite);

		for (Bullet& b : bullets) {
			g.window->draw(b.shape);
		}
	}

	bool Game::isInSpace()
	{
		return isLeavingEntering > 0 || !activeMap;
	}

	Game *game;
}