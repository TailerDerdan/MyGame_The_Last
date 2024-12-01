#pragma once
#include "includes.h"
#include "Tile.h"

const int WIDTH_MAP = 154;
const int HEIGHT_MAP = 200;

class Map
{
public:
	Map(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, const sf::View& view, sf::RenderTexture& castTexture, sf::Texture& textureOfCaveOuter);

	void DrawMap(sf::RenderWindow& window, sf::RenderTexture& castTexture);
	void UpdateMap(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, const sf::View& view, sf::RenderTexture& castTexture);

	void FillFromCell(sf::Vector2f& coord, int& radius);

	TypeTile GetTypeOfTile(int numberOfTile);
	void ChangeColorOfTile(int numberOfTile);

	std::vector<Tile*> GetVectorTiles();
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> GetMapInEnum();
	std::vector<bool> GetMapOfLightInBool();

private:
	void CreateTileForMap();

	int GetCountOfNeighbor(sf::Vector2i coordOfTile);
	void SetRandomGeneration();
	void GetNextIteration();

	void GenerateMap(int countOfIteration);

private:
	bool isMapRenderFirstTime = false;

	sf::Vector2f previousCoordView;

	sf::Texture textureOfCave;

	std::vector<Tile*> mapOfTexture;

	std::vector<bool> mapOfTileInBool;
	std::vector<bool> mapOfTileOfLight;

	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> generatedMap;
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> newGeneratedMap;
};