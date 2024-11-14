#include "Map.h"

Map::Map(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, sf::Vector2f viewPosition)
{
	textureOfWall.loadFromFile("../assets/wall.jpg");
	textureOfStone.loadFromFile("../assets/stone.jpg");

	GenerateMap(4);
	UpdateMap(WIDTH_WINDOW, HEIGHT_WINDOW, viewPosition);
	previousCoordView = viewPosition;
}

sf::Vector2i Map::GetCountOfNeighbor(sf::Vector2i coordOfTile)
{
	sf::Vector2i countOfNeighbor;

	bool isTileOnBorderTop = false;
	bool isTileOnBorderBottom = false;

	bool isTileOnBorderLeft = false;
	bool isTileOnBorderRight = false;

	if (coordOfTile.x == 0)
	{
		isTileOnBorderLeft = true;
		countOfNeighbor.x += 3;
	}
	else if (coordOfTile.x == WIDTH_MAP)
	{
		isTileOnBorderRight = true;
		countOfNeighbor.x += 3;
	}

	if (coordOfTile.y == 0)
	{
		isTileOnBorderTop = true;
		countOfNeighbor.x += 3;
	}
	else if (coordOfTile.y == HEIGHT_MAP)
	{
		isTileOnBorderBottom = true;
		countOfNeighbor.x += 3;
	}

	if ((coordOfTile.x == 0 && coordOfTile.y == 0) || (coordOfTile.x == WIDTH_MAP && coordOfTile.y == HEIGHT_MAP))
	{
		countOfNeighbor.x--;
	}

	if (!isTileOnBorderLeft && !isTileOnBorderTop && generatedMap[coordOfTile.x - 1][coordOfTile.y - 1] == TypeTile::Wall)
	{
		countOfNeighbor.x += 1;
	}
	if (!isTileOnBorderLeft && !isTileOnBorderTop && generatedMap[coordOfTile.x - 1][coordOfTile.y - 1] == TypeTile::Stone)
	{
		countOfNeighbor.y += 1;
	}

	if (!isTileOnBorderTop && generatedMap[coordOfTile.x][coordOfTile.y - 1] == TypeTile::Wall)
	{
		countOfNeighbor.x += 1;
	}
	if (!isTileOnBorderTop && generatedMap[coordOfTile.x][coordOfTile.y - 1] == TypeTile::Stone)
	{
		countOfNeighbor.y += 1;
	}

	if (!isTileOnBorderRight && !isTileOnBorderTop && generatedMap[coordOfTile.x + 1][coordOfTile.y - 1] == TypeTile::Wall)
	{
		countOfNeighbor.x += 1;
	}
	if (!isTileOnBorderRight && !isTileOnBorderTop && generatedMap[coordOfTile.x + 1][coordOfTile.y - 1] == TypeTile::Stone)
	{
		countOfNeighbor.y += 1;
	}

	if (!isTileOnBorderLeft && generatedMap[coordOfTile.x - 1][coordOfTile.y] == TypeTile::Wall)
	{
		countOfNeighbor.x += 1;
	}
	if (!isTileOnBorderLeft && generatedMap[coordOfTile.x - 1][coordOfTile.y] == TypeTile::Stone)
	{
		countOfNeighbor.y += 1;
	}

	if (!isTileOnBorderRight && generatedMap[coordOfTile.x + 1][coordOfTile.y] == TypeTile::Wall)
	{
		countOfNeighbor.x += 1;
	}
	if (!isTileOnBorderRight && generatedMap[coordOfTile.x + 1][coordOfTile.y] == TypeTile::Stone)
	{
		countOfNeighbor.y += 1;
	}

	if (!isTileOnBorderLeft && !isTileOnBorderBottom && generatedMap[coordOfTile.x - 1][coordOfTile.y + 1] == TypeTile::Wall)
	{
		countOfNeighbor.x += 1;
	}
	if (!isTileOnBorderLeft && !isTileOnBorderBottom && generatedMap[coordOfTile.x - 1][coordOfTile.y + 1] == TypeTile::Stone)
	{
		countOfNeighbor.y += 1;
	}

	if (!isTileOnBorderBottom && generatedMap[coordOfTile.x][coordOfTile.y + 1] == TypeTile::Wall)
	{
		countOfNeighbor.x += 1;
	}
	if (!isTileOnBorderBottom && generatedMap[coordOfTile.x][coordOfTile.y + 1] == TypeTile::Stone)
	{
		countOfNeighbor.y += 1;
	}

	if (!isTileOnBorderRight && !isTileOnBorderBottom && generatedMap[coordOfTile.x + 1][coordOfTile.y + 1] == TypeTile::Wall)
	{
		countOfNeighbor.x += 1;
	}
	if (!isTileOnBorderRight && !isTileOnBorderBottom && generatedMap[coordOfTile.x + 1][coordOfTile.y + 1] == TypeTile::Stone)
	{
		countOfNeighbor.y += 1;
	}

	return countOfNeighbor;
}

void Map::SetRandomGeneration()
{
	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			int randomNumber = rand() % 100;
			if (randomNumber < 54)
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
	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			sf::Vector2i countOfNeighbor = GetCountOfNeighbor({ iterX, iterY });
			if (countOfNeighbor.x > 4)
			{
				newGeneratedMap[iterX][iterY] = TypeTile::Wall;
			}
			else
			{
				newGeneratedMap[iterX][iterY] = TypeTile::Stone;
			}

		}
	}

	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
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

void Map::UpdateMap(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, sf::Vector2f viewPosition)
{
	sf::Vector2f startPosition = { 0.f, 0.f };
	if (previousCoordView == viewPosition && previousCoordView != startPosition)
	{
		return;
	}
	mapOfTexture.clear();
	previousCoordView = viewPosition;
	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			int numberOfTile = iterX * WIDTH_MAP + iterY;
			int yCoord = std::floor(numberOfTile / WIDTH_MAP);
			int xCoord = numberOfTile - WIDTH_MAP * yCoord;

			/*if (xCoord * WIDTH_TILE < viewPosition.x || xCoord * WIDTH_TILE > WIDTH_WINDOW)
			{
				continue;
			}

			if (yCoord * HEIGHT_TILE < viewPosition.y || yCoord * HEIGHT_TILE > HEIGHT_WINDOW)
			{
				continue;
			}*/

			if (generatedMap[iterX][iterY] == TypeTile::Wall)
			{
				Tile* tile = new Tile();
				tile->number = numberOfTile;

				tile->typeTile = TypeTile::Wall;

				tile->sprite.setTexture(textureOfWall);
				tile->texture = { 0, 0, WIDTH_TILE, HEIGHT_TILE };
				tile->sprite.setTextureRect(tile->texture);

				tile->sprite.setPosition({ float(xCoord * WIDTH_TILE), float(yCoord * HEIGHT_TILE) });

				mapOfTexture.push_back(tile);
			}
			if (generatedMap[iterX][iterY] == TypeTile::Stone)
			{
				Tile* tile = new Tile();
				tile->number = numberOfTile;

				tile->typeTile = TypeTile::Stone;

				tile->sprite.setTexture(textureOfStone);
				tile->texture = { 0, 0, WIDTH_TILE, HEIGHT_TILE };
				tile->sprite.setTextureRect(tile->texture);

				tile->sprite.setPosition({ float(xCoord * WIDTH_TILE), float(yCoord * HEIGHT_TILE) });

				mapOfTexture.push_back(tile);
			}
		}
	}
}

void Map::DrawMap(sf::RenderWindow& window)
{
	for (auto tile : mapOfTexture)
	{
		window.draw(tile->sprite);
	}
}