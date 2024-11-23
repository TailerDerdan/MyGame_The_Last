#pragma once
#include "includes.h"

const int WIDTH_TILE = 25;
const int HEIGHT_TILE = 25;

const int COUNT_SIDE = 4;

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

enum TypeTile {
	Wall,
	Stone,
};

struct Tile {
	TypeTile typeTile;
	sf::IntRect texture;
	sf::Sprite sprite;

	int number;

	bool neighboursExist[COUNT_SIDE] = { 0, 0, 0, 0 };
	int edgeId[COUNT_SIDE] = { 0, 0, 0, 0 };
	bool edgeExist[COUNT_SIDE] = { 0, 0, 0, 0 };

	Tile()
	{
		typeTile = TypeTile::Wall;
		number = -1;
	}
};