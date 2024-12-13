#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"

namespace GigaGra {

#define TILE_SIZE 32

	class Map
	{
	public:
		Map();
		~Map();
		bool load(std::string mapPath);
		bool loadFromMemory(char* map, unsigned int n);
		void draw(float x, float y, float frame_delta);
		Tile* getTile(int x, int y);
		Tile* getTileMap(float x, float y);
		Tile* getTileMap(sf::Vector2f pos);
		void limitCollision(const sf::FloatRect &rectToCollide, sf::Vector2f &movementToLimit);
		bool isInBounds(float x, float y);

		bool onAction(float x, float y);

	private:
		int width = 0;
		int height = 0;
		int map0x = 0;
		int map0y = 0;
		Tile** tiles = 0;
	};

}

