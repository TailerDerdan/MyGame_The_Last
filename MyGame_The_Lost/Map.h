#pragma once
#include "includes.h"
#include "Tile.h"

const int WIDTH_MAP = 154;
const int HEIGHT_MAP = 200;

const int RADIUS_WATER = 8;

const int MAX_LEVEL = 3;

const int RADIUS_SPREAD_LIGHT_LEVEL_1 = 4;
const int RADIUS_SPREAD_LIGHT_LEVEL_2 = 4;
const int RADIUS_SPREAD_LIGHT_LEVEL_3 = 3;

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
	double weightCurrentTile;
	double weightNextTile;
};

class Map
{
public:
	Map(const sf::View& view, sf::RenderTexture& castTexture, sf::Texture& textureOfCaveOuter, sf::Texture& textureOfHole);

	void DrawMap(sf::RenderWindow& window, sf::RenderTexture& castTexture);
	void UpdateMap(const sf::View& view, sf::RenderTexture& castTexture);

	void SpreadTheLight(sf::Vector2f& coord, bool isFirstTimeOfSpreadLight);

	TypeTile GetTypeOfTile(int numberOfTile);
	void ChangeColorOfTile(int numberOfTile);

	void DeleteStone(int numberOfTile, sf::Vector2f coordOfTile);

	bool DidPlayerFindTeam(sf::Vector2f coordPlayer);

	int GetCountOfStoneNeighbor(sf::Vector2i coordOfTile);

	void MoveStoneDown(sf::Vector2f coordOfTile);
	void MoveWater();
	bool CoordInWater(sf::Vector2f coord);

	std::vector<Tile*> GetVectorTiles();
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> GetMapInEnum();
	std::vector<bool> GetMapOfLightInBool();

	int GetRadiusSpreadLight();

private:
	void FillFromCell(sf::Vector2f& coord, int& radius);

	void SpreadWater(sf::Vector2f centralBlock);
	void FillWaterFromCell(sf::Vector2f centralBlock, int radius);
	void GenerateRandomWater(int countOfMainBlockWater);
	TwoTypeWater GetTypeWaterInBottomTile(double weightOfCurrentTile, double weightOfBottomTile);
	TwoTypeWater GetTypeWaterInLeftRightTile(double weightOfCurrentTile, double weightOfLeftRightTile);
	void WaterDistribution(sf::Vector2f coord);
	std::vector<BlockWater>::iterator FindBlockWaterInOldVector(sf::Vector2f coordBlock);
	std::vector<BlockWater>::iterator FindBlockWaterInNewVector(sf::Vector2f coordBlock);

	void CreateTileForMap();

	void CreatePortalToNextLevel();

	int GetCountOfWallNeighbor(sf::Vector2i coordOfTile);
	void SetRandomGeneration();
	void GetNextIteration();
	void GenerateMap(int countOfIteration);

	int GetCountOfStoneNeighborForCoal(sf::Vector2i coordOfTile);
	void SetRandomGenerationForCoal();
	void GetNextIterationForCoal();
	void GenerateCoal(int countOfIteration);

	void SetParamsForStone(ParamsForTile params);
	void SetParamsForWall(ParamsForTile params);
	void SetParamsForWaterStone(ParamsForTile params);
	void SetParamsForCoal(ParamsForTile params);
	void SetParamsForWaterCoal(ParamsForTile params);

	void MakeMap(const sf::View& view, sf::RenderTexture& castTexture);

	int Random(int min, int max);

private:

	sf::Vector2f previousCoordOfSpreadLight;
	int previousRadiusOfSpreadLight = 0;
	int radiusSpreadLight = 0;

	bool isMapRenderFirstTime = false;

	sf::Texture textureOfCave;

	std::vector<Tile*> mapOfTexture;
	std::vector<BlockWater> blocksWater;
	std::vector<BlockWater> newBlocksWater;

	sf::Clock timerForWater;
	bool isLeftDirection = false;
	bool isRightDirection = false;

	sf::Sprite teamOfPlayer;
	sf::Texture teamOfPlayer_texture;

	std::vector<bool> mapOfTileInBool;
	std::vector<bool> mapOfTileOfLight;

	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> generatedMap;
	std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> newGeneratedMap;

	int currentLevel = 1;
};