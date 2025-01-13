#include "Disaster.h"

Disaster::Disaster(Map* map, Player* player, ShadowLight* light, sf::Texture& newGhostTexture)
{
	MakeTableOfWeight();
	m_map = map;
	m_player = player;
	m_light = light;

	isLightWork = true;
	isGhostMove = false;

	soundOfSiren.openFromFile("../assets/disastersSound/siren.wav");
	soundOfRockfall.openFromFile("../assets/disastersSound/rockfall.wav");
	soundOfTurningOn.openFromFile("../assets/disastersSound/turningOn.wav");
	soundOfTurningOff.openFromFile("../assets/disastersSound/turningOff.wav");

	ghostTexture = newGhostTexture;
	ghostIntRect = { 0, 0, WIDTH_GHOST, HEIGHT_GHOST };
	ghost.setTexture(ghostTexture);
	ghost.setTextureRect(ghostIntRect);
	ghost.setColor(sf::Color(255, 255, 255, 0));

	timerForDisaster.restart();
}

int Disaster::Random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return dist(rng);

	/*int randomInt = rand() % 100;

	return randomInt;*/
}

void Disaster::MakeRandomDisaster(sf::Vector2f playerCoord, bool isPlayerMovementToRight, float levelTime)
{
	if (timerForDisaster.getElapsedTime().asSeconds() < 5)
	{
		if (isFirstDisaster)
		{
			return;
		}
	}

	if (timerForDisaster.getElapsedTime().asSeconds() < 12)
	{
		if (isNextDisaster || !isFirstDisaster)
		{
			return;
		}
	}

	if (levelTime >= MAX_TIME_SECONDS_FOR_LEVEL)
	{
		isTimeForGhostMove = true;
	}

	if (m_map->GetCurrentLevel() == 3)
	{
		isTimeForGhostMove = true;
		isGhostMoveFirst = false;
	}

	if (isTimeForGhostMove && isGhostMoveFirst)
	{
		DoGhost(playerCoord);
		isGhostMoveFirst = false;
	}

	int randNum = Random(0, sumOfAllWeight);
	int currentWeight = 0;

	for (auto& disaster : tableOFWeightOfDisaster)
	{
		currentWeight += disaster.second;
		if (currentWeight > randNum)
		{
			WriteDisaster(disaster.first, playerCoord, isPlayerMovementToRight);
			break;
		}
	}
}

void Disaster::WriteDisaster(TypeOfDisaster disaster, sf::Vector2f playerCoord, bool isPlayerMovementToRight)
{
	switch (disaster)
	{
	case None:
		break;
	case Rockfall:
		DoRockfall(playerCoord, isPlayerMovementToRight);
		timerForDisaster.restart();
		isFirstDisaster = false;
		isNextDisaster = true;
		m_player->ChangeFearLevel(20.f);
		break;
	case Siren:
		DoSiren();
		timerForDisaster.restart();
		isFirstDisaster = false;
		isNextDisaster = true;
		m_player->ChangeFearLevel(35.f);
		break;
	case TurningOfTheLight:
		DoTurningOffTheLight();
		timerForDisaster.restart();
		isFirstDisaster = false;
		isNextDisaster = true;
		m_player->ChangeFearLevel(15.f);
		break;
	case Ghost:
		if (!isTimeForGhostMove) break;

		DoGhost(playerCoord);
		timerForDisaster.restart();
		isFirstDisaster = false;
		isNextDisaster = true;
		m_player->ChangeFearLevel(20.f);
		break;
	default:
		break;
	}
}

void Disaster::MakeTableOfWeight()
{
	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::None, 25000));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::None)->second;

	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::TurningOfTheLight, 3));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::TurningOfTheLight)->second;

	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::Siren, 1));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::Siren)->second;

	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::Rockfall, 1));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::Rockfall)->second;

	tableOFWeightOfDisaster.insert(std::pair(TypeOfDisaster::Ghost, 2));
	sumOfAllWeight += tableOFWeightOfDisaster.find(TypeOfDisaster::Ghost)->second;
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

void Disaster::Shake(float dTime, sf::RenderWindow& window, sf::View view)
{
	if (m_animation.current >= m_animation.max)
	{
		view.reset(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));
		window.setView(view);
		return;
	}

	isShake = true;

	double angle = CAMERA_ANGLE_SHAKE * m_animation.shakingPower * RandomAngleForShake();

	sf::Vector2f offset;
	offset.x = CAMERA_ANGLE_OFFSET * m_animation.shakingPower * RandomAngleForShake();
	offset.y = CAMERA_ANGLE_OFFSET * m_animation.shakingPower * RandomAngleForShake();

	view.setRotation(angle);
	view.setCenter({ WINDOW_WIDTH / 2 + offset.x, WINDOW_HEIGHT / 2 + offset.y });
	window.setView(view);

	m_animation.current += sf::seconds(dTime);

	float ratio = m_animation.current.asSeconds() / m_animation.max.asSeconds();
	m_animation.shakingPower *= 1.0 - ratio * ratio;
}

void Disaster::CheckStoneAroundFallingStone(sf::Vector2i coordOfStone)
{
	if (coordOfStone.x * HEIGHT_MAP + coordOfStone.y + 1 < WIDTH_MAP * HEIGHT_MAP &&
		std::find(stones.begin(), stones.end(), std::pair(sf::Vector2i{coordOfStone.x, coordOfStone.y + 1}, false)) == stones.end() &&
		m_map->GetTypeOfTile(coordOfStone.x * HEIGHT_MAP + coordOfStone.y + 1) != TypeTile::Wall)
	{
		if (m_map->GetTypeOfTile(coordOfStone.x * HEIGHT_MAP + coordOfStone.y + 1) == TypeTile::Ice)
		{
			int varForExcavatedCoal = 0;
			m_map->DeleteStone(coordOfStone.x * HEIGHT_MAP + coordOfStone.y + 1, 
				{ float(coordOfStone.x), float(coordOfStone.y + 1) }, varForExcavatedCoal);
		}
		else
		{
			countOfFallingStone++;
			stones.push_back(std::pair(sf::Vector2i{coordOfStone.x, coordOfStone.y + 1}, false));
		}
	}

	if (coordOfStone.x * HEIGHT_MAP + coordOfStone.y - 1 > 0 &&
		std::find(stones.begin(), stones.end(), std::pair(sf::Vector2i{coordOfStone.x, coordOfStone.y - 1}, false)) == stones.end() &&
		m_map->GetTypeOfTile(coordOfStone.x * HEIGHT_MAP + coordOfStone.y - 1) != TypeTile::Wall)
	{
		if (m_map->GetTypeOfTile(coordOfStone.x * HEIGHT_MAP + coordOfStone.y - 1) == TypeTile::Ice)
		{
			int varForExcavatedCoal = 0;
			m_map->DeleteStone(coordOfStone.x * HEIGHT_MAP + coordOfStone.y - 1, 
				{ float(coordOfStone.x), float(coordOfStone.y - 1) }, varForExcavatedCoal);
		}
		else
		{
			countOfFallingStone++;
			stones.push_back(std::pair(sf::Vector2i{coordOfStone.x, coordOfStone.y - 1}, false));
		}
	}

	if ((coordOfStone.x + 1) * HEIGHT_MAP + coordOfStone.y < WIDTH_MAP * HEIGHT_MAP &&
		std::find(stones.begin(), stones.end(), std::pair(sf::Vector2i{coordOfStone.x + 1, coordOfStone.y}, false)) == stones.end() &&
		m_map->GetTypeOfTile((coordOfStone.x + 1) * HEIGHT_MAP + coordOfStone.y) != TypeTile::Wall)
	{
		if (m_map->GetTypeOfTile((coordOfStone.x + 1) * HEIGHT_MAP + coordOfStone.y) == TypeTile::Ice)
		{
			int varForExcavatedCoal = 0;
			m_map->DeleteStone((coordOfStone.x + 1) * HEIGHT_MAP + coordOfStone.y, 
				{ float(coordOfStone.x + 1), float(coordOfStone.y) }, varForExcavatedCoal);
		}
		else
		{
			countOfFallingStone++;
			stones.push_back(std::pair(sf::Vector2i{coordOfStone.x + 1, coordOfStone.y}, false));
		}
	}
}

void Disaster::FindSuitablesStonesForFall(sf::Vector2i tileCoordOfRightBottomRect, sf::Vector2i tileCoordOfLeftTopRect)
{
	for (int iterY = tileCoordOfLeftTopRect.y; iterY < tileCoordOfRightBottomRect.y; iterY++)
	{
		for (int iterX = tileCoordOfLeftTopRect.x; iterX < tileCoordOfRightBottomRect.x; iterX++)
		{
			if ((m_map->GetTypeOfTile(iterY * HEIGHT_MAP + iterX) != TypeTile::Wall && m_map->GetTypeOfTile(iterY * HEIGHT_MAP + iterX) != TypeTile::Ice) &&
				std::find(stones.begin(), stones.end(), std::pair(sf::Vector2i{iterY, iterX}, false)) == stones.end() &&
				m_map->GetCountOfStoneNeighbor({ iterY, iterX }) < 3)
			{
				countOfFallingStone++;
				stones.push_back(std::pair(sf::Vector2i{iterY, iterX}, false));
				
				if (iterX != tileCoordOfLeftTopRect.x && iterX != tileCoordOfRightBottomRect.x - 1)
				{
					CheckStoneAroundFallingStone(sf::Vector2i(iterY, iterX));
				}
			}
		}
	}
}

void Disaster::CreateVectorOfStones(sf::Vector2i tileCoordOfRightBottomRect, sf::Vector2i tileCoordOfLeftTopRect)
{
	for (int iterX = tileCoordOfLeftTopRect.x; iterX < tileCoordOfRightBottomRect.x; iterX++)
	{
		for (int iterY = tileCoordOfRightBottomRect.y + 20; iterY >= tileCoordOfLeftTopRect.y; iterY--)
		{
			if (m_map->GetTypeOfTile(iterY * HEIGHT_MAP + iterX) == TypeTile::Wall || 
				m_map->GetTypeOfTile(iterY * HEIGHT_MAP + iterX) == TypeTile::Ice) continue;

			sf::Vector2i coordStone = { iterY, iterX };

			if (iterY == tileCoordOfRightBottomRect.y + 20)
			{
				stones.push_back(std::pair(coordStone, true));
				continue;
			}

			sf::Vector2i coordStoneBelow = { coordStone.x + 1, coordStone.y };
			sf::Vector2i coordStoneLeft = { coordStone.x, coordStone.y - 1 };
			sf::Vector2i coordStoneRight = { coordStone.x, coordStone.y + 1 };

			if (std::find(stones.begin(), stones.end(), std::pair(coordStone, false)) != stones.end())
			{
				continue;
			}

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

void Disaster::EnumerationStones(sf::Vector2f playerCoord)
{
	stonesForNextIteration.clear();
	bool isPlayerDamaged = false;
	int indexStone = 0;
	for (auto& stone : stones)
	{
		if (!stone.second)
		{
			sf::Vector2i coordStoneBelow = { stone.first.x + 1, stone.first.y };

			if (coordStoneBelow.x == WIDTH_MAP || coordStoneBelow.x == WIDTH_MAP - 1)
			{
				stones.push_back(std::pair(stone.first, true));
				continue;
			}

			if (std::find(stones.begin(), stones.end(), std::pair(coordStoneBelow, true)) == stones.end())
			{
				m_map->MoveStoneDown({ float(stone.first.x + 1), float(stone.first.y) }, { playerCoord.x + PLAYER_WIDTH / 2, playerCoord.y }, isPlayerDamaged);
				if (isPlayerDamaged)
				{
					std::vector<std::pair<sf::Vector2i, bool>>::iterator iterForDeleteStone = stones.begin() + indexStone;
					stones.erase(iterForDeleteStone);
					m_player->ChangeHpLevel(-DAMAGE_FOR_PLAYER);
					isPlayerDamaged = false;
				}
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
		indexStone++;
	}
	itItStonesNow = false;
}

void Disaster::EnumerationStonesForNextIteration(sf::Vector2f playerCoord)
{
	stones.clear();
	bool isPlayerDamaged = false;
	int indexStone = 0;
	for (auto& stone : stonesForNextIteration)
	{
		if (!stone.second)
		{
			sf::Vector2i coordStoneBelow = { stone.first.x + 1, stone.first.y };

			if (coordStoneBelow.x == WIDTH_MAP || coordStoneBelow.x == WIDTH_MAP - 1)
			{
				stones.push_back(std::pair(stone.first, true));
				continue;
			}

			if (std::find(stonesForNextIteration.begin(), stonesForNextIteration.end(), std::pair(coordStoneBelow, true)) ==
				stonesForNextIteration.end())
			{
				m_map->MoveStoneDown({ float(stone.first.x + 1), float(stone.first.y) }, {playerCoord.x + PLAYER_WIDTH / 2, playerCoord.y}, isPlayerDamaged);
				if (isPlayerDamaged)
				{
					std::vector<std::pair<sf::Vector2i, bool>>::iterator iterForDeleteStone = stonesForNextIteration.begin() + indexStone;
					stonesForNextIteration.erase(iterForDeleteStone);
					m_player->ChangeHpLevel(-DAMAGE_FOR_PLAYER);
					isPlayerDamaged = false;
				}
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
		indexStone++;
	}
	itItStonesNow = true;
}

void Disaster::FallingStone(float dTime, sf::RenderWindow& window, sf::Vector2f playerCoord)
{
	if (countOfFallingStone == 0)
	{
		itItStonesNow = true;
		stones.clear();
		stonesForNextIteration.clear();

		timerForRockfall.restart();

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

	if (timerForRockfall.getElapsedTime().asSeconds() > 0.3f)
	{
		if (itItStonesNow)
		{
			EnumerationStones(playerCoord);
		}
		else
		{
			EnumerationStonesForNextIteration(playerCoord);
		}
		timerForRockfall.restart();
	}
}

void Disaster::DoRockfall(sf::Vector2f playerCoord, bool isPlayerMovementToRight)
{
	timerForRockfall.restart();
	soundOfRockfall.play();
	SetParamsForShake(2, 2400);
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

void Disaster::DoSiren()
{
	SetParamsForShake(0.5, 350000);
	soundOfSiren.play();
}

void Disaster::DoTurningOffTheLight()
{
	timerForLight.restart();
	m_light->ChangeWorkingLight();
	soundOfTurningOff.play();
	isLightWork = false;
}

void Disaster::DoTurningOnTheLight()
{
	if (isLightWork) return;

	if (timerForLight.getElapsedTime().asSeconds() >= 6)
	{
		soundOfTurningOn.play();
		m_light->ChangeWorkingLight();
		isLightWork = true;
	}
}

float Disaster::GetModuleVector(const sf::Vector2f& vect)
{
	return std::sqrt(vect.x * vect.x + vect.y * vect.y);
}

void Disaster::DoGhost(sf::Vector2f playerCoord)
{
	//m_player->SetBadState(true);
	isGhostMove = true;
	ghost.setColor(sf::Color(255, 255, 255, 255));
	ghost.setPosition(playerCoord.x + 200, playerCoord.y - 150);
}

void Disaster::CheckBounds(sf::Vector2i& coord, int& widthRect, int& heightRect)
{
	if (coord.x < 0)
	{
		coord.x = 0;
	}

	if (coord.x > WIDTH_MAP)
	{
		coord.x = WIDTH_MAP;
	}

	if (coord.y < 0)
	{
		coord.y = 0;
	}

	if (coord.y > HEIGHT_MAP)
	{
		coord.y = HEIGHT_MAP;
	}

	if (coord.x + widthRect > HEIGHT_MAP)
	{
		widthRect = coord.x + widthRect - HEIGHT_MAP - 1;
	}

	if (coord.y + heightRect > WIDTH_MAP)
	{
		heightRect = coord.y + heightRect - WIDTH_MAP - 1;
	}
}

void Disaster::MoveGhost(sf::RenderTexture& castTexture)
{
	if (!isGhostMove) return;

	sf::Vector2f positionGhost = ghost.getPosition();
	sf::Vector2f endPoint = { -250, positionGhost.y };

	sf::Vector2f motion = { endPoint.x - positionGhost.x, endPoint.y - positionGhost.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;

	float movementOffset = 0.0f;

	if (positionGhost.x > 20.f)
	{
		movementOffset = 18 * (200 / positionGhost.x) * deltaTime;
	}
	else
	{
		movementOffset = 18 * deltaTime;
	}	 

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.x - positionGhost.x) <= 1.0f)
	{
		isGhostMove = false;
		return;
	}

	sf::Vector2i coordGhost = { int(std::floor(ghost.getPosition().y / WIDTH_TILE)), int(std::floor(ghost.getPosition().x / HEIGHT_TILE)) };

	int widthRect = int(std::floor(WIDTH_GHOST / WIDTH_TILE));
	int heightRect = int(std::floor(HEIGHT_GHOST / HEIGHT_TILE));

	CheckBounds(coordGhost, widthRect, heightRect);
	m_map->SpreadIceAroundRect(coordGhost, widthRect, heightRect);

	ghost.setPosition(positionGhost + newDirection);
	castTexture.draw(ghost);
}

bool Disaster::GetStateGhost()
{
	return isGhostMove;
}