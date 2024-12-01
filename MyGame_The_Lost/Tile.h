#pragma once
#include "includes.h"

const int WIDTH_TILE = 25;
const int HEIGHT_TILE = 25;

enum TypeTile {
	Wall,
	Stone,
};

struct Tile {
	TypeTile typeTile;
	sf::IntRect texture;
	sf::Sprite sprite;

	int number;

	Tile()
	{
		typeTile = TypeTile::Wall;
		number = -1;
	}
};