#include "Map.h"

Map::Map(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, const sf::View& view, sf::RenderTexture& castTexture, sf::Texture& textureOfCaveOuter)
{
	mapOfTexture.resize(WIDTH_MAP * HEIGHT_MAP + 1);

	mapOfTileOfLight = std::vector<bool>(WIDTH_MAP * HEIGHT_MAP, false);
	mapOfTileInBool = std::vector<bool>(WIDTH_MAP * HEIGHT_MAP, false);

	textureOfCave = textureOfCaveOuter;

	GenerateMap(4);
	CreateTileForMap();
	UpdateMap(WIDTH_WINDOW, HEIGHT_WINDOW, view, castTexture);

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
}

int Map::GetCountOfNeighbor(sf::Vector2i coordOfTile)
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

void Map::SetRandomGeneration()
{
	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			int randomNumber = rand() % 100;
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
	for (int iterX = 0; iterX < WIDTH_MAP; iterX++)
	{
		for (int iterY = 0; iterY < HEIGHT_MAP; iterY++)
		{
			int countOfNeighbor = GetCountOfNeighbor({ iterX, iterY });
			
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

void Map::CreateTileForMap()
{
	for (int iter = 0; iter < WIDTH_MAP * HEIGHT_MAP; iter++)
	{
		Tile* tile = new Tile();
		tile->sprite.setTexture(textureOfCave);
		mapOfTexture.at(iter) = tile;
	}
}

//TODO: декомпозировать функцию, так чтобы строк было небольше 20 и небольше 2 вложенностей
void Map::UpdateMap(const int WIDTH_WINDOW, const int HEIGHT_WINDOW, const sf::View& view, sf::RenderTexture& castTexture)
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

			if ((xCoord * WIDTH_TILE >= viewPosition.x && xCoord * WIDTH_TILE <= WIDTH_WINDOW + viewPosition.x) &&
				(yCoord * HEIGHT_TILE >= viewPosition.y && yCoord * HEIGHT_TILE <= HEIGHT_WINDOW + viewPosition.y))
			{
				if (generatedMap[iterX][iterY] == TypeTile::Wall)
				{
					mapOfTexture[iterForVector]->number = numberOfTile;
					
					mapOfTexture[iterForVector]->typeTile = TypeTile::Wall;

					mapOfTexture[iterForVector]->texture = { 25, 0, WIDTH_TILE, HEIGHT_TILE };
					mapOfTexture[iterForVector]->sprite.setTextureRect(mapOfTexture[iterForVector]->texture);

					mapOfTexture[iterForVector]->sprite.setPosition({ float(xCoord * WIDTH_TILE), float(yCoord * HEIGHT_TILE) });

				}
				if (generatedMap[iterX][iterY] == TypeTile::Stone)
				{
					mapOfTexture[iterForVector]->number = numberOfTile;

					mapOfTexture[iterForVector]->typeTile = TypeTile::Stone;

					mapOfTexture[iterForVector]->texture = { 0, 0, WIDTH_TILE, HEIGHT_TILE };
					mapOfTexture[iterForVector]->sprite.setTextureRect(mapOfTexture[iterForVector]->texture);

					mapOfTexture[iterForVector]->sprite.setPosition({ float(xCoord * WIDTH_TILE), float(yCoord * HEIGHT_TILE) });
				}
				castTexture.draw(mapOfTexture[iterForVector]->sprite);
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
	generatedMap[coordOfTile.x][coordOfTile.y] = TypeTile::Wall;
	mapOfTileInBool[numberOfTile] = 0;
}