#include "Flower.h"

Flower::Flower(Map* map, sf::Texture& textureForFlowers)
{
	m_map = map;
	MakeRandomGeneration(COUNT_FRIENDLY_FLOWER, COUNT_ANGRY_FLOWER);
	m_textureForFlowers = textureForFlowers;
	m_textureOfFlowerAngry = textureForFlowers;
}

int Flower::Random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return dist(rng);

	/*int randomInt = rand() % 100;

	return randomInt;*/
}

std::pair<bool, bool> Flower::IsPlaceForFlower(int iterX, int iterY)
{
	if (m_map->GetTypeOfTile(iterX, iterY) == TypeTile::Wall &&
		m_map->GetTypeOfTile(iterX + 1, iterY) == TypeTile::Stone &&
		m_map->GetTypeOfTile(iterX - 1, iterY) == TypeTile::Wall &&
		m_map->GetTypeOfTile(iterX - 1, iterY + 1) == TypeTile::Wall &&
		m_map->GetTypeOfTile(iterX, iterY + 1) == TypeTile::Wall &&
		m_map->GetTypeOfTile(iterX, iterY - 1) == TypeTile::Wall &&
		((m_map->GetTypeOfTile(iterX + 1, iterY - 1) == TypeTile::Stone && m_map->GetTypeOfTile(iterX + 1, iterY - 2) == TypeTile::Stone) &&
		(m_map->GetTypeOfTile(iterX + 1, iterY + 1) == TypeTile::Stone && m_map->GetTypeOfTile(iterX + 1, iterY + 2) == TypeTile::Stone)))
	{
		if ((m_map->GetTypeOfTile(iterX + 1, iterY - 1) == TypeTile::Stone && m_map->GetTypeOfTile(iterX + 1, iterY - 2) == TypeTile::Stone))
		{
			return std::pair(true, true);
		}
		return std::pair(true, false);
	}
	return std::pair(false, false);
}

void Flower::MakeRandomGeneration(int countOfFlowerFriendlyBlock, int countOfFlowerAngryBlock)
{
	int countOfCenterBlockFriendlyFlower = countOfFlowerFriendlyBlock;
	int countOfCenterBlockAngryFlower = countOfFlowerAngryBlock;
	for (int iterX = 1; iterX < WIDTH_MAP - 1; iterX++)
	{
		if (countOfCenterBlockFriendlyFlower == 0 || countOfCenterBlockAngryFlower == 0) return;

		for (int iterY = 1; iterY < HEIGHT_MAP - 1; iterY++)
		{
			if ((iterX >= -1 && iterX <= 1) || (iterY >= -1 && iterY <= 1) || iterY == HEIGHT_MAP - 1 || iterX == WIDTH_MAP - 1) continue;

			std::pair<bool, bool> stateAboutPositionFlower = IsPlaceForFlower(iterX, iterY);
			if (stateAboutPositionFlower.first)
			{
				if (countOfCenterBlockFriendlyFlower == 0 || countOfCenterBlockAngryFlower == 0) continue;

				int randomNumber = Random(0, 10000);

				if (randomNumber >= 9000)
				{
					int randomNumberForTypeFlower = Random(0, 100);

					FlowerType flowerType;

					FlowerSprite flowerSprite;
					if (randomNumberForTypeFlower > 60)
					{
						flowerType = FlowerType::Friendly;
						countOfCenterBlockFriendlyFlower--;
						flowerSprite.sprite.setTexture(m_textureForFlowers);
					}
					else
					{
						std::cout << iterX << " " << iterY << std::endl;
						flowerType = FlowerType::Angry;
						countOfCenterBlockAngryFlower--;
						flowerSprite.sprite.setTexture(m_textureOfFlowerAngry);
					}
					
					flowerSprite.texture = { 0, 0, WIDTH_TILE, HEIGHT_TILE };
					flowerSprite.sprite.setTextureRect(flowerSprite.texture);
					flowerSprite.sprite.setPosition({ float(iterY * HEIGHT_TILE), float(iterX * WIDTH_TILE) });
					flowerSprite.flowerType = flowerType;

					flowersSprite.push_back(flowerSprite);

					if (flowerSprite.flowerType == FlowerType::Angry) continue;

					BlockWater water;

					if (stateAboutPositionFlower.second)
					{
						water.coord = { float(iterX + 1), float(iterY - 1) };
						water.block.setFillColor(sf::Color(0, 0, 255, 80));
						water.block.setSize({ float(WIDTH_TILE), float(HEIGHT_TILE) });
						water.block.setPosition({ float((iterY - 1) * HEIGHT_TILE), float((iterX + 1) * WIDTH_TILE) });
						water.weight = 1.0;
					}
					else
					{
						water.coord = { float(iterX + 1), float(iterY + 1) };
						water.block.setFillColor(sf::Color(0, 0, 255, 80));
						water.block.setSize({ float(WIDTH_TILE), float(HEIGHT_TILE) });
						water.block.setPosition({ float((iterY + 1) * HEIGHT_TILE), float((iterX + 1) * WIDTH_TILE) });
						water.weight = 1.0;
					}

					m_map->AddBlockWater(water);
				}
			}
		}
	}

	FlowerSprite flowerSprite;
	flowerSprite.texture = { 0, 0, WIDTH_TILE, HEIGHT_TILE };
	flowerSprite.sprite.setTexture(m_textureOfFlowerAngry);
	flowerSprite.sprite.setTextureRect(flowerSprite.texture);
	flowerSprite.sprite.setPosition({ float(15 * WIDTH_TILE), float(10 * HEIGHT_TILE) });
	flowerSprite.flowerType = FlowerType::Angry;

	flowersSprite.push_back(flowerSprite);
}

void Flower::DrawFlowers(sf::RenderTexture& castTexture)
{
	for (int iter = 0; iter < flowersSprite.size(); iter++)
	{
		castTexture.draw(flowersSprite[iter].sprite);
	}
}

bool Flower::IsCoordInAngryFlower(sf::Vector2f coord, float xCoordErosionShader)
{
	int iterNecessary = -1;
	bool isFlower = false;
	for (int iter = 0; iter < flowersSprite.size(); iter++)
	{
		FlowerSprite flower = flowersSprite[iter];
		if (flower.flowerType == FlowerType::Friendly) continue;

		if (((coord.x >= flower.sprite.getPosition().x && coord.x <= flower.sprite.getPosition().x + flower.sprite.getLocalBounds().width &&
			coord.y >= flower.sprite.getPosition().y && coord.y <= flower.sprite.getPosition().y + flower.sprite.getLocalBounds().height)) ||

			((coord.x + PLAYER_WIDTH >= flower.sprite.getPosition().x && coord.x + PLAYER_WIDTH <= flower.sprite.getPosition().x + flower.sprite.getLocalBounds().width &&
			coord.y + PLAYER_HEIGHT >= flower.sprite.getPosition().y && coord.y + PLAYER_HEIGHT <= flower.sprite.getPosition().y + flower.sprite.getLocalBounds().height)) ||
			
			((coord.x + float(WIDTH_TILE) / 2 >= flower.sprite.getPosition().x && coord.x + float(PLAYER_WIDTH) / 2 <= flower.sprite.getPosition().x + flower.sprite.getLocalBounds().width &&
				coord.y + float(HEIGHT_TILE) / 2 >= flower.sprite.getPosition().y && coord.y + float(PLAYER_HEIGHT) / 2 <= flower.sprite.getPosition().y + flower.sprite.getLocalBounds().height))
			)
		{
			if (xCoordErosionShader >= 0.8)
			{
				iterNecessary = iter;
				break;
			}
			isFlower = true;
		}
	}

	if (iterNecessary != -1)
	{
		flowersSprite.erase(std::next(flowersSprite.begin(), iterNecessary));
	}

	return isFlower;
}

bool Flower::IsCoordInFriendlyFlower(sf::Vector2f coord)
{
	for (auto& flower : flowersSprite)
	{
		if (flower.flowerType == FlowerType::Angry) continue;

		if (coord.x >= flower.sprite.getPosition().x && coord.x <= flower.sprite.getPosition().x + flower.sprite.getLocalBounds().width &&
			coord.y >= flower.sprite.getPosition().y && coord.y <= flower.sprite.getPosition().y + flower.sprite.getLocalBounds().height)
		{
			return true;
		}
	}
	return false;
}