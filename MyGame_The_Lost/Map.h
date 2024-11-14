#pragma once
#include "includes.h"

const int WIDTH_MAP = 154;
const int HEIGHT_MAP = 200;

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
		number = 0;
	}
};

class Map
{
public:
	Map(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, sf::Vector2f viewPosition);
	void DrawMap(sf::RenderWindow& window);
	void UpdateMap(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, sf::Vector2f viewPosition);

private:
	sf::Vector2i GetCountOfNeighbor(sf::Vector2i coordOfTile);
	void SetRandomGeneration();
	void GetNextIteration();

	void GenerateMap(int countOfIteration);

private:
	sf::Vector2f previousCoordView;

	sf::Texture textureOfWall;
	sf::Texture textureOfStone;

	std::vector<Tile*> mapOfTexture;
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> generatedMap;
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> newGeneratedMap;
};