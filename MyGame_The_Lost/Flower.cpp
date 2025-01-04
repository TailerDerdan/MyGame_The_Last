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
		m_map->GetTypeOfTile(iterX - 1, iterY) == TypeTile::Wall &&
		m_map->GetTypeOfTile(iterX - 1, iterY + 1) == TypeTile::Wall &&
		m_map->GetTypeOfTile(iterX - 1, iterY - 1) == TypeTile::Wall &&				
		m_map->GetTypeOfTile(iterX, iterY + 1) == TypeTile::Wall &&
		m_map->GetTypeOfTile(iterX, iterY - 1) == TypeTile::Wall &&
		m_map->GetTypeOfTile(iterX + 1, iterY) == TypeTile::Stone &&
		((m_map->GetTypeOfTile(iterX + 1, iterY - 1) == TypeTile::Stone && m_map->GetTypeOfTile(iterX + 1, iterY - 2) == TypeTile::Stone) ||
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
						std::cout << iterX << " " << iterY << " FRIENDLY" << std::endl;
						flowerType = FlowerType::Friendly;
						countOfCenterBlockFriendlyFlower--;
						flowerSprite.sprite.setTexture(m_textureForFlowers);
					}
					else
					{
						std::cout << iterX << " " << iterY << " ANGRY" << std::endl;
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
}

void Flower::Update()
{
	int index = 0;
	for (auto& flower : flowersSprite)
	{
		if (m_map->GetTypeOfTile(int(std::floor(flower.sprite.getPosition().y / 25) - 1), int(std::floor(flower.sprite.getPosition().x / 25))))
		{
			std::vector<FlowerSprite>::iterator iterForDelete = flowersSprite.begin() + index;
			flowersSprite.erase(iterForDelete);
		}
		index++;
	}
}

void Flower::DrawFlowers(sf::RenderTexture& castTexture)
{
	for (int iter = 0; iter < flowersSprite.size(); iter++)
	{
		castTexture.draw(flowersSprite[iter].sprite);
	}
}

bool Flower::IsPointInFlower(sf::Vector2f point, sf::Sprite flower)
{
	bool isPointMoreX = (point.x >= flower.getPosition().x);
	bool isPointMoreY = (point.y >= flower.getPosition().y);
	bool isPointLessWidthFlower = (point.x <= (flower.getPosition().x + flower.getGlobalBounds().width));
	bool isPointLessHeightFlower = (point.y <= (flower.getPosition().y + flower.getGlobalBounds().height));
	
	return (isPointMoreX && isPointMoreY && isPointLessWidthFlower && isPointLessHeightFlower);
}

bool Flower::IsCoordInAngryFlower(sf::Vector2f coord, float xCoordErosionShader, bool& isAngryFlower)
{
	int iterNecessary = -1;
	bool isFlower = false;
	for (int iter = 0; iter < flowersSprite.size(); iter++)
	{
		FlowerSprite flower = flowersSprite[iter];
		if (flower.flowerType == FlowerType::Friendly) continue;

		sf::Vector2f centerPoint = { coord.x + float(PLAYER_WIDTH) / 2, coord.y + float(PLAYER_HEIGHT) / 2 };
		sf::Vector2f centerTopPoint = { coord.x + float(PLAYER_WIDTH) / 2, coord.y };
		sf::Vector2f centerBottomPoint = { coord.x + float(PLAYER_WIDTH) / 2, coord.y + PLAYER_HEIGHT };
		sf::Vector2f centerRightPoint = { coord.x, coord.y + float(PLAYER_HEIGHT) / 2 };
		sf::Vector2f centerLeftPoint = { coord.x + PLAYER_WIDTH, coord.y + float(PLAYER_HEIGHT) / 2 };

		if (IsPointInFlower(centerPoint, flower.sprite) || IsPointInFlower(centerTopPoint, flower.sprite) || IsPointInFlower(centerBottomPoint, flower.sprite) ||
			IsPointInFlower(centerRightPoint, flower.sprite) || IsPointInFlower(centerLeftPoint, flower.sprite))
		{
			isFlower = true;
			if (xCoordErosionShader >= 0.8)
			{
				iterNecessary = iter;
				break;
			}
		}
	}

	if (iterNecessary != -1)
	{
		isAngryFlower = true;
		flowersSprite.erase(std::next(flowersSprite.begin(), iterNecessary));
	}

	return isFlower;
}

bool Flower::IsCoordInFriendlyFlower(sf::Vector2f coord)
{
	int iterNecessary = -1;
	bool isFlower = false;
	for (int iter = 0; iter < flowersSprite.size(); iter++)
	{
		FlowerSprite flower = flowersSprite[iter];
		if (flower.flowerType == FlowerType::Angry) continue;

		sf::Vector2f centerPoint = { coord.x + float(PLAYER_WIDTH) / 2, coord.y + float(PLAYER_HEIGHT) / 2 };
		sf::Vector2f centerTopPoint = { coord.x + float(PLAYER_WIDTH) / 2, coord.y };
		sf::Vector2f centerBottomPoint = { coord.x + float(PLAYER_WIDTH) / 2, coord.y + PLAYER_HEIGHT };
		sf::Vector2f centerRightPoint = { coord.x, coord.y + float(PLAYER_HEIGHT) / 2 };
		sf::Vector2f centerLeftPoint = { coord.x + PLAYER_WIDTH, coord.y + float(PLAYER_HEIGHT) / 2 };

		if (IsPointInFlower(centerPoint, flower.sprite) || IsPointInFlower(centerTopPoint, flower.sprite) || IsPointInFlower(centerBottomPoint, flower.sprite) ||
			IsPointInFlower(centerRightPoint, flower.sprite) || IsPointInFlower(centerLeftPoint, flower.sprite))
		{
			iterNecessary = iter;
			isFlower = true;
			break;
		}
	}

	if (iterNecessary != -1)
	{
		flowersSprite.erase(std::next(flowersSprite.begin(), iterNecessary));
	}

	return isFlower;
}