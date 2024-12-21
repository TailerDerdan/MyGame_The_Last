#pragma once
#include "includes.h"

const int WIDTH_TILE = 25;
const int HEIGHT_TILE = 25;

enum TypeTile 
{
	Wall,
	Stone,
	WaterInStone,
	Coal,
	WaterInCoal,
};

struct Tile 
{
	TypeTile typeTile;
	sf::IntRect texture;
	sf::Sprite sprite;

	float weight = 0.0f;

	int number;

	Tile()
	{
		typeTile = TypeTile::Wall;
		number = 0;
	}
};

struct BlockWater
{
	double weight = 0.0f;
	sf::RectangleShape block;
	sf::Vector2f coord;

	bool operator==(const BlockWater& rhs)
	{
		if (rhs.coord == this->coord)
		{
			return true;
		}
	}
};