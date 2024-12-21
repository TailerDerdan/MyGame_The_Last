#include "Map.h"

Map::Map(const sf::View& view, sf::RenderTexture& castTexture, sf::Texture& textureOfCaveOuter, sf::Texture& textureOfHole)
{
	mapOfTexture.resize(WIDTH_MAP * HEIGHT_MAP + 1);

	textureOfCave = textureOfCaveOuter;
	teamOfPlayer_texture = textureOfHole;

	MakeMap(view, castTexture);

	timerForWater.restart();
}

int Map::Random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return dist(rng);

	/*int randomInt = rand() % 100;

	return randomInt;*/
}

int Map::GetCountOfWallNeighbor(sf::Vector2i coordOfTile)
{
	int countOfNeighbor = 0;

	for (int iterX = -1; iterX <= 1; iterX++)
	{
		for (int iterY = -1; iterY <= 1; iterY++)
		{
			if (coordOfTile.x + iterX < 0)
			{
				continue;
			}
			if (coordOfTile.x + iterX >= WIDTH_MAP)
			{
				continue;
			}
			if (coordOfTile.y + iterY < 0)
			{
				continue;
			}
			if (coordOfTile.y + iterY >= HEIGHT_MAP)
			{
				continue;
			}
			if (iterX == 0 && iterY == 0)
			{
				continue;
			}
			if (generatedMap[iterX + coordOfTile.x][iterY + coordOfTile.y] == TypeTile::Wall)
			{
				countOfNeighbor++;
			}
		}
	}

	return countOfNeighbor;
}

int Map::GetCountOfStoneNeighborForCoal(sf::Vector2i coordOfTile)
{
	int countOfNeighbor = 0;

	for (int iterX = -1; iterX <= 1; iterX++)
	{
		for (int iterY = -1; iterY <= 1; iterY++)
		{
			if (coordOfTile.x + iterX < 0)
			{
				continue;
			}
			if (coordOfTile.x + iterX >= WIDTH_MAP)
			{
				continue;
			}
			if (coordOfTile.y + iterY < 0)
			{
				continue;
			}
			if (coordOfTile.y + iterY >= HEIGHT_MAP)
			{
				continue;
			}
			if (iterX == 0 && iterY == 0)
			{
				continue;
			}
			if (generatedMap[iterX + coordOfTile.x][iterY + coordOfTile.y] == TypeTile::Stone)
			{
				countOfNeighbor++;
			}
		}
	}

	return countOfNeighbor;
}

int Map::GetCountOfStoneNeighbor(sf::Vector2i coordOfTile)
{
	int countOfNeighbor = 0;

	if (coordOfTile.x - 1 >= 0 && generatedMap[coordOfTile.x - 1][coordOfTile.y] == TypeTile::Stone)
	{
		countOfNeighbor++;
	}

	if (coordOfTile.x + 1 < WIDTH_MAP && generatedMap[coordOfTile.x + 1][coordOfTile.y] == TypeTile::Stone)
	{
		countOfNeighbor++;
	}

	if (coordOfTile.y - 1 >= 0 && generatedMap[coordOfTile.x][coordOfTile.y - 1] == TypeTile::Stone)
	{
		countOfNeighbor++;
	}

	if (coordOfTile.y + 1 < HEIGHT_MAP && generatedMap[coordOfTile.x][coordOfTile.y + 1] == TypeTile::Stone)
	{
		countOfNeighbor++;
	}

	return countOfNeighbor;
}

void Map::SetRandomGeneration()
{
	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			if (iterX == 0 || iterY == 0 || iterX == WIDTH_MAP - 1 || iterY == HEIGHT_MAP - 1 || iterX == WIDTH_MAP - 2 || iterY == HEIGHT_MAP - 2)
			{
				generatedMap[iterX][iterY] = TypeTile::Stone;
				continue;
			}
			int randomNumber = Random(0, 100);
			if (randomNumber < 58)
			{
				generatedMap[iterX][iterY] = TypeTile::Wall;
			}
			else
			{
				generatedMap[iterX][iterY] = TypeTile::Stone;
			}
		}
	}
}

void Map::SetRandomGenerationForCoal()
{
	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			if (generatedMap[iterX][iterY] == TypeTile::Wall) continue;

			if (iterX == 0 || iterY == 0 || iterX == WIDTH_MAP - 1 || iterY == HEIGHT_MAP - 1 || iterX == WIDTH_MAP - 2 || iterY == HEIGHT_MAP - 2) continue;

			int randomNumber = Random(0, 100);
			if (randomNumber < 77)
			{
				generatedMap[iterX][iterY] = TypeTile::Stone;
			}
			else
			{
				generatedMap[iterX][iterY] = TypeTile::Coal;
			}
		}
	}
}

void Map::GetNextIteration()
{
	for (int iterX = 1; iterX < WIDTH_MAP - 1; iterX++)
	{
		for (int iterY = 1; iterY < HEIGHT_MAP - 1; iterY++)
		{
			int countOfNeighbor = GetCountOfWallNeighbor({ iterX, iterY });
			
			if (countOfNeighbor > 4)
			{
				newGeneratedMap[iterX][iterY] = TypeTile::Wall;
			}
			else
			{
				newGeneratedMap[iterX][iterY] = TypeTile::Stone;
			}
		}
	}

	for (int iterX = 1; iterX < WIDTH_MAP - 1; iterX++)
	{
		for (int iterY = 1; iterY < HEIGHT_MAP - 1; iterY++)
		{
			generatedMap[iterX][iterY] = newGeneratedMap[iterX][iterY];
		}
	}
}

void Map::GetNextIterationForCoal()
{
	for (int iterX = 1; iterX < WIDTH_MAP - 1; iterX++)
	{
		for (int iterY = 1; iterY < HEIGHT_MAP - 1; iterY++)
		{
			if (generatedMap[iterX][iterY] == TypeTile::Wall) continue;

			int countOfNeighbor = GetCountOfStoneNeighborForCoal({ iterX, iterY });

			if (countOfNeighbor > 4)
			{
				newGeneratedMap[iterX][iterY] = TypeTile::Stone;
			}
			else
			{
				newGeneratedMap[iterX][iterY] = TypeTile::Coal;
			}
		}
	}

	for (int iterX = 1; iterX < WIDTH_MAP - 1; iterX++)
	{
		for (int iterY = 1; iterY < HEIGHT_MAP - 1; iterY++)
		{
			generatedMap[iterX][iterY] = newGeneratedMap[iterX][iterY];
		}
	}
}

void Map::GenerateMap(int countOfIteration)
{
	SetRandomGeneration();
	for (int iter = 0; iter < countOfIteration; iter++)
	{
		GetNextIteration();
	}
}

void Map::GenerateCoal(int countOfIteration)
{
	SetRandomGenerationForCoal();
	for (int iter = 0; iter < countOfIteration; iter++)
	{
		GetNextIterationForCoal();
	}
}

void Map::FillWaterFromCell(sf::Vector2f coord, int radius)
{
	if (coord.y == -1 || coord.x == -1 || coord.y == HEIGHT_MAP || coord.x == WIDTH_MAP) return;

	BlockWater water;
	water.coord = { coord.x, coord.y };
	water.block.setFillColor(sf::Color(0, 0, 255, 80));
	water.block.setSize({ WIDTH_TILE, HEIGHT_TILE });
	water.block.setPosition({ coord.y * HEIGHT_TILE, coord.x * WIDTH_TILE });
	water.weight = 1.0;

	bool isThisBlock = false;
	for (auto waterBlock : blocksWater)
	{
		if (waterBlock.coord == water.coord)
		{
			isThisBlock = true;
			break;
		}
	}

	if (!isThisBlock)
	{
		blocksWater.push_back(water);
	}

	if (generatedMap[coord.x][coord.y] == TypeTile::Stone || generatedMap[coord.x][coord.y] == TypeTile::WaterInStone)
	{
		generatedMap[coord.x][coord.y] = TypeTile::WaterInStone;
	}
	else if (generatedMap[coord.x][coord.y] == TypeTile::Coal || generatedMap[coord.x][coord.y] == TypeTile::WaterInCoal)
	{
		generatedMap[coord.x][coord.y] = TypeTile::WaterInCoal;
	}
	else
	{
		generatedMap[coord.x][coord.y] = TypeTile::Wall;
	}
	
	int newRadius = radius - 1;
	if (newRadius < 0) return;

	{
		sf::Vector2f newCoord = { coord.x + 1, coord.y };
		FillWaterFromCell(newCoord, newRadius);
	}

	{
		sf::Vector2f newCoord = { coord.x - 1, coord.y };
		FillWaterFromCell(newCoord, newRadius);
	}

	{
		sf::Vector2f newCoord = { coord.x, coord.y - 1 };
		FillWaterFromCell(newCoord, newRadius);
	}

	{
		sf::Vector2f newCoord = { coord.x, coord.y + 1 };
		FillWaterFromCell(newCoord, newRadius);
	}

}

void Map::SpreadWater(sf::Vector2f centralBlock)
{
	FillWaterFromCell(centralBlock, RADIUS_WATER);
}

void Map::GenerateRandomWater(int countOfMainBlockWater)
{
	/*for (int iterX = 1; iterX < WIDTH_MAP - 1; iterX++)
	{
		if (countOfMainBlockWater == 0) return;

		for (int iterY = 1; iterY < HEIGHT_MAP - 1; iterY++)
		{
			if (countOfMainBlockWater == 0) return;

			int randomNumber = Random(0, 10000);

			if (randomNumber >= 9999)
			{
				countOfMainBlockWater--;
				SpreadWater(sf::Vector2f( float(iterX), float(iterY) ));
			}
		}
	}*/

	SpreadWater(sf::Vector2f(float(10), float(5)));
}

void Map::MakeMap(const sf::View& view, sf::RenderTexture& castTexture)
{
	mapOfTileOfLight = std::vector<bool>(WIDTH_MAP * HEIGHT_MAP, false);
	mapOfTileInBool = std::vector<bool>(WIDTH_MAP * HEIGHT_MAP, false);

	if (currentLevel == 1)
	{
		radiusSpreadLight = RADIUS_SPREAD_LIGHT_LEVEL_1;
		GenerateMap(4);
		GenerateCoal(1);
		GenerateRandomWater(10);
		CreateTileForMap();
	}
	if (currentLevel == 2)
	{
		radiusSpreadLight = RADIUS_SPREAD_LIGHT_LEVEL_2;
		GenerateMap(4);
		GenerateCoal(1);
		GenerateRandomWater(25);
	}
	if (currentLevel == 3)
	{
		radiusSpreadLight = RADIUS_SPREAD_LIGHT_LEVEL_3;
		GenerateMap(4);
		GenerateCoal(1);
		GenerateRandomWater(10);
	}

	CreatePortalToNextLevel();

	UpdateMap(view, castTexture);

	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			int numberOfTile = iterX * HEIGHT_MAP + iterY;
			if (generatedMap[iterX][iterY] != TypeTile::Wall)
			{
				mapOfTileInBool[numberOfTile] = true;
			}
		}
	}
}

void Map::CreateTileForMap()
{
	for (int iter = 0; iter < WIDTH_MAP * HEIGHT_MAP; iter++)
	{
		Tile* tile = new Tile();
		tile->sprite.setTexture(textureOfCave);
		mapOfTexture.at(iter) = tile;
	}
}

void Map::SetParamsForStone(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::Stone && mapOfTexture[params.iterForVector])
	{

		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 2.0f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::Stone;

		mapOfTexture[params.iterForVector]->texture = { 0, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

void Map::SetParamsForWall(ParamsForTile params)
{
	if ((generatedMap[params.iterX][params.iterY] == TypeTile::Wall) && mapOfTexture[params.iterForVector])
	{

		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 0.0f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::Wall;

		mapOfTexture[params.iterForVector]->texture = { 25, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

void Map::SetParamsForWaterStone(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::WaterInStone && mapOfTexture[params.iterForVector])
	{

		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 1.0f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::WaterInStone;

		mapOfTexture[params.iterForVector]->texture = { 0, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

void Map::SetParamsForCoal(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::Coal && mapOfTexture[params.iterForVector])
	{
		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 2.0f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::Coal;

		mapOfTexture[params.iterForVector]->texture = { 50, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

void Map::SetParamsForWaterCoal(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::WaterInCoal && mapOfTexture[params.iterForVector])
	{
		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 2.0f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::WaterInCoal;

		mapOfTexture[params.iterForVector]->texture = { 50, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

//TODO: декомпозировать функцию, так чтобы строк было небольше 20 и небольше 2 вложенностей
void Map::UpdateMap(const sf::View& view, sf::RenderTexture& castTexture)
{
	sf::Vector2f viewPosition = sf::Vector2f{ view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 };

	int iterForVector = 0;

	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			int numberOfTile = iterX * HEIGHT_MAP + iterY;
			int yCoord = iterX;
			int xCoord = iterY;

			if ((xCoord * WIDTH_TILE >= viewPosition.x && xCoord * WIDTH_TILE <= WINDOW_WIDTH + viewPosition.x) &&
				(yCoord * HEIGHT_TILE >= viewPosition.y && yCoord * HEIGHT_TILE <= WINDOW_HEIGHT + viewPosition.y))
			{

				ParamsForTile params;
				params.iterForVector = iterForVector;
				params.iterX = iterX;
				params.iterY = iterY;
				params.xCoord = xCoord;
				params.yCoord = yCoord;
				params.numberOfTile = numberOfTile;

				SetParamsForStone(params);
				SetParamsForWaterStone(params);
				SetParamsForWall(params);
				SetParamsForCoal(params);
				SetParamsForWaterCoal(params);

				if (mapOfTexture[iterForVector])
				{
					castTexture.draw(mapOfTexture[iterForVector]->sprite);
				}
				
				iterForVector++;
			}
		}
	}

	castTexture.draw(teamOfPlayer);

	for (auto& water : blocksWater)
	{
		if (generatedMap[water.coord.x][water.coord.y] == TypeTile::Wall)
		{
			castTexture.draw(water.block);
		}
	}
}

void Map::FillFromCell(sf::Vector2f& coord, int& radius)
{
	if (!mapOfTileInBool[coord.y * HEIGHT_MAP + coord.x])
	{
		mapOfTileOfLight[coord.y * HEIGHT_MAP + coord.x] = 1;
	}
	int newRadius = radius - 1;
	if (newRadius < 0)
	{
		return;
	}
	if (!mapOfTileInBool[coord.y * HEIGHT_MAP + coord.x + 1])
	{
		sf::Vector2f newCoord = { coord.x + 1, coord.y };
		FillFromCell(newCoord, newRadius);
	}
	if (!mapOfTileInBool[coord.y * HEIGHT_MAP + coord.x - 1])
	{
		sf::Vector2f newCoord = { coord.x - 1, coord.y };
		FillFromCell(newCoord, newRadius);
	}
	if (!mapOfTileInBool[(coord.y - 1) * HEIGHT_MAP + coord.x])
	{
		sf::Vector2f newCoord = { coord.x, coord.y - 1 };
		FillFromCell(newCoord, newRadius);
	}
	if (!mapOfTileInBool[(coord.y + 1) * HEIGHT_MAP + coord.x])
	{
		sf::Vector2f newCoord = { coord.x, coord.y + 1 };
		FillFromCell(newCoord, newRadius);
	}
}

void Map::SpreadTheLight(sf::Vector2f& coord, bool isFirstTimeOfSpreadLight)
{
	if (!isFirstTimeOfSpreadLight)
	{
		for (int iterY = previousCoordOfSpreadLight.y - previousRadiusOfSpreadLight - 1; iterY < previousCoordOfSpreadLight.y + previousRadiusOfSpreadLight + 1; iterY++)
		{
			for (int iterX = previousCoordOfSpreadLight.x - previousRadiusOfSpreadLight - 1; iterX < previousCoordOfSpreadLight.x + previousRadiusOfSpreadLight + 1; iterX++)
			{
				if (iterY < 0 || iterX < 0)
				{
					continue;
				}
				mapOfTileOfLight[iterY * HEIGHT_MAP + iterX] = 0;
			}
		}
	}

	previousCoordOfSpreadLight = coord;
	previousRadiusOfSpreadLight = radiusSpreadLight;

	FillFromCell(coord, radiusSpreadLight);
}

void Map::DrawMap(sf::RenderWindow& window, sf::RenderTexture& castTexture)
{
	window.draw(sf::Sprite(castTexture.getTexture()));
}

TypeTile Map::GetTypeOfTile(int numberOfTile)
{
	for (auto tile : mapOfTexture)
	{
		if (tile && tile->number == numberOfTile)
		{
			return tile->typeTile;
		}
	}
}

void Map::ChangeColorOfTile(int numberOfTile)
{
	for (auto tile : mapOfTexture)
	{
		if (tile && tile->number == numberOfTile)
		{
			tile->sprite.setColor(sf::Color(255, 0, 0));
			break;
		}
	}
}

std::vector<Tile*> Map::GetVectorTiles()
{
	return mapOfTexture;
}

std::array<std::array<int, HEIGHT_MAP>, WIDTH_MAP> Map::GetMapInEnum()
{
	return generatedMap;
}

std::vector<bool> Map::GetMapOfLightInBool()
{
	return mapOfTileOfLight;
}

void Map::DeleteStone(int numberOfTile, sf::Vector2f coordOfTile)
{
	if (generatedMap[int(coordOfTile.x)][int(coordOfTile.y)] == TypeTile::WaterInStone || generatedMap[int(coordOfTile.x)][int(coordOfTile.y)] == TypeTile::WaterInCoal)
	{
		BlockWater water;
		water.coord = { coordOfTile.x, coordOfTile.y };
		water.block.setFillColor(sf::Color(0, 0, 255, 80));
		water.block.setSize({ WIDTH_TILE, HEIGHT_TILE });
		water.block.setPosition({ coordOfTile.y * HEIGHT_TILE, coordOfTile.x * WIDTH_TILE });
		water.weight = 1.0;
	}

	//std::cout << generatedMap[int(coordOfTile.x)][int(coordOfTile.y)] << std::endl;

	generatedMap[int(coordOfTile.x)][int(coordOfTile.y)] = TypeTile::Wall;
	mapOfTileInBool[numberOfTile] = 0;
}

void Map::MoveStoneDown(sf::Vector2f coordOfTile)
{
	if (coordOfTile.x == WIDTH_MAP)
	{
		generatedMap[WIDTH_MAP - 1][HEIGHT_MAP - 1] = TypeTile::Stone;
		return;
	}

	generatedMap[coordOfTile.x][coordOfTile.y] = generatedMap[coordOfTile.x - 1][coordOfTile.y];
	generatedMap[coordOfTile.x - 1][coordOfTile.y] = TypeTile::Wall;
}

TwoTypeWater Map::GetTypeWaterInBottomTile(double weightOfCurrentTile, double weightOfBottomTile)
{
	TwoTypeWater typesWater;
	if (weightOfBottomTile >= 1.0f)
	{
		typesWater.weightCurrentTile = weightOfCurrentTile;
		typesWater.weightNextTile = weightOfBottomTile;
		return typesWater;
	}
	
	double newWeightOfCurrentTile = 0.0f;
	double newWeightOfBottomTile = 0.0f;

	newWeightOfBottomTile = weightOfCurrentTile + weightOfBottomTile;

	if (newWeightOfBottomTile > 1.0f)
	{
		newWeightOfCurrentTile = newWeightOfBottomTile - 1.0f;
		newWeightOfBottomTile -= newWeightOfCurrentTile;
	}

	if (newWeightOfCurrentTile < 0.0f)
	{
		newWeightOfCurrentTile = 0.0f;
	}

	if (newWeightOfCurrentTile > 1.0f)
	{
		newWeightOfCurrentTile = 1.0f;
	}

	if (newWeightOfBottomTile < 0.0f)
	{
		newWeightOfBottomTile = 0.0f;
	}

	if (newWeightOfBottomTile > 1.0f)
	{
		newWeightOfBottomTile = 1.0f;
	}

	typesWater.weightCurrentTile = newWeightOfCurrentTile;
	typesWater.weightNextTile = newWeightOfBottomTile;

	return typesWater;
}

TwoTypeWater Map::GetTypeWaterInLeftRightTile(double weightOfCurrentTile, double weightOfLeftRightTile)
{
	double newWeightOfCurrentTile = weightOfCurrentTile;
	double newWeightOfLeftRightTile = weightOfLeftRightTile;

	if (weightOfLeftRightTile <= weightOfCurrentTile)
	{
		if (weightOfCurrentTile <= 0.1)
		{
			newWeightOfCurrentTile = 0;
			newWeightOfLeftRightTile = weightOfCurrentTile + weightOfLeftRightTile;
		}
		else
		{
			double weightForBoth = (weightOfCurrentTile + weightOfLeftRightTile) / 2;

			newWeightOfCurrentTile = weightForBoth;
			newWeightOfLeftRightTile = weightForBoth;
		}
	}

	if (newWeightOfCurrentTile < 0.0f)
	{
		newWeightOfCurrentTile = 0.0f;
	}

	if (newWeightOfCurrentTile > 1.0f)
	{
		newWeightOfCurrentTile = 1.0f;
	}

	if (newWeightOfLeftRightTile < 0.0f)
	{
		newWeightOfLeftRightTile = 0.0f;
	}

	if (newWeightOfLeftRightTile > 1.0f)
	{
		newWeightOfLeftRightTile = 1.0f;
	}

	TwoTypeWater typesWater;
	typesWater.weightCurrentTile = newWeightOfCurrentTile;
	typesWater.weightNextTile = newWeightOfLeftRightTile;

	return typesWater;
}

std::vector<BlockWater>::iterator Map::FindBlockWaterInOldVector(sf::Vector2f coordBlock)
{
	std::vector<BlockWater>::iterator iterBlockWater = blocksWater.end();
	for (std::vector<BlockWater>::iterator iter = blocksWater.begin(); iter < blocksWater.end(); iter++)
	{
		if (iter->coord == coordBlock)
		{
			iterBlockWater = iter;
			break;
		}
	}
	return iterBlockWater;
}

std::vector<BlockWater>::iterator Map::FindBlockWaterInNewVector(sf::Vector2f coordBlock)
{
	std::vector<BlockWater>::iterator iterBlockWater = newBlocksWater.end();
	for (std::vector<BlockWater>::iterator iter = newBlocksWater.begin(); iter < newBlocksWater.end(); iter++)
	{
		if (iter->coord == coordBlock)
		{
			iterBlockWater = iter;
			break;
		}
	}
	return iterBlockWater;
}

void Map::WaterDistribution(sf::Vector2f coord)
{
	std::vector<BlockWater>::iterator currentWater = FindBlockWaterInOldVector(coord);

	if (currentWater == blocksWater.end()) return;

	double massCurrentWater = currentWater->weight;

	if (generatedMap[coord.x + 1][coord.y] == TypeTile::Wall)
	{
		double weightOfCurrentBlockWater = currentWater->weight;

		sf::Vector2f coordBottomBlock = { coord.x + 1, coord.y };
		std::vector<BlockWater>::iterator bottomBlockWater = FindBlockWaterInOldVector(coordBottomBlock);

		double weightOfBottomBlockWater = 0;
		if (bottomBlockWater != blocksWater.end())
		{
			weightOfBottomBlockWater = bottomBlockWater->weight;
		}

		TwoTypeWater typesWater = GetTypeWaterInBottomTile(weightOfCurrentBlockWater, weightOfBottomBlockWater);

		massCurrentWater -= (typesWater.weightNextTile - weightOfBottomBlockWater);

		std::vector<BlockWater>::iterator iterForCurrentBlock = FindBlockWaterInNewVector(coord);

		if (iterForCurrentBlock == newBlocksWater.end())
		{
			BlockWater water;

			water.coord = coord;
			water.weight = typesWater.weightCurrentTile;

			water.block.setFillColor(sf::Color(0, 0, 255, 80));
			water.block.setSize({ WIDTH_TILE, float(water.weight * HEIGHT_TILE) });
			water.block.setPosition({ water.coord.y * HEIGHT_TILE,
				(water.coord.x + 1) * WIDTH_TILE - water.block.getSize().y });

			if (water.weight != 0)
			{
				currentWater->block.setSize(water.block.getSize());
				currentWater->block.setPosition(water.block.getPosition());
				currentWater->weight = water.weight;
				currentWater->coord = water.coord;

				newBlocksWater.push_back(water);
			}
			else
			{
				if (bottomBlockWater == blocksWater.end())
				{
					currentWater->weight = typesWater.weightNextTile;
					currentWater->coord = coordBottomBlock;
					currentWater->block.setSize({ WIDTH_TILE, float(currentWater->weight * HEIGHT_TILE) });
					currentWater->block.setPosition({ currentWater->coord.y * HEIGHT_TILE,
					(currentWater->coord.x + 1) * WIDTH_TILE - currentWater->block.getSize().y });
				}
				else
				{
					currentWater->block.setSize(water.block.getSize());
					currentWater->block.setPosition(water.block.getPosition());
					currentWater->weight = water.weight;
					currentWater->coord = water.coord;
				}
			}
		}
		else
		{
			iterForCurrentBlock->weight = typesWater.weightCurrentTile;
			iterForCurrentBlock->coord = coord;

			iterForCurrentBlock->block.setSize({ WIDTH_TILE, float(iterForCurrentBlock->weight * HEIGHT_TILE) });
			iterForCurrentBlock->block.setPosition({ iterForCurrentBlock->coord.y * HEIGHT_TILE,
				(iterForCurrentBlock->coord.x + 1) * WIDTH_TILE - iterForCurrentBlock->block.getSize().y });

			currentWater->block.setSize(iterForCurrentBlock->block.getSize());
			currentWater->block.setPosition(iterForCurrentBlock->block.getPosition());
			currentWater->weight = iterForCurrentBlock->weight;
			currentWater->coord = iterForCurrentBlock->coord;
		}

		std::vector<BlockWater>::iterator iterForBottomBlock = FindBlockWaterInNewVector(coordBottomBlock);

		if (iterForBottomBlock == newBlocksWater.end())
		{
			BlockWater bottomWater;

			bottomWater.weight = typesWater.weightNextTile;
			bottomWater.coord = coordBottomBlock;

			bottomWater.block.setFillColor(sf::Color(0, 0, 255, 80));
			bottomWater.block.setSize({ WIDTH_TILE, float(bottomWater.weight * HEIGHT_TILE) });

			bottomWater.block.setPosition({ bottomWater.coord.y * HEIGHT_TILE,
				(bottomWater.coord.x + 1) * WIDTH_TILE - bottomWater.block.getSize().y });

			if (bottomBlockWater != blocksWater.end())
			{
				bottomBlockWater->block.setSize(bottomWater.block.getSize());
				bottomBlockWater->block.setPosition(bottomWater.block.getPosition());
				bottomBlockWater->weight = bottomWater.weight;
				bottomBlockWater->coord = bottomWater.coord;
			}

			if (bottomWater.weight != 0)
			{
				newBlocksWater.push_back(bottomWater);
			}
		}
		else
		{
			iterForBottomBlock->weight = typesWater.weightNextTile;
			iterForBottomBlock->block.setSize({ WIDTH_TILE, float(iterForBottomBlock->weight * HEIGHT_TILE) });
			iterForBottomBlock->coord = coordBottomBlock;
			iterForBottomBlock->block.setPosition({ iterForBottomBlock->coord.y * HEIGHT_TILE,
				(iterForBottomBlock->coord.x + 1) * WIDTH_TILE - iterForBottomBlock->block.getSize().y });

			if (bottomBlockWater != blocksWater.end())
			{
				bottomBlockWater->block.setSize(iterForBottomBlock->block.getSize());
				bottomBlockWater->block.setPosition(iterForBottomBlock->block.getPosition());
				bottomBlockWater->weight = iterForBottomBlock->weight;
				bottomBlockWater->coord = iterForBottomBlock->coord;
			}
		}
	}

	if (massCurrentWater <= 0)
	{
		return;
	}

	if (generatedMap[coord.x][coord.y + 1] == TypeTile::Wall)
	{
		double weightOfCurrentBlockWater = currentWater->weight;

		sf::Vector2f coordRightBlock = { coord.x, coord.y + 1 };
		std::vector<BlockWater>::iterator rightBlockWater = FindBlockWaterInOldVector(coordRightBlock);

		double weightOfRightBlockWater = 0;
		if (rightBlockWater != blocksWater.end())
		{
			weightOfRightBlockWater = rightBlockWater->weight;
		}

		TwoTypeWater typesWater = GetTypeWaterInLeftRightTile(weightOfCurrentBlockWater, weightOfRightBlockWater);

		massCurrentWater -= (typesWater.weightNextTile - weightOfRightBlockWater);

		std::vector<BlockWater>::iterator iterForCurrentBlock = FindBlockWaterInNewVector(coord);

		if (iterForCurrentBlock == newBlocksWater.end())
		{
			BlockWater water;
			water.coord = coord;
			water.weight = typesWater.weightCurrentTile;

			water.block.setFillColor(sf::Color(0, 0, 255, 80));
			water.block.setSize({ WIDTH_TILE, float(water.weight * HEIGHT_TILE) });

			water.block.setPosition({ water.coord.y * HEIGHT_TILE,
				(water.coord.x + 1) * WIDTH_TILE - water.block.getSize().y });

			currentWater->block.setSize(water.block.getSize());
			currentWater->block.setPosition(water.block.getPosition());
			currentWater->weight = water.weight;
			currentWater->coord = water.coord;

			if (water.weight != 0)
			{
				newBlocksWater.push_back(water);
			}
		}
		else
		{
			iterForCurrentBlock->weight = typesWater.weightCurrentTile;
			iterForCurrentBlock->block.setSize({ WIDTH_TILE, float(iterForCurrentBlock->weight * HEIGHT_TILE) });
			iterForCurrentBlock->coord = coord;
			iterForCurrentBlock->block.setPosition({ iterForCurrentBlock->coord.y * HEIGHT_TILE,
				(iterForCurrentBlock->coord.x + 1) * WIDTH_TILE - iterForCurrentBlock->block.getSize().y });

			currentWater->block.setSize(iterForCurrentBlock->block.getSize());
			currentWater->block.setPosition(iterForCurrentBlock->block.getPosition());
			currentWater->weight = iterForCurrentBlock->weight;
			currentWater->coord = iterForCurrentBlock->coord;
		}

		std::vector<BlockWater>::iterator iterForRightBlock = FindBlockWaterInNewVector(coordRightBlock);

		if (iterForRightBlock == newBlocksWater.end())
		{
			BlockWater rightWater;
			rightWater.coord = coordRightBlock;
			rightWater.weight = typesWater.weightNextTile;

			rightWater.block.setFillColor(sf::Color(0, 0, 255, 80));
			rightWater.block.setSize({ WIDTH_TILE, float(rightWater.weight * HEIGHT_TILE) });

			rightWater.block.setPosition({ rightWater.coord.y * HEIGHT_TILE,
				(rightWater.coord.x + 1) * WIDTH_TILE - rightWater.block.getSize().y });

			if (rightBlockWater != blocksWater.end())
			{
				rightBlockWater->block.setSize(rightWater.block.getSize());
				rightBlockWater->block.setPosition(rightWater.block.getPosition());
				rightBlockWater->weight = rightWater.weight;
				rightBlockWater->coord = rightWater.coord;
			}
			else
			{
				BlockWater newRightWaterInOldVector;

				newRightWaterInOldVector.weight = rightWater.weight;
				newRightWaterInOldVector.coord = rightWater.coord;
				newRightWaterInOldVector.block.setSize(rightWater.block.getSize());
				newRightWaterInOldVector.block.setPosition(rightWater.block.getPosition());

				blocksWater.push_back(newRightWaterInOldVector);

				currentWater = FindBlockWaterInOldVector(coord);
			}
			
			if (rightWater.weight != 0)
			{
				newBlocksWater.push_back(rightWater);
			}
		}
		else
		{
			iterForRightBlock->weight = typesWater.weightNextTile;
			iterForRightBlock->block.setSize({ WIDTH_TILE, float(iterForRightBlock->weight * HEIGHT_TILE) });
			iterForRightBlock->coord = coordRightBlock;
			iterForRightBlock->block.setPosition({ iterForRightBlock->coord.y * HEIGHT_TILE,
				(iterForRightBlock->coord.x + 1) * WIDTH_TILE - iterForRightBlock->block.getSize().y });

			if (rightBlockWater != blocksWater.end())
			{
				rightBlockWater->block.setSize(iterForRightBlock->block.getSize());
				rightBlockWater->block.setPosition(iterForRightBlock->block.getPosition());
				rightBlockWater->weight = iterForRightBlock->weight;
				rightBlockWater->coord = iterForRightBlock->coord;
			}
			else
			{
				BlockWater newRightWaterInOldVector;

				newRightWaterInOldVector.weight = iterForRightBlock->weight;
				newRightWaterInOldVector.coord = iterForRightBlock->coord;
				newRightWaterInOldVector.block.setSize(iterForRightBlock->block.getSize());
				newRightWaterInOldVector.block.setPosition(iterForRightBlock->block.getPosition());

				blocksWater.push_back(newRightWaterInOldVector);

				currentWater = FindBlockWaterInOldVector(coord);
			}
		}
	}

	if (massCurrentWater <= 0)
	{
		return;
	}

	if (generatedMap[coord.x][coord.y - 1] == TypeTile::Wall)
	{
		double weightOfCurrentBlockWater = 0;
		weightOfCurrentBlockWater = currentWater->weight;

		sf::Vector2f coordLeftBlock = { coord.x, coord.y - 1 };
		std::vector<BlockWater>::iterator leftBlockWater = FindBlockWaterInOldVector(coordLeftBlock);

		double weightOfLeftBlockWater = 0;
		if (leftBlockWater != blocksWater.end())
		{
			weightOfLeftBlockWater = leftBlockWater->weight;
		}

		TwoTypeWater typesWater = GetTypeWaterInLeftRightTile(weightOfCurrentBlockWater, weightOfLeftBlockWater);

		massCurrentWater -= (typesWater.weightNextTile - weightOfLeftBlockWater);

		std::vector<BlockWater>::iterator iterForCurrentBlock = FindBlockWaterInNewVector(coord);

		if (iterForCurrentBlock == newBlocksWater.end())
		{
			BlockWater water;
			water.coord = { coord.x, coord.y };
			water.weight = typesWater.weightCurrentTile;

			water.block.setFillColor(sf::Color(0, 0, 255, 80));
			water.block.setSize({ WIDTH_TILE, float(water.weight * HEIGHT_TILE) });

			water.block.setPosition({ water.coord.y * HEIGHT_TILE,
				(water.coord.x + 1) * WIDTH_TILE - water.block.getSize().y });

			currentWater->block.setSize(water.block.getSize());
			currentWater->block.setPosition(water.block.getPosition());
			currentWater->weight = water.weight;
			currentWater->coord = water.coord;

			if (water.weight != 0)
			{
				newBlocksWater.push_back(water);
			}
		}
		else
		{
			iterForCurrentBlock->weight = typesWater.weightCurrentTile;
			iterForCurrentBlock->block.setSize({ WIDTH_TILE, float(iterForCurrentBlock->weight * HEIGHT_TILE) });
			iterForCurrentBlock->coord = { coord.x, coord.y };
			iterForCurrentBlock->block.setPosition({ iterForCurrentBlock->coord.y * HEIGHT_TILE,
				(iterForCurrentBlock->coord.x + 1) * WIDTH_TILE - iterForCurrentBlock->block.getSize().y });


			currentWater->block.setSize(iterForCurrentBlock->block.getSize());
			currentWater->block.setPosition(iterForCurrentBlock->block.getPosition());
			currentWater->weight = iterForCurrentBlock->weight;
			currentWater->coord = iterForCurrentBlock->coord;
		}

		std::vector<BlockWater>::iterator iterForLeftBlock = FindBlockWaterInNewVector(coordLeftBlock);

		if (iterForLeftBlock == newBlocksWater.end())
		{
			BlockWater leftWater;
			leftWater.coord = { coord.x, coord.y - 1 };;
			leftWater.weight = typesWater.weightNextTile;

			leftWater.block.setFillColor(sf::Color(0, 0, 255, 80));
			leftWater.block.setSize({ WIDTH_TILE, float(leftWater.weight * HEIGHT_TILE) });

			leftWater.block.setPosition({ leftWater.coord.y * HEIGHT_TILE,
				(leftWater.coord.x + 1) * WIDTH_TILE - leftWater.block.getSize().y });

			if (leftBlockWater != blocksWater.end())
			{
				leftBlockWater->block.setSize(leftWater.block.getSize());
				leftBlockWater->block.setPosition(leftWater.block.getPosition());
				leftBlockWater->weight = leftWater.weight;
				leftBlockWater->coord = leftWater.coord;
			}

			if (leftWater.weight != 0)
			{
				newBlocksWater.push_back(leftWater);
			}
		}
		else
		{
			iterForLeftBlock->weight = typesWater.weightNextTile;
			iterForLeftBlock->block.setSize({ WIDTH_TILE, float(iterForLeftBlock->weight * HEIGHT_TILE) });
			iterForLeftBlock->coord = { coord.x, coord.y - 1 };
			iterForLeftBlock->block.setPosition({ iterForLeftBlock->coord.y * HEIGHT_TILE,
				(iterForLeftBlock->coord.x + 1) * WIDTH_TILE - iterForLeftBlock->block.getSize().y });

			if (leftBlockWater != blocksWater.end())
			{
				leftBlockWater->block.setSize(iterForLeftBlock->block.getSize());
				leftBlockWater->block.setPosition(iterForLeftBlock->block.getPosition());
				leftBlockWater->weight = iterForLeftBlock->weight;
				leftBlockWater->coord = iterForLeftBlock->coord;
			}
		}
	}

	if (massCurrentWater <= 0)
	{
		return;
	}

	std::vector<BlockWater>::iterator currentIterInNewVector = FindBlockWaterInNewVector(currentWater->coord);

	if (currentIterInNewVector == newBlocksWater.end())
	{
		if (currentWater->weight != 0)
		{
			newBlocksWater.push_back({ currentWater->weight, currentWater->block, currentWater->coord });
		}
	}
}

void Map::MoveWater()
{
	if (timerForWater.getElapsedTime().asSeconds() < 0.1) return;

	for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
	{
		for (int iterX = WIDTH_MAP - 1; iterX >= 0; iterX--)
		{
			if (generatedMap[iterX][iterY] == TypeTile::Wall)
			{
				sf::Vector2f coord = { float(iterX), float(iterY) };
				WaterDistribution(coord);
			}
		}
	}

	blocksWater.clear();

	for (auto& waterInBlock : newBlocksWater)
	{
		if (waterInBlock.weight != 0)
		{
			blocksWater.push_back(waterInBlock);
		}
	}

	newBlocksWater.clear();
	
	timerForWater.restart();
}

void Map::CreatePortalToNextLevel()
{
	int xCoordForGame = Random(WIDTH_MAP / 2, WIDTH_MAP) * WIDTH_TILE;
	int yCoordForGame = Random(HEIGHT_MAP - 40, HEIGHT_MAP - 20) * HEIGHT_TILE;

	/*int xCoordForGame = 10 * 25;
	int yCoordForGame = 10 * 25;*/

	std::cout << xCoordForGame / 25 << " " << yCoordForGame / 25 << std::endl;

	teamOfPlayer.setTexture(teamOfPlayer_texture);
	teamOfPlayer.setTextureRect({ 0, 0, 92, 40 });
	teamOfPlayer.setPosition({ float(yCoordForGame), float(xCoordForGame)});
}

bool Map::DidPlayerFindTeam(sf::Vector2f coordPlayer)
{
	if (coordPlayer.x >= teamOfPlayer.getPosition().x && coordPlayer.x <= teamOfPlayer.getPosition().x + teamOfPlayer.getLocalBounds().width &&
		coordPlayer.y >= teamOfPlayer.getPosition().y && coordPlayer.y <= teamOfPlayer.getPosition().y + teamOfPlayer.getLocalBounds().height)
	{
		if (currentLevel == 3)
		{
			return true;
		}
		currentLevel++;
	}
	return false;
}

bool Map::CoordInWater(sf::Vector2f coord)
{
	bool coordInWater = false;
	for (auto& water : blocksWater)
	{
		if ((coord.x >= water.block.getPosition().x && coord.x <= water.block.getPosition().x + water.block.getLocalBounds().width) &&
			(coord.y >= water.block.getPosition().y && coord.y <= water.block.getPosition().y + water.block.getLocalBounds().height))
		{
			coordInWater = true;
			break;
		}
	}
	return coordInWater;
}

int Map::GetRadiusSpreadLight()
{
	return radiusSpreadLight;
}