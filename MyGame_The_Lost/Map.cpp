#include "Map.h"

Map::Map(const sf::View& view, sf::RenderTexture& castTexture, sf::Texture& textureOfCaveOuter, sf::RenderTexture& castTextureForWater)
{
	mapOfTexture.resize(WIDTH_MAP * HEIGHT_MAP + 1);

	mapOfTileOfLight = std::vector<bool>(WIDTH_MAP * HEIGHT_MAP, false);
	mapOfTileInBool = std::vector<bool>(WIDTH_MAP * HEIGHT_MAP, false);

	textureOfCave = textureOfCaveOuter;

	GenerateMap(4);
	CreateTileForMap();
	GenerateRandomWater();
	UpdateMap(view, castTexture, castTextureForWater);

	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			int numberOfTile = iterX * HEIGHT_MAP + iterY;
			if (generatedMap[iterX][iterY] == TypeTile::Stone)
			{
				mapOfTileInBool[numberOfTile] = true;
			}
		}
	}

	timerForWater.restart();
}

int Map::Random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return dist(rng);
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

void Map::GenerateMap(int countOfIteration)
{
	SetRandomGeneration();
	for (int iter = 0; iter < countOfIteration; iter++)
	{
		GetNextIteration();
	}
}

void Map::FillWaterFromCell(sf::Vector2f coord, int radius)
{
	if (coord.y == -1 || coord.x == -1 || coord.y == HEIGHT_MAP || coord.x == WIDTH_MAP) return;

	if (generatedMap[coord.x][coord.y] == TypeTile::Water10 || generatedMap[coord.x][coord.y] == TypeTile::WaterInStone) return;

	if (generatedMap[coord.x][coord.y] == TypeTile::Stone)
	{
		generatedMap[coord.x][coord.y] = TypeTile::WaterInStone;
		coordsOfBlockWater.push_back(std::pair(TypeTile::WaterInStone, coord));
	}
	else
	{
		generatedMap[coord.x][coord.y] = TypeTile::Water10;
		coordsOfBlockWater.push_back(std::pair(TypeTile::Water10, coord));
	}
	countOfWater++;
	
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

void Map::GenerateRandomWater()
{
	/*int countOfMainBlockWater = 7;
	for (int iterX = 1; iterX < WIDTH_MAP - 1; iterX++)
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

	SpreadWater(sf::Vector2f(float(10), float(10)));
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
	if (generatedMap[params.iterX][params.iterY] == TypeTile::Wall && mapOfTexture[params.iterForVector])
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

void Map::SetParamsForWater10(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::Water10 && mapOfTexture[params.iterForVector])
	{

		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 1.0f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::Water10;

		mapOfTexture[params.iterForVector]->texture = { 50, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

void Map::SetParamsForWater8(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::Water8 && mapOfTexture[params.iterForVector])
	{

		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 0.8f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::Water8;

		mapOfTexture[params.iterForVector]->texture = { 75, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

void Map::SetParamsForWater5(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::Water5 && mapOfTexture[params.iterForVector])
	{

		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 0.5f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::Water5;

		mapOfTexture[params.iterForVector]->texture = { 100, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

void Map::SetParamsForWater3(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::Water3 && mapOfTexture[params.iterForVector])
	{

		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 0.3f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::Water3;

		mapOfTexture[params.iterForVector]->texture = { 125, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

void Map::SetParamsForWater1(ParamsForTile params)
{
	if (generatedMap[params.iterX][params.iterY] == TypeTile::Water1 && mapOfTexture[params.iterForVector])
	{

		mapOfTexture[params.iterForVector]->number = params.numberOfTile;

		mapOfTexture[params.iterForVector]->weight = 0.1f;

		mapOfTexture[params.iterForVector]->typeTile = TypeTile::Water1;

		mapOfTexture[params.iterForVector]->texture = { 150, 0, WIDTH_TILE, HEIGHT_TILE };
		mapOfTexture[params.iterForVector]->sprite.setTextureRect(mapOfTexture[params.iterForVector]->texture);

		mapOfTexture[params.iterForVector]->sprite.setPosition({ float(params.xCoord * WIDTH_TILE), float(params.yCoord * HEIGHT_TILE) });
	}
}

//TODO: декомпозировать функцию, так чтобы строк было небольше 20 и небольше 2 вложенностей
void Map::UpdateMap(const sf::View& view, sf::RenderTexture& castTexture, sf::RenderTexture& castTextureForWater)
{
	sf::Vector2f viewPosition = sf::Vector2f{ view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 };

	int iterForVector = 0;

	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		m_iterX = iterX;
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			int numberOfTile = iterX * HEIGHT_MAP + iterY;
			int yCoord = iterX;
			int xCoord = iterY;

			m_iterY = iterY;

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
				SetParamsForWater10(params);
				SetParamsForWater8(params);
				SetParamsForWater5(params);
				SetParamsForWater3(params);
				SetParamsForWater1(params);

				if (mapOfTexture[iterForVector])
				{
					castTexture.draw(mapOfTexture[iterForVector]->sprite);
				}
				
				iterForVector++;
			}
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

void Map::SpreadTheLight(sf::Vector2f& coord, int& radius, bool isFirstTimeOfSpreadLight)
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
	previousRadiusOfSpreadLight = radius;

	FillFromCell(coord, radius);
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

	generatedMap[coordOfTile.x - 1][coordOfTile.y] = TypeTile::Wall;
	generatedMap[coordOfTile.x][coordOfTile.y] = TypeTile::Stone;
}

TypeTile Map::GetTypeWaterByWeight(float weight)
{
	if (weight <= 0.05f)
	{
		return TypeTile::Wall;
	}
	if (weight > 0.05f && weight < 0.2f)
	{
		return TypeTile::Water1;
	}
	if (weight >= 0.2f && weight < 0.4f)
	{
		return TypeTile::Water3;
	}
	if (weight >= 0.4f && weight < 0.6f)
	{
		return TypeTile::Water5;
	}
	if (weight >= 0.6f && weight < 0.8f)
	{
		return TypeTile::Water8;
	}
	return TypeTile::Water10;
}

float Map::GetWeightByTypeWater(TypeTile typeWater)
{
	switch (typeWater)
	{
	case Wall:
		return 0.0f;
	case Stone:
		return 2.0f;
	case Water1:
		return 0.1f;
	case Water3:
		return 0.3f;
	case Water5:
		return 0.5f;
	case Water8:
		return 0.8f;
	case Water10:
		return 1.0f;
	case WaterInStone:
		return 3.0f;
	default:
		break;
	}
}

TwoTypeWater Map::GetTypeWaterInBottomTile(TypeTile typeWaterOfCurrentTile, TypeTile typeWaterOfBottomTile)
{
	float weightOfCurrentTile = GetWeightByTypeWater(typeWaterOfCurrentTile);
	float weightOfBottomTile = GetWeightByTypeWater(typeWaterOfBottomTile);

	float dif = weightOfCurrentTile - weightOfBottomTile;
	
	float newWeightOfCurrentTile = 0.0f;
	float newWeightOfBottomTile = 0.0f;

	if (dif > 0.0f)
	{
		newWeightOfCurrentTile = weightOfCurrentTile - dif;
		newWeightOfBottomTile = weightOfBottomTile + dif;

		std::cout << newWeightOfCurrentTile << " " << newWeightOfBottomTile << std::endl;
	}

	if (dif <= 0.0f)
	{
		newWeightOfCurrentTile = 0.0f;
		newWeightOfBottomTile = weightOfBottomTile + weightOfCurrentTile;
		if (newWeightOfBottomTile > 1.0f)
		{
			dif = 1.0 - newWeightOfBottomTile;
			newWeightOfCurrentTile = dif;
		}
	}

	TwoTypeWater typesWater;
	typesWater.typeWaterCurrentTile = GetTypeWaterByWeight(newWeightOfCurrentTile);
	typesWater.typeWaterNextTile = GetTypeWaterByWeight(newWeightOfBottomTile);

	return typesWater;
}

TwoTypeWater Map::GetTypeWaterInLeftRightTile(TypeTile typeWaterOfCurrentTile, TypeTile typeWaterOfLeftRightTile)
{
	float weightOfCurrentTile = GetWeightByTypeWater(typeWaterOfCurrentTile);
	float weightOfLeftRightTile = GetWeightByTypeWater(typeWaterOfLeftRightTile);

	float newWeightOfCurrentTile = weightOfCurrentTile;
	float newWeightOfLeftRightTile = weightOfLeftRightTile;

	if (weightOfLeftRightTile < weightOfCurrentTile)
	{
		float weightForBoth = (weightOfCurrentTile + weightOfLeftRightTile) / 2;
		newWeightOfCurrentTile = weightForBoth;
		newWeightOfLeftRightTile = weightForBoth;
	}

	TwoTypeWater typesWater;
	typesWater.typeWaterCurrentTile = GetTypeWaterByWeight(newWeightOfCurrentTile);
	typesWater.typeWaterNextTile = GetTypeWaterByWeight(newWeightOfLeftRightTile);

	return typesWater;
}

void Map::WaterDistribution(sf::Vector2f coord)
{
	if (generatedMap[coord.x + 1][coord.y] != TypeTile::Stone && generatedMap[coord.x + 1][coord.y] != TypeTile::WaterInStone)
	{
		TypeTile typeWaterCurrentTile = TypeTile(generatedMap[coord.x][coord.y]);
		TypeTile typeWaterBottomTile = TypeTile(generatedMap[coord.x + 1][coord.y]);

		TwoTypeWater typesWater = GetTypeWaterInBottomTile(typeWaterCurrentTile, typeWaterBottomTile);

		generatedMap[coord.x][coord.y] = typesWater.typeWaterCurrentTile;
		generatedMap[coord.x + 1][coord.y] = typesWater.typeWaterNextTile;
	}

	if (generatedMap[coord.x][coord.y + 1] != TypeTile::Stone && generatedMap[coord.x][coord.y + 1] != TypeTile::WaterInStone)
	{
		TypeTile typeWaterCurrentTile = TypeTile(generatedMap[coord.x][coord.y]);
		TypeTile typeWaterRightTile = TypeTile(generatedMap[coord.x][coord.y + 1]);

		TwoTypeWater typesWater = GetTypeWaterInLeftRightTile(typeWaterCurrentTile, typeWaterRightTile);

		generatedMap[coord.x][coord.y] = typesWater.typeWaterCurrentTile;
		generatedMap[coord.x][coord.y + 1] = typesWater.typeWaterNextTile;
	}

	if (generatedMap[coord.x][coord.y - 1] != TypeTile::Stone && generatedMap[coord.x][coord.y - 1] != TypeTile::WaterInStone)
	{
		TypeTile typeWaterCurrentTile = TypeTile(generatedMap[coord.x][coord.y]);
		TypeTile typeWaterLeftTile = TypeTile(generatedMap[coord.x][coord.y - 1]);

		TwoTypeWater typesWater = GetTypeWaterInLeftRightTile(typeWaterCurrentTile, typeWaterLeftTile);

		generatedMap[coord.x][coord.y] = typesWater.typeWaterCurrentTile;
		generatedMap[coord.x][coord.y - 1] = typesWater.typeWaterNextTile;
	}
}

void Map::MoveWater()
{
	if (timerForWater.getElapsedTime().asSeconds() < 1) return;

	for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
	{
		for (int iterX = WIDTH_MAP - 1; iterX >= 0; iterX--)
		{
			if (generatedMap[iterX][iterY] == TypeTile::Water10 || 
				generatedMap[iterX][iterY] == TypeTile::Water8  ||
				generatedMap[iterX][iterY] == TypeTile::Water5  ||
				generatedMap[iterX][iterY] == TypeTile::Water3  ||
				generatedMap[iterX][iterY] == TypeTile::Water1)
			{
				sf::Vector2f coord = { float(iterX), float(iterY) };
				WaterDistribution(coord);
			}
		}
	}
	
	timerForWater.restart();
}