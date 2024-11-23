#pragma once
#include "includes.h"
#include "Tile.h"

const int WIDTH_MAP = 154;
const int HEIGHT_MAP = 200;

class Map
{
public:
	Map(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, sf::Vector2f viewPosition);

	void DrawMap(sf::RenderWindow& window);
	void UpdateMap(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, const sf::Vector2f& viewPosition);

	TypeTile GetTypeOfTile(int numberOfTile);
	void ChangeColorOfTile(int numberOfTile);

	std::vector<Tile*> GetVectorTiles();

private:
	void CreateTileForMap();

	int GetCountOfNeighbor(sf::Vector2i coordOfTile);
	void SetRandomGeneration();
	void GetNextIteration();

	void SetNeighborOfTile(int iterX, int iterY, int iterForVector, int numberOfTile);

	void GenerateMap(int countOfIteration);

private:
	bool isMapRenderFirstTime = false;

	sf::Vector2f previousCoordView;

	sf::Texture textureOfCave;

	std::vector<Tile*> mapOfTexture;

	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> generatedMap;
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> newGeneratedMap;
};