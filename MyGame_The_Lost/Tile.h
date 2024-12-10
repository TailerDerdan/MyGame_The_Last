#pragma once
#include "includes.h"

const int WIDTH_TILE = 25;
const int HEIGHT_TILE = 25;

enum TypeTile 
{
	Wall,
	Stone,

	Water1,
	Water3,
	Water5,
	Water8,
	Water10,
	
	WaterInStone,
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