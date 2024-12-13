#include "Map.h"
#include <fstream>
#include "Assets.h"
#include "globals.h"

namespace GigaGra {
	Map::Map()
	{

	}
	Map::~Map()
	{
		if (width == 0) return;
		for (int i = 0; i < width * height; i++) {
			if (tiles[i]) {
				delete tiles[i];
			}
		}
		delete[] tiles;
	}

	bool Map::load(std::string mapPath) {
		std::ifstream file(mapPath, std::ios::binary);
		file.read(reinterpret_cast<char*>(&width), sizeof(width));
		file.read(reinterpret_cast<char*>(&height), sizeof(height));
		tiles = new Tile * [width * height];
		for (int i = 0; i < width * height; i++) {
			unsigned char id;
			file.read(reinterpret_cast<char*>(&id), sizeof(id));
			if (id == 0) {
				tiles[i] = nullptr;
			}
			else {
				tiles[i] = assets->availableTiles[id - 1]->clone();
			}
		}
		map0x = width / 2;
		map0y = height / 2;
		return true;
	}
	bool Map::loadFromMemory(char* map, unsigned int n)
	{
		return false;
	}
	Tile* Map::getTile(int x, int y) {
		int i = y * width + x;
		return tiles[i];
	}
	Tile* Map::getTileMap(float x, float y)
	{
		int tileX = floor(x / TILE_SIZE) + map0x;
		int tileY = floor(y / TILE_SIZE) + map0y;
		if (tileX >= 0 && tileX < width && tileY >= 0 && tileY < height) {
			return getTile(tileX, tileY);
		}
		return nullptr;
	}
	Tile* Map::getTileMap(sf::Vector2f pos)
	{
		return getTileMap(pos.x, pos.y);
	}
	void Map::limitCollision(const sf::FloatRect& rectToCollide, sf::Vector2f& movementToLimit)
	{
		if (movementToLimit.x > 0) {
			sf::Vector2f cornerTR = { rectToCollide.left + rectToCollide.width, rectToCollide.top };
			sf::Vector2f cornerBR = { rectToCollide.left + rectToCollide.width, rectToCollide.top + rectToCollide.height };
			sf::Vector2f cornerTRNew = { cornerTR.x + movementToLimit.x, cornerTR.y };
			sf::Vector2f cornerBRNew = { cornerBR.x + movementToLimit.x, cornerBR.y };
			if (Tile* t = getTileMap(cornerTRNew); t && !t->isWalkable()) {
				movementToLimit.x = std::fminf(movementToLimit.x, ((int)floor(cornerTRNew.x / TILE_SIZE)) * TILE_SIZE - cornerTR.x - 1);
			}
			if (Tile* t = getTileMap(cornerBRNew); t && !t->isWalkable()) {
				movementToLimit.x = std::fminf(movementToLimit.x, ((int)floor(cornerTRNew.x / TILE_SIZE)) * TILE_SIZE - cornerTR.x - 1);
			}
		}
		if (movementToLimit.x < 0) {
			sf::Vector2f cornerTL = { rectToCollide.left, rectToCollide.top };
			sf::Vector2f cornerBL = { rectToCollide.left, rectToCollide.top + rectToCollide.height };
			sf::Vector2f cornerTLNew = { cornerTL.x + movementToLimit.x, cornerTL.y };
			sf::Vector2f cornerBLNew = { cornerBL.x + movementToLimit.x, cornerBL.y };
			if (Tile* t = getTileMap(cornerTLNew); t && !t->isWalkable()) {
				movementToLimit.x = std::fmaxf(movementToLimit.x, (((int)floor(cornerTLNew.x / TILE_SIZE)) * TILE_SIZE + TILE_SIZE - 1) - cornerTL.x + 1);
			}
			if (Tile* t = getTileMap(cornerBLNew); t && !t->isWalkable()) {
				movementToLimit.x = std::fmaxf(movementToLimit.x, (((int)floor(cornerBLNew.x / TILE_SIZE)) * TILE_SIZE + TILE_SIZE - 1) - cornerTL.x + 1);
			}
		}
		if (movementToLimit.y > 0) {
			sf::Vector2f cornerBL = { rectToCollide.left, rectToCollide.top + rectToCollide.height };
			sf::Vector2f cornerBR = { rectToCollide.left + rectToCollide.width, rectToCollide.top + rectToCollide.height };
			sf::Vector2f cornerBLNew = { cornerBL.x, cornerBL.y + movementToLimit.y };
			sf::Vector2f cornerBRNew = { cornerBR.x, cornerBR.y + movementToLimit.y };
			if (Tile* t = getTileMap(cornerBLNew); t && !t->isWalkable()) {
				movementToLimit.y = std::fminf(movementToLimit.y, ((int)floor(cornerBLNew.y / TILE_SIZE)) * TILE_SIZE - cornerBL.y - 1);
			}
			if (Tile* t = getTileMap(cornerBRNew); t && !t->isWalkable()) {
				movementToLimit.y = std::fminf(movementToLimit.y, ((int)floor(cornerBRNew.y / TILE_SIZE)) * TILE_SIZE - cornerBR.y - 1);
			}
		}
		if (movementToLimit.y < 0) {
			sf::Vector2f cornerTL = { rectToCollide.left, rectToCollide.top };
			sf::Vector2f cornerTR = { rectToCollide.left + rectToCollide.width, rectToCollide.top };
			sf::Vector2f cornerTLNew = { cornerTL.x, cornerTL.y + movementToLimit.y };
			sf::Vector2f cornerTRNew = { cornerTR.x, cornerTR.y + movementToLimit.y };
			if (Tile* t = getTileMap(cornerTLNew); t && !t->isWalkable()) {
				movementToLimit.y = std::fmaxf(movementToLimit.y, (((int)floor(cornerTLNew.y / TILE_SIZE)) * TILE_SIZE + TILE_SIZE - 1) - cornerTL.y + 1);
			}
			if (Tile* t = getTileMap(cornerTRNew); t && !t->isWalkable()) {
				movementToLimit.y = std::fmaxf(movementToLimit.y, (((int)floor(cornerTRNew.y / TILE_SIZE)) * TILE_SIZE + TILE_SIZE - 1) - cornerTR.y + 1);
			}
		}
	}
	void Map::draw(float x, float y, float frame_delta)
	{
		sf::View view = g.window->getView();
		sf::Vector2f viewCenter = view.getCenter();
		sf::Vector2f viewSize = view.getSize();

		int startX = floor((viewCenter.x - viewSize.x / 2) / TILE_SIZE);
		int startY = floor((viewCenter.y - viewSize.y / 2) / TILE_SIZE);
		int endX = floor((viewCenter.x + viewSize.x / 2) / TILE_SIZE) + 1;
		int endY = floor((viewCenter.y + viewSize.y / 2) / TILE_SIZE) + 1;

		for (int x = startX; x < endX; x++) {
			for (int y = startY; y < endY; y++) {
				if (x < -map0x || y < -map0y || x >= map0x || y >= map0y) continue;
				Tile* tile = getTile(x + map0x, y + map0y);
				if (tile)
					tile->draw(x * 32, y * 32, frame_delta);
			}
		}
	}
}