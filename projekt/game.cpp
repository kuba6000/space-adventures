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
    int nextMapIndex = -1;
    int isLeavingEntering = 0;
    bool isMirrored = false;
    bool isInControlPanel = false;
    int isInMerchant = 0;
    int travelTime = 0;
    struct Bullet {
        sf::RectangleShape shape;
        sf::Vector2f vel;
        bool isEnemyBullet = false;
		int damage = 1;
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


	char shipUpgradeLevel = 1;
    int playerUpgrades = 0;
    std::string shipUpgradeName[] = {
        "Laser cannon",
        "Fuel factory",
		"Shield generator",
		"Hyperdrive",
    };

    void setupLeavingDataForEntering() {
        isLeavingEntering = 1;
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

    void setupLeavingDataForLeaving() {
        isLeavingEntering = 2;
        leavingEnteringData.acc = 0.06f;
        leavingEnteringData.vel = 1.f;
        leavingEnteringData.backView = g.window->getDefaultView();
        leavingEnteringData.viewCenter = { 0, 0 };
        leavingEnteringData.viewSize = leavingEnteringData.backView.getSize();
    }

    struct Enemy {
        int type;
        sf::Vector2f pos;
		sf::Sprite sprite;
        int hp = 1;
    };

    std::vector<Enemy> enemies{};

    struct Pickup {
        int type;
        sf::Sprite sprite;
    };

    std::vector<Pickup> pickups{};

    void Game::travelTo(int i) {
        if (shipUpgradeLevel < i - 1)
            return;
        if (activeMapIndex == i) return;
        if (activeMapIndex != -1) {
            enemies.clear();
			bullets.clear();
			pickups.clear();
            setupLeavingDataForLeaving();
			travelTime = fabsf(activeMapIndex - i) * 200;
            nextMapIndex = i;
            return;
        }
		static Map* planetMaps[] = {
			&map2,
			&map3,
			&map4,
            &map3,
            &map3,
            &map3
		};
		activeMap = planetMaps[i];
		activeMapIndex = i;
        setupLeavingDataForEntering();
    }

    Game::Game()
    {
        gameView = g.window->getDefaultView();

        playerPos = { -200.f, 0.f };
        playerSprite.setTexture(assets->playerTexture);
        handSprite.setTexture(assets->handPistol);
        handPistolSprite.setTexture(assets->handPistol);
        handSprite.setOrigin({2, 2});
		npcSprite.setTexture(assets->jackTexture);
		npcSprite.setPosition(-256, -160);

        map.load("assets\\map1.map");
        map2.load("assets\\map2.map");
        map3.load("assets\\map3.map");
        map4.load("assets\\map4.map");

        activeMap = &map2;
        activeMapIndex = 0;


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
                bool handled = false;
                handled = map.onAction(playerPos.x, playerPos.y);
                if (!handled && activeMap)
                    handled = activeMap->onAction(playerPos.x, playerPos.y);
                if (!handled && activeMapIndex == 0) {
                    // npc interaction
                    sf::FloatRect npcBounds = npcSprite.getGlobalBounds();
                    if (npcBounds.contains(playerPos)) {
                        isInMerchant = 1;
                    }
                }
            }
            else if (event.key.code == sf::Keyboard::Q) {
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
				planet.setTexture(&assets->planet1Texture);
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

                    if (InputHelper::wasMousePressed(sf::Mouse::Left) && hoveredPlanet != activeMapIndex && activeMapIndex != -1 && isLeavingEntering == 0) {
                        travelTo(hoveredPlanet);
                        isInControlPanel = false;
                    }


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
        else if (isInMerchant >= 1) {

			static const sf::Color bgColor = { 19, 25, 36 };
			static const sf::Color fgColor = { 31, 42, 59 };
			g.window->setView(g.window->getDefaultView());
			sf::Vector2f mousePos = Utils::mousePos();
			sf::RectangleShape s = sf::RectangleShape(sf::Vector2f(g.gameWidth, g.gameHeight));
			s.setFillColor(bgColor);
			g.window->draw(s);
			s.setPosition(20, 20);
			s.setSize({ g.gameWidth - 40, g.gameHeight - 40 });
			s.setFillColor(fgColor);
			g.window->draw(s);
			s.setFillColor(bgColor);
			s.setPosition(200, 20);
			s.setSize({ 20, g.gameHeight - 40 });
			g.window->draw(s);
			float y = 0;
			if (menu->button({ isInMerchant == 1 ? "Jack" : "L", ui->Roboto}, {25, y += 25})) {
			}
			if (menu->button({ "Exit", ui->Roboto }, { 25, y += 35 })) {
                    
				isInMerchant = 0;
			}
			y = 0;
			sf::Text t{ isInMerchant == 1 ? "Jack" : "L", ui->Roboto };
			t.setLetterSpacing(2);
			t.setStyle(sf::Text::Bold);
			t.setPosition(225, y += 25);
			g.window->draw(t);
			t.setLetterSpacing(1);
			t.setStyle(sf::Text::Regular);
            if (isInMerchant == 1)
			    t.setString("Hello! I am Jack The Weaponary Trader,\n How can I help you today?");
            else if(isInMerchant == 2)
                t.setString("Hello! I am L The Engineer,\n How can I help you today?");
			t.setPosition(225, y += 35);
			g.window->draw(t);
            if (isInMerchant == 1) {
                if (menu->button({ "Bullet damage [0/5]: 100 coins", ui->Roboto }, { 225, y += 35 + 35 })) {

                }
                if (menu->button({ "Armor damage [0/5]: 100 coins", ui->Roboto }, { 225, y += 35 })) {

                }
                if (menu->button({ "Your armor [0/5]: 200 coins", ui->Roboto }, { 225, y += 35 })) {

                }
                if (menu->button({ "Critical chance [0/5]: 300 coins", ui->Roboto }, { 225, y += 35 })) {

                }
                if (menu->button({ "Shield piercing [0/5]: 10000 coins", ui->Roboto }, { 225, y += 35 })) {

                }
                if (menu->button({ "Shield protection [0/5]: 10000 coins", ui->Roboto }, { 225, y += 35 })) {

                }
			}
            else if (isInMerchant == 2) {
                if (menu->button({ "Laser cannon: 100 coins + 5 laser parts", ui->Roboto }, { 225, y += 35 + 35 })) {

                }
                if (menu->button({ "Fuel factory: 500 coins + 5 fuel factory parts", ui->Roboto }, { 225, y += 35 + 35 })) {

                }
                if (menu->button({ "Shield generator: 800 coins + 5 shield parts", ui->Roboto }, { 225, y += 35 + 35 })) {

                }
                if (menu->button({ "Hyperdrive: 20000 coins + 5 hyperdrive parts", ui->Roboto }, { 225, y += 35 + 35 })) {

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
            if(activeMap)
				activeMap->limitCollision(pBounds, moveBy);

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
                continue;
            }
            if (b.isEnemyBullet) {
                if (playerSprite.getGlobalBounds().contains(b.shape.getPosition())) {
                    playerData.hp -= b.damage;
                    bullets.erase(bullets.begin() + i);
                    i--;
                    continue;
                }
            }
        }
        {
            static float t = 0.f;
            t += frame_delta;
            bool enemiesAttack = false;
            if (t > 100.f) {
                enemiesAttack = true;
                t = 0.f;
            }
            for (int i = 0; i < enemies.size(); i++) {

                Enemy& e = enemies[i];
                sf::Vector2f moveBy = { 0.f, 0.f };

                sf::Vector2f posDif = playerPos - e.pos;
                float rotation = atan2f(posDif.y, posDif.x);
                moveBy.x = cosf(rotation) * 1.f;
                moveBy.y = sinf(rotation) * 1.f;
                e.pos += moveBy * frame_delta;
                e.sprite.setPosition(e.pos);
                if (enemiesAttack) {
                    if (e.type == 2) {
                        // create bullet
						sf::RectangleShape shape = sf::RectangleShape({ 5, 5 });
						shape.setOrigin({ 2.5, 2.5 });
						shape.setRotation(rotation * 180.f / 3.14159265f);
						shape.setFillColor(sf::Color::Red);
						shape.setPosition(e.pos);
						sf::Vector2f vel = moveBy * 5.f;
                        bullets.push_back({shape, vel, true, 5});
					}
				}

                for (int j = 0; j < bullets.size(); j++) {
                    Bullet& b = bullets[j];
                    if (b.isEnemyBullet) continue;
                    if (e.sprite.getGlobalBounds().contains(b.shape.getPosition())) {
                        pushFloatingText({ "Hit!", ui->Roboto }, b.shape.getPosition(), 10);
                        e.hp -= 1;
                        bullets.erase(bullets.begin() + j);
                        j--;
                        if (e.hp <= 0) {
                            if (rand() % 100 < 5) {
                                Pickup p{ 1, {} };
                                p.sprite.setTexture(assets->shipPart1Texture);
                                p.sprite.setPosition(b.shape.getPosition());

                                pickups.push_back(p);
                            }
                            else {
                                Pickup p{ 0, {} };
                                p.sprite.setTexture(assets->coinTexture);
                                p.sprite.setPosition(b.shape.getPosition());

                                pickups.push_back(p);
                            }
                            enemies.erase(enemies.begin() + i);
                            i--;
                            goto outer_continue;
                        }
                        else
                            continue;
                    }
                }
                if (e.sprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds())) {
                    enemies.erase(enemies.begin() + i);
                    i--;
                    playerData.hp -= e.hp * 5;
                }

            outer_continue:;
            }
        }

        for (int i = 0; i < pickups.size(); i++) {
            Pickup& p = pickups[i];
            if (p.sprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds())) {
				sf::Text t = sf::Text("", ui->Roboto);
                if (p.type == 0) {
					t.setString("+1 coin");
                    t.setFillColor(sf::Color::Yellow);
                    playerData.coins += 1;
                }
                else if (p.type == 1) {
					t.setString("+1 Laser cannon part");
                    t.setFillColor(sf::Color::Red);
                    playerData.parts1 += 1;
                }
                pushFloatingText(t, p.sprite.getPosition(), 30);
                pickups.erase(pickups.begin() + i);
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
                    activeMapIndex = -1;
                    //travelTo(nextMapIndex);
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
        
            
        if (!activeMap) {
			static sf::View starsView = g.window->getDefaultView();
            starsView.setSize(sf::Vector2f{g.gameWidth, g.gameHeight} * (gameView.getSize().x / g.gameWidth));
            static float yoffset = 0;
            yoffset -= 3.f + moveBy.y;
            starsView.setCenter(g.gameWidth, yoffset + fr.getPosition().y + (fr.getSize().y / 2.f));

			g.window->setView(starsView);

            travelTime -= 1.f * frame_delta;

            if (travelTime <= 0 && nextMapIndex != -1) {
                travelTo(nextMapIndex);
            }
            else {
                static std::vector<std::pair<sf::Vector2f, float>> stars{};

                if (stars.size() == 0) {
                    for (int i = 0; i < 1000; i++) {
                        stars.push_back({ {Utils::randomFloat(0, g.gameWidth * 2.f), Utils::randomFloat(0, g.gameHeight * 2.f)}, Utils::randomFloat(1, 3) });
                    }
                }

                sf::Vector2f cent = starsView.getCenter();
                float bott = cent.y + g.gameHeight;

                for (int i = 0; i < stars.size(); i++) {
					std::pair<sf::Vector2f, float>& s = stars[i];
                    sf::CircleShape star{ s.second };
                    star.setFillColor(sf::Color::White);
                    star.setPosition(s.first);
                    g.window->draw(star);

                    if (s.first.y > bott)
						s.first.y -= g.gameHeight * 2.f;
                }
            }
        }

        g.window->setView(gameView);
        //gameView.setViewport({ 0.75f, 0.f, 0.25f, 0.25f });
        //gameView.setSize(g.gameWidth / 4, g.gameHeight / 4);

        if (activeMap && !isLeavingEntering) {
            activeMap->draw(0, 0, frame_delta);
            if (activeMapIndex == 0) {
                g.window->draw(npcSprite);
            }
            else if (activeMapIndex > 0) {
                static float t = 0;
                t += frame_delta;
                if (t > 100) {
                    t = 0;
                    // spawn enemies outside visible area

					float leftX = gameView.getCenter().x - gameView.getSize().x / 2.f;
					float rightX = gameView.getCenter().x + gameView.getSize().x / 2.f;
					float topY = gameView.getCenter().y - gameView.getSize().y / 2.f;
					float bottomY = gameView.getCenter().y + gameView.getSize().y / 2.f;

                    int side = rand() % 4;
                    Enemy e;
					if (side == 0) {
						// top
						e = { 0, { Utils::randomFloat(leftX, rightX), topY - 100 }, {} };
					}
					else if (side == 1) {
						// right
						e = { 0, { rightX + 100, Utils::randomFloat(topY, bottomY) }, {} };
					}
					else if (side == 2) {
						// bottom
						e = { 0, { Utils::randomFloat(leftX, rightX), bottomY + 100 }, {} };
						
					}
					else if (side == 3) {
						// left
						e = { 0, { leftX - 100, Utils::randomFloat(topY, bottomY) }, {} };
					}
                    e.sprite.setPosition(e.pos);
                    if (activeMapIndex == 1) {
                        e.sprite.setTexture(assets->gragTexture);
                        
                    }
                    else if (activeMapIndex == 2) {
                        e.sprite.setTexture(assets->stoneBlobTexture);
                        e.type = 1;
                        if (rand() % 100 < 30)
                        {
                            e.sprite.setTexture(assets->ironManTexture);
                            e.type = 2;
							e.hp = 3;
                        }
                    }
                    enemies.push_back(e);
                }
            }
        }
        

        sf::Vector2f gameViewSize = gameView.getSize();
        sf::Vector2f gameViewCenter = gameView.getCenter();
        sf::FloatRect gameViewPort = sf::FloatRect(gameViewCenter.x - gameViewSize.x / 2.f, gameViewCenter.y - gameViewSize.y / 2.f, gameViewSize.x, gameViewSize.y);

        //std::cout << gameViewSize.x << " " << gameViewSize.y << std::endl;
        
        map.draw(0,0, frame_delta);

        
        handSprite.setPosition(playerPos.x + (isMirrored ? 6 : 9), playerPos.y + 14);

        sf::Vector2f mousePos = Utils::mousePos();
        mousePos -= handSprite.getPosition();
        float rot = RAD_TO_DEG(atan2f(mousePos.y, mousePos.x));
        

        if (InputHelper::wasMousePressed(sf::Mouse::Left) && activeMap && !isLeavingEntering) {

            sf::Vector2f bulletPos = handSprite.getPosition();
            sf::Vector2f bulletDir = { cosf(DEG_TO_RAD(rot)), sinf(DEG_TO_RAD(rot)) };
            sf::Vector2f bulletVel = bulletDir * 10.f;
            sf::Vector2f bulletSize = { 4.f, 4.f };
            sf::Vector2f bulletOrigin = { 2.f, 2.f };
            sf::Color bulletColor = { 201, 201, 201 };
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

        for (Enemy& e : enemies) {
			g.window->draw(e.sprite);
        }
        {
            static int t = 0;
            for (Pickup& p : pickups) {

                if (t < 10)
                {
                    p.sprite.move(0.f, 0.5f);
                }
                else if (t < 20)
                {
                    p.sprite.move(0.f, -0.5f);
                }
                else
                {
                    t = 0;
                }

                g.window->draw(p.sprite);
            }
            t++;
        }

		// floating texts
		for (int i = 0; i < floatingTexts.size(); i++) {
			auto& ft = floatingTexts[i];
			ft.second -= frame_delta;
            ft.first.move(0.f, -1.f * frame_delta);
			if (ft.second <= 0) {
				floatingTexts.erase(floatingTexts.begin() + i);
				i--;
				continue;
			}
			g.window->draw(ft.first);
		}

        if (activeMap)
        {
            std::string hint = activeMap->findInteractionHint(playerPos.x, playerPos.y);
            if (hint == "")
				hint = map.findInteractionHint(playerPos.x, playerPos.y);
            if (hint != "") {
                sf::Text t{ hint, ui->Roboto };
                t.setCharacterSize(18);
                t.setStyle(sf::Text::Bold);
                t.setFillColor(sf::Color::White);
                t.setPosition(playerPos.x - 50, playerPos.y - 50);
                g.window->draw(t);
            }
        }

        // player ui

		g.window->setView(g.window->getDefaultView());

        sf::RectangleShape s{ {g.gameWidth, 50} };
        static const sf::Color bgColor = { 19, 25, 36 };
        static const sf::Color fgColor = { 31, 42, 59 };
		s.setFillColor(bgColor);
        s.setPosition(0, g.gameHeight - 50);
		g.window->draw(s);

		s.setFillColor(fgColor);
		s.setPosition(0, g.gameHeight - 40);
		s.setSize({ g.gameWidth, 40 });
        g.window->draw(s);

		sf::RectangleShape healthBar = sf::RectangleShape(sf::Vector2f(100, 10));
		healthBar.setFillColor(sf::Color::Red);
		healthBar.setPosition(10, g.gameHeight - 15);
		g.window->draw(healthBar);

		healthBar.setSize(sf::Vector2f(playerData.hp, 10));
		healthBar.setFillColor(sf::Color::Green);
		g.window->draw(healthBar);

		sf::Text t{ "HP", ui->Roboto };
		t.setCharacterSize(18);
		t.setStyle(sf::Text::Bold);

		t.setPosition(10, g.gameHeight - 35);
		t.setFillColor(sf::Color::White);
        g.window->draw(t);

		t.setString("Coins");
        t.setPosition(130, g.gameHeight - 35);
        g.window->draw(t);
        sf::Text d {std::to_string(playerData.coins),  ui->Roboto };
		d.setCharacterSize(18);
        d.setPosition(130, g.gameHeight - 20);
        d.setFillColor({ 255, 181, 43 });
        g.window->draw(d);






    }

    bool Game::isInSpace()
    {
        return isLeavingEntering > 0 || !activeMap;
    }

    void Game::openControlPanel()
    {
        isInControlPanel = true;
    }

    void Game::save()
    {
		std::ofstream file("savegame.sav", std::ios::binary);

        file.write(&shipUpgradeLevel, 1);
        file.write(reinterpret_cast<char*>(&playerPos), sizeof(sf::Vector2f));
        file.write(reinterpret_cast<char*>(&playerData), sizeof(PlayerData));

        pushFloatingText({ "Saved!", ui->Roboto }, playerPos, 100);
    }

    bool Game::load()
    {
		std::ifstream file("savegame.sav", std::ios::binary);
        if (!file.is_open()) return false;
		file.read(&shipUpgradeLevel, 1);
		file.read(reinterpret_cast<char*>(&playerPos), sizeof(sf::Vector2f));
		file.read(reinterpret_cast<char*>(&playerData), sizeof(PlayerData));
        return true;
    }

    void Game::pushFloatingText(sf::Text text, sf::Vector2f pos, float time)
    {
        text.setCharacterSize(18);
        text.setPosition(pos);
		floatingTexts.push_back({ text, time });
    }

    Game *game;
}