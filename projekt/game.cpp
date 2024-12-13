#include "game.h"
#include "globals.h"
#include "Assets.h"
#include <fstream>
#include <Windows.h>
#include "Utils.h"
#include "InputHelper.h"
#include <iostream>
#include "menu.h"
#include "UI.h"

namespace GigaGra {

    Map* activeMap = nullptr;
	int activeMapIndex = -1;
    int isLeavingEntering = 0;
    bool isMirrored = false;
    bool isInControlPanel = false;
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

	std::string planetNames[] = {
		"Abos",
		"Kelvion",
		"Apra",
		"Izarvis",
        "Genope",
        "Struxuzuno"
	};
	int shipUpgradeLevel = 0;
    std::string shipUpgradeName[] = {
        "Laser cannon",
        "Fuel factorer",
		"Shield generator",
		"Hyperdrive",
    };

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
                activeMapIndex = 0;
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

        if (isInControlPanel) {
            static const sf::Color bgColor = { 19, 25, 36 };
            static const sf::Color fgColor = { 31, 42, 59 };
            static int currentMenu = 0;
            g.window->setView(g.window->getDefaultView());
			sf::Vector2f mousePos = Utils::mousePos();
            sf::RectangleShape s = sf::RectangleShape(sf::Vector2f(g.gameWidth, g.gameHeight));
            s.setFillColor(bgColor);
            g.window->draw(s);
            s.setPosition({20, 20});
            s.setSize({ g.gameWidth - 40, g.gameHeight - 40 });
            s.setFillColor(fgColor);
            g.window->draw(s);
            s.setFillColor(bgColor);
            s.setPosition({ 200, 20 });
            s.setSize({ 20, g.gameHeight - 40 });
            g.window->draw(s);
            float y = 0;
            if (menu->button({ "Info", ui->Roboto }, { 25, y += 25 })) {
                currentMenu = 0;
            }
            if (menu->button({ "Space map", ui->Roboto }, { 25, y += 35 })) {
                currentMenu = 1;
            }
            if (menu->button({ "Exit", ui->Roboto }, { 25, y += 35 })) {
                isInControlPanel = false;
            }

            if (currentMenu == 0) {
                y = 0;
                sf::Text t{"Info", ui->Roboto};
                t.setLetterSpacing(2);
				t.setStyle(sf::Text::Bold);
                t.setPosition(225, y += 25);
                g.window->draw(t);
                t.setLetterSpacing(1);
                t.setStyle(sf::Text::Regular);
                if (!activeMap)
                    t.setString(std::string("You are currently in space"));
                else
                    t.setString(std::string("Current planet: ").append(planetNames[activeMapIndex]));
                t.setPosition(225, y += 35);
                g.window->draw(t);
            }
            else if (currentMenu == 1) {
                s.setFillColor({ 0,0,0 });
                s.setPosition(220, 20);
				s.setSize({ g.gameWidth - 240, g.gameHeight - 40 });
				g.window->draw(s);
				y = 0;
				sf::Text t{ "Space map", ui->Roboto };
                t.setLetterSpacing(2);
                t.setStyle(sf::Text::Bold);
				t.setPosition(225, y += 25);
				g.window->draw(t);
				t.setLetterSpacing(1);
				t.setStyle(sf::Text::Regular);
				t.setString(std::string("Current planet: ").append("?")); t.setPosition(225, y += 35);
				g.window->draw(t);

                std::pair<sf::Vector2f, float> planets[] = {
					{{270, g.gameHeight - 200}, 50.f},
					{{450, g.gameHeight - 140}, 40.f},
					{{360, g.gameHeight - 400}, 36.f},
					{{700, g.gameHeight - 350}, 46.f},
					{{900, g.gameHeight - 180}, 40.f},
					{{1200, 80}, 60.f}
                };

				//lambda to draw line between two planets
				auto drawLine = [&](std::pair<sf::Vector2f, float>& p1, std::pair<sf::Vector2f, float>& p2, sf::Color c = sf::Color::White) {
					sf::Vertex line[] = {
                        sf::Vertex({p1.first.x + p1.second / 2.f, p1.first.y + p1.second / 2.f}, c),
						sf::Vertex({p2.first.x + p2.second / 2.f, p2.first.y + p2.second / 2.f}, c)
					};
					g.window->draw(line, 2, sf::Lines);
				};


                // planet connections
                drawLine(planets[0], planets[1]);
                drawLine(planets[0], planets[2], shipUpgradeLevel < 1 ? sf::Color::Red : sf::Color::White);
                drawLine(planets[2], planets[3], shipUpgradeLevel < 2 ? sf::Color::Red : sf::Color::White);
                drawLine(planets[3], planets[4], shipUpgradeLevel < 3 ? sf::Color::Red : sf::Color::White);
                drawLine(planets[4], planets[5], shipUpgradeLevel < 4 ? sf::Color::Red : sf::Color::White);

                sf::CircleShape planet{};
				planet.setFillColor(sf::Color::White);
				planet.setTexture(&assets->grassTexture);
				planet.setOutlineColor(sf::Color::Red);
				int hoveredPlanet = -1;
				for (int i = 0; i < 6; i++) {
					planet.setRadius(planets[i].second);
					planet.setOrigin(planets[i].second / 2, planets[i].second / 2);
					planet.setPosition(planets[i].first);
                    if(activeMapIndex == i)
						planet.setOutlineThickness(2);
					else
						planet.setOutlineThickness(0);
					g.window->draw(planet);

                    if (planet.getGlobalBounds().contains(mousePos)) {
						hoveredPlanet = i;
                    }
				}
                if (hoveredPlanet != -1) {
                    t.setString(std::string(planetNames[hoveredPlanet])); 
                    t.setLetterSpacing(2);
					t.setStyle(sf::Text::Bold);
                    if (hoveredPlanet == 5)
                        t.setPosition(mousePos.x - 200, mousePos.y - 10);
                    else
                        t.setPosition(mousePos.x + 10, mousePos.y - 10);
                    g.window->draw(t);
					t.setFillColor(sf::Color::White);
                    if (hoveredPlanet == 0) {
                        t.setString(std::string("Your home\n").append(activeMapIndex == hoveredPlanet ? "You are here" : "Click to travel"));
                    }
                    else {
						if (shipUpgradeLevel < hoveredPlanet - 1) {
							t.setString(std::string("Requires: ").append(shipUpgradeName[hoveredPlanet - 2]));
							t.setFillColor(sf::Color::Red);
						}
						else {
							if (activeMapIndex == hoveredPlanet)
								t.setString("You are here");
							else
							    t.setString("Click to travel");
						}
                    }
                    t.setLetterSpacing(1);
                    t.setStyle(sf::Text::Regular);
                    if (hoveredPlanet == 5)
                        t.setPosition(mousePos.x - 200, mousePos.y + 15);
                    else
                        t.setPosition(mousePos.x + 10, mousePos.y + 15);
                    g.window->draw(t);
                }
				

            }
            return;
        }







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
            //sf::FloatRect pRect{ playerPos.x, playerPos.y, pBounds.width, pBounds.height };
            map.limitCollision(pBounds, moveBy);

            playerPos += moveBy;
        }

        if (!activeMap)
            bullets.clear();
        for (int i = 0; i < bullets.size(); i++) {
            Bullet& b = bullets[i];    
            sf::Vector2f moveBy = b.vel * frame_delta;
            sf::FloatRect pBounds = b.shape.getGlobalBounds();
			if (map.limitCollision(pBounds, moveBy)) {
				bullets.erase(bullets.begin() + i);
				i--;
				continue;
			}
            b.shape.move(moveBy);
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

    void Game::openControlPanel()
    {
        isInControlPanel = true;
    }

    Game *game;
}