#pragma once
#include "includes.h"
#include "Tile.h"

const int WIDTH_MAP = 154;
const int HEIGHT_MAP = 200;

const int RADIUS_WATER = 6;

enum TypeOfMovementWater
{
	NoneDir,
	RightDir,
	DownDir,
	LeftDir,
};

struct ParamsForTile
{
	int iterX = 0;
	int iterY = 0;

	int xCoord = 0;
	int yCoord = 0;

	int iterForVector = 0;
	int numberOfTile = 0;
};

struct TwoTypeWater
{
	TypeTile typeWaterCurrentTile;
	TypeTile typeWaterNextTile;
};

class Map
{
public:
	Map(const sf::View& view, sf::RenderTexture& castTexture, sf::Texture& textureOfCaveOuter, sf::RenderTexture& castTextureForWater);

	void DrawMap(sf::RenderWindow& window, sf::RenderTexture& castTexture);
	void UpdateMap(const sf::View& view, sf::RenderTexture& castTexture, sf::RenderTexture& castTextureForWater);

	void SpreadTheLight(sf::Vector2f& coord, int& radius, bool isFirstTimeOfSpreadLight);

	TypeTile GetTypeOfTile(int numberOfTile);
	void ChangeColorOfTile(int numberOfTile);

	void DeleteStone(int numberOfTile, sf::Vector2f coordOfTile);

	int GetCountOfStoneNeighbor(sf::Vector2i coordOfTile);

	void MoveStoneDown(sf::Vector2f coordOfTile);
	void MoveWater();

	std::vector<Tile*> GetVectorTiles();
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> GetMapInEnum();
	std::vector<bool> GetMapOfLightInBool();

private:
	void FillFromCell(sf::Vector2f& coord, int& radius);

	void SpreadWater(sf::Vector2f centralBlock);
	void FillWaterFromCell(sf::Vector2f centralBlock, int radius);
	void GenerateRandomWater();
	TwoTypeWater GetTypeWaterInBottomTile(TypeTile typeWaterOfCurrentTile, TypeTile typeWaterOfBottomTile);
	TwoTypeWater GetTypeWaterInLeftRightTile(TypeTile typeWaterOfCurrentTile, TypeTile typeWaterOfLeftRightTile);
	void WaterDistribution(sf::Vector2f coord);
	TypeTile GetTypeWaterByWeight(float weight);
	float GetWeightByTypeWater(TypeTile typeWater);

	void CreateTileForMap();

	int GetCountOfWallNeighbor(sf::Vector2i coordOfTile);
	void SetRandomGeneration();
	void GetNextIteration();
	void GenerateMap(int countOfIteration);

	void SetParamsForStone(ParamsForTile params);
	void SetParamsForWall(ParamsForTile params);
	void SetParamsForWaterStone(ParamsForTile params);
	void SetParamsForWater10(ParamsForTile params);
	void SetParamsForWater8(ParamsForTile params);
	void SetParamsForWater5(ParamsForTile params);
	void SetParamsForWater3(ParamsForTile params);
	void SetParamsForWater1(ParamsForTile params);

	int Random(int min, int max);

private:

	sf::Vector2f previousCoordOfSpreadLight;
	int previousRadiusOfSpreadLight = 0;

	bool isMapRenderFirstTime = false;

	sf::Texture textureOfCave;

	std::vector<Tile*> mapOfTexture;
	std::vector<std::pair<TypeTile, sf::Vector2f>> coordsOfBlockWater;

	sf::Clock timerForWater;
	bool isLeftDirection = false;
	bool isRightDirection = false;

	int countOfWater = 0;

	int m_iterX = 0;
	int m_iterY = 0;

	std::vector<bool> mapOfTileInBool;
	std::vector<bool> mapOfTileOfLight;

	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> generatedMap;
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> newGeneratedMap;
};