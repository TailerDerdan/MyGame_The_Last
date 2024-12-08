#include "Disaster.h"

Disaster::Disaster(Map* map, sf::View view)
{
	MakeTableOfWeight();
	m_map = map;
	m_view = m_view;
	centerView = view.getCenter();
}

void Disaster::MakeRandomDisaster(sf::Vector2f playerCoord, bool isPlayerMovementToRight)
{
	/*int R = rand() % sumOfAllWeight;
	int T = 0;

	for (auto& disaster : tableOFWeightOfDisaster)
	{
		T += disaster.second;
		if (T > R)
		{
			WriteDisaster(disaster.first);
			break;
		}
	}*/

	DoRockfall(playerCoord, isPlayerMovementToRight);
}

void Disaster::WriteDisaster(TypeOfDisaster disaster)
{
	switch (disaster)
	{
	case None:
		break;
	case Rockfall:
		std::cout << "Rockfall" << std::endl;
		break;
	case Siren:
		std::cout << "Siren" << std::endl;
		break;
	case TurningOfTheLight:
		std::cout << "TurningOfTheLight" << std::endl;
		break;
	default:
		break;
	}
}

void Disaster::MakeTableOfWeight()
{
	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::None, 25000));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::None)->second;

	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::TurningOfTheLight, 2));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::TurningOfTheLight)->second;

	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::Siren, 1));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::Siren)->second;

	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::Rockfall, 1));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::Rockfall)->second;
}

void Disaster::CheckCoordInField(sf::Vector2i& coord)
{
	if (coord.x >= WIDTH_MAP)
	{
		coord.x = WIDTH_MAP - 1;
	}
	if (coord.y >= HEIGHT_MAP)
	{
		coord.y = HEIGHT_MAP - 1;
	}

	if (coord.x < 0)
	{
		coord.x = 0;
	}
	if (coord.y < 0)
	{
		coord.y = 0;
	}
}

float Disaster::RandomAngleForShake()
{
	return -1 + 2 * ((float)rand()) / RAND_MAX;
}

void Disaster::SetParamsForShake(float shakingPower, float maxTime)
{
	m_animation.current = sf::seconds(0);
	m_animation.max = sf::milliseconds(maxTime);
	m_animation.shakingPower += shakingPower;
}

void Disaster::Shake(float dTime, sf::RenderWindow& window)
{
	if (m_animation.current >= m_animation.max)
	{
		m_view.reset(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));
		window.setView(m_view);
		return;
	}

	double angle = CAMERA_ANGLE_SHAKE * m_animation.shakingPower * RandomAngleForShake();

	//std::cout << m_animation.shakingPower << std::endl;

	sf::Vector2f offset;
	offset.x = CAMERA_ANGLE_OFFSET * m_animation.shakingPower * RandomAngleForShake();
	offset.y = CAMERA_ANGLE_OFFSET * m_animation.shakingPower * RandomAngleForShake();

	m_view.setRotation(angle);
	//m_view.setCenter(centerView + offset);
	window.setView(m_view);

	m_animation.current += sf::seconds(dTime);

	float ratio = m_animation.current.asSeconds() / m_animation.max.asSeconds();
	m_animation.shakingPower *= 1.0 - ratio * ratio;
}

void Disaster::FindSuitablesStonesForFall(sf::Vector2i tileCoordOfRightBottomRect, sf::Vector2i tileCoordOfLeftTopRect)
{
	for (int iterY = tileCoordOfLeftTopRect.y; iterY < tileCoordOfRightBottomRect.y; iterY++)
	{
		for (int iterX =  tileCoordOfLeftTopRect.x; iterX < tileCoordOfRightBottomRect.x; iterX++)
		{
			if (m_map->GetTypeOfTile(iterY * HEIGHT_MAP + iterX) == TypeTile::Stone &&
				m_map->GetCountOfStoneNeighbor({ iterY, iterX }) < 3)
			{
				countOfFallingStone++;
				stones.push_back(std::pair(sf::Vector2i{iterY, iterX}, false));
			}
		}
	}
}

void Disaster::CreateVectorOfStones(sf::Vector2i tileCoordOfRightBottomRect, sf::Vector2i tileCoordOfLeftTopRect)
{
	for (int iterX = tileCoordOfLeftTopRect.x; iterX < tileCoordOfRightBottomRect.x; iterX++)
	{
		for (int iterY = tileCoordOfRightBottomRect.y + 10; iterY >= tileCoordOfLeftTopRect.y; iterY--)
		{
			if (m_map->GetTypeOfTile(iterY * HEIGHT_MAP + iterX) == TypeTile::Wall) continue;

			sf::Vector2i coordStone = { iterY, iterX };

			if (iterY == tileCoordOfRightBottomRect.y + 10)
			{
				stones.push_back(std::pair(coordStone, true));
				continue;
			}

			sf::Vector2i coordStoneBelow = { coordStone.x + 1, coordStone.y };
			sf::Vector2i coordStoneLeft = { coordStone.x, coordStone.y - 1 };
			sf::Vector2i coordStoneRight = { coordStone.x, coordStone.y + 1 };

			if (std::find(stones.begin(), stones.end(), std::pair(coordStoneBelow, true)) != stones.end() ||
				std::find(stones.begin(), stones.end(), std::pair(coordStoneLeft, true)) != stones.end() ||
				std::find(stones.begin(), stones.end(), std::pair(coordStoneRight, true)) != stones.end() ||
				m_map->GetCountOfStoneNeighbor({ iterY, iterX }) >= 3)
			{
				stones.push_back(std::pair(coordStone, true));
				continue;
			}
		}
	}
}

void Disaster::EnumerationStones()
{
	stonesForNextIteration.clear();
	for (auto& stone : stones)
	{
		if (!stone.second)
		{
			sf::Vector2i coordStoneBelow = { stone.first.x + 1, stone.first.y };
			if (std::find(stones.begin(), stones.end(), std::pair(coordStoneBelow, true)) == stones.end() &&
				coordStoneBelow.x != WIDTH_MAP)
			{
				m_map->MoveStoneDown({ float(stone.first.x + 1), float(stone.first.y) });
				stonesForNextIteration.push_back(std::pair(coordStoneBelow, false));
			}
			else
			{
				countOfFallingStone--;
				stonesForNextIteration.push_back(std::pair(stone.first, true));
			}
		}
		else
		{
			stonesForNextIteration.push_back(stone);
		}
	}
	itItStonesNow = false;
}

void Disaster::EnumerationStonesForNextIteration()
{
	stones.clear();
	for (auto& stone : stonesForNextIteration)
	{
		if (!stone.second)
		{
			sf::Vector2i coordStoneBelow = { stone.first.x + 1, stone.first.y };
			if (std::find(stonesForNextIteration.begin(), stonesForNextIteration.end(), std::pair(coordStoneBelow, true)) ==
				stonesForNextIteration.end() && coordStoneBelow.x != WIDTH_MAP)
			{
				m_map->MoveStoneDown({ float(stone.first.x + 1), float(stone.first.y) });
				stones.push_back(std::pair(coordStoneBelow, false));
			}
			else
			{
				countOfFallingStone--;
				stones.push_back(std::pair(stone.first, true));
			}
		}
		else
		{
			stones.push_back(stone);
		}
	}
	itItStonesNow = true;
}

void Disaster::FallingStone(int time, float dTime, sf::RenderWindow& window)
{
	Shake(dTime, window);
	if (countOfFallingStone == 0)
	{
		itItStonesNow = true;
		stones.clear();
		stonesForNextIteration.clear();

		return;
	}
	else
	{
		bool isStonesEmpty = true;
		bool isStonesForNextIterationEmpty = true;

		if (stones.size() != 0)
		{
			for (auto& stone : stones)
			{
				if (!stone.second)
				{
					isStonesEmpty = false;
					break;
				}
			}
		}

		if (stonesForNextIteration.size() != 0)
		{
			for (auto& stone : stonesForNextIteration)
			{
				if (!stone.second)
				{
					isStonesForNextIterationEmpty = false;
					break;
				}
			}
		}

		if (isStonesEmpty && isStonesForNextIterationEmpty)
		{
			countOfFallingStone = 0;
			return;
		}
	}

	if (time % 2 != 0) return;

	if (itItStonesNow)
	{
		EnumerationStones();
	}
	else
	{
		EnumerationStonesForNextIteration();
	}
}

void Disaster::DoRockfall(sf::Vector2f playerCoord, bool isPlayerMovementToRight)
{
	SetParamsForShake(0.2, 1200);
	sf::Vector2i tileCoordOfRightBottomRect;
	sf::Vector2i tileCoordOfLeftTopRect;

	if (isPlayerMovementToRight)
	{
		tileCoordOfRightBottomRect = { int(std::floor(playerCoord.x / WIDTH_TILE)) + 15, int(std::floor(playerCoord.y / HEIGHT_TILE)) + 5 };
		tileCoordOfLeftTopRect = { tileCoordOfRightBottomRect.x - 13, tileCoordOfRightBottomRect.y - 15 };
	}
	else
	{
		tileCoordOfRightBottomRect = { int(std::floor(playerCoord.x / WIDTH_TILE)) - 2, int(std::floor(playerCoord.y / HEIGHT_TILE)) + 5 };
		tileCoordOfLeftTopRect = { tileCoordOfRightBottomRect.x - 13, tileCoordOfRightBottomRect.y - 15 };
	}

	CheckCoordInField(tileCoordOfRightBottomRect);
	CheckCoordInField(tileCoordOfLeftTopRect);

	FindSuitablesStonesForFall(tileCoordOfRightBottomRect, tileCoordOfLeftTopRect);
	
	CreateVectorOfStones(tileCoordOfRightBottomRect, tileCoordOfLeftTopRect);
}