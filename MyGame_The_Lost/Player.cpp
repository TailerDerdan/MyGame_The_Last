#include "Player.h"

Player::Player(Map* map, EndGame* end, sf::Vector2f viewPosition, sf::RenderTexture& castTexture)
{
	textureMovingRight.loadFromFile("../assets/movingRight.png");
	textureMovingLeft.loadFromFile("../assets/movingLeft.png");

	player.setTexture(textureMovingRight);
	player.setTextureRect({ 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT });
	player.setPosition({ 175, 550 });

	framesForMovementLeft.resize(COUNT_SPRITE_MOVING);

	SetIntRectsForMoving();
	SetIntRectForDigging();

	MakeRectsOfStates(viewPosition);

	castTexture.draw(rectForHp);
	castTexture.draw(rectForOxygen);
	castTexture.draw(rectForWater);

	castTexture.draw(wrapperRectForHp);
	castTexture.draw(wrapperRectForOxygen);
	castTexture.draw(wrapperRectForWater);

	timerForUpdateStatePlayer.restart();

	m_map = map;
	m_end = end;
}

void Player::SetIntRectsForMoving()
{
	int index = COUNT_SPRITE_MOVING - 1;

	sf::IntRect intRect;
	intRect.top = 0;
	intRect.height = 35;

	intRect.left = 0;
	intRect.width = 19;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 42;
	intRect.width = 20;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 88;
	intRect.width = 21;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 134;
	intRect.width = 21;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 180;
	intRect.width = 20;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 228;
	intRect.width = 19;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 277;
	intRect.width = 18;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 320;
	intRect.width = 19;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 364;
	intRect.width = 21;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 409;
	intRect.width = 22;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 456;
	intRect.width = 21;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
	index--;

	intRect.left = 506;
	intRect.width = 18;
	framesForMovementRight.push_back(intRect);

	framesForMovementLeft[index] = intRect;
}

void Player::SetIntRectForDigging()
{
	sf::IntRect intRect;
	intRect.top = 0;
	intRect.height = 35;

	intRect.left = 0;
	intRect.width = 27;
	framesForDigging.push_back(intRect);

	intRect.left = 43;
	intRect.width = 28;
	framesForDigging.push_back(intRect);

	intRect.left = 83;
	intRect.width = 22;
	framesForDigging.push_back(intRect);

	intRect.left = 134;
	intRect.width = 34;
	framesForDigging.push_back(intRect);

	intRect.left = 177;
	intRect.width = 33;
	framesForDigging.push_back(intRect);

	intRect.left = 215;
	intRect.width = 32;
	framesForDigging.push_back(intRect);
}

sf::Vector2f Player::GetPosition()
{
	return player.getPosition();
}

void Player::UpdateMovement(PlayerMovement movement)
{
	m_movement.isTop = movement.isTop;
	m_movement.isRight = movement.isRight;
	m_movement.isLeft = movement.isLeft;
}

void Player::UpdateMouseCoord(sf::Vector2f mouseCoord)
{
	m_digging.mouseCoord = mouseCoord;
}

void Player::UpdateDigging(bool isPlayerDig)
{
	m_digging.isDig = isPlayerDig;
}

void Player::DisplayMovement()
{
	std::cout << m_movement.isTop << " " << m_movement.isRight << " " << m_movement.isLeft << std::endl;
}

bool Player::CanPlayerPass(const sf::Vector2f& positionPlayer, SideForChechiking side)
{
	bool isThisTileAWall_forFirstPoint = false;
	bool isThisTileAWall_forSecondPoint = false;

	switch (side)
	{
	case Top:
	{
		int xCoord = std::floor((positionPlayer.x + 2) / 25);
		int yCoord = std::floor((positionPlayer.y - 2) / 25);
		int numberOfTile = yCoord * HEIGHT_MAP + xCoord;

		if (m_map->GetTypeOfTile(numberOfTile) == TypeTile::Wall)
		{
			isThisTileAWall_forFirstPoint = true;
		}

		xCoord = std::floor((positionPlayer.x + PLAYER_WIDTH - 2) / 25);
		numberOfTile = yCoord * HEIGHT_MAP + xCoord;

		if (m_map->GetTypeOfTile(numberOfTile) == TypeTile::Wall)
		{
			isThisTileAWall_forSecondPoint = true;
		}

		break;
	}
	case Right:
	{
		int xCoord = std::floor((positionPlayer.x + PLAYER_WIDTH - 2) / 25);
		int yCoord = std::floor((positionPlayer.y + 2) / 25);
		int numberOfTile = yCoord * HEIGHT_MAP + xCoord;

		if (m_map->GetTypeOfTile(numberOfTile) == TypeTile::Wall)
		{
			isThisTileAWall_forFirstPoint = true;
		}

		yCoord = std::floor((positionPlayer.y + PLAYER_HEIGHT - 2) / 25);
		numberOfTile = yCoord * HEIGHT_MAP + xCoord;

		if (m_map->GetTypeOfTile(numberOfTile) == TypeTile::Wall)
		{
			isThisTileAWall_forSecondPoint = true;
		}

		break;
	}
	case Bottom:
	{
		int xCoord = std::floor((positionPlayer.x + 1) / 25);
		int yCoord = std::floor((positionPlayer.y + PLAYER_HEIGHT + 2) / 25);
		int numberOfTile = yCoord * HEIGHT_MAP + xCoord;

		if (m_map->GetTypeOfTile(numberOfTile) == TypeTile::Wall)
		{
			isThisTileAWall_forFirstPoint = true;
		}

		xCoord = std::floor((positionPlayer.x + PLAYER_WIDTH - 2) / 25);
		numberOfTile = yCoord * HEIGHT_MAP + xCoord;

		if (m_map->GetTypeOfTile(numberOfTile) == TypeTile::Wall)
		{
			isThisTileAWall_forSecondPoint = true;
		}

		break;
	}
	case Left:
	{
		int xCoord = std::floor((positionPlayer.x - 2) / 25);
		int yCoord = std::floor((positionPlayer.y + 2) / 25);
		int numberOfTile = yCoord * HEIGHT_MAP + xCoord;

		if (m_map->GetTypeOfTile(numberOfTile) == TypeTile::Wall)
		{
			isThisTileAWall_forFirstPoint = true;
		}

		yCoord = std::floor((positionPlayer.y + PLAYER_HEIGHT - 2) / 25);
		numberOfTile = yCoord * HEIGHT_MAP + xCoord;

		if (m_map->GetTypeOfTile(numberOfTile) == TypeTile::Wall)
		{
			isThisTileAWall_forSecondPoint = true;
		}

		break;
	}
	default:
		break;
	}
	if (isThisTileAWall_forFirstPoint && isThisTileAWall_forSecondPoint)
	{
		return true;
	}
	return false;
}

float Player::GetModuleVector(const sf::Vector2f& vect)
{
	return std::sqrt(vect.x * vect.x + vect.y * vect.y);
}

void Player::PlayerMoveToRightSide(float deltaTimeForMovement, StatePlayerInWater statePlayerForWater)
{
	isPlayerMovementToRight = true;

	numberFrameOfMovementLeft = 0;
	timeForMovementLeft = 0;

	sf::Vector2f positionPlayer = player.getPosition();
	sf::Vector2f endPoint = { WIDTH_TILE * HEIGHT_MAP + 1, positionPlayer.y };

	sf::Vector2f motion = { endPoint.x - positionPlayer.x, endPoint.y - positionPlayer.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;
	float movementOffset = SPEED_PLAYER * deltaTime;

	if (statePlayerForWater.playerInWaterBottomLeft || statePlayerForWater.playerInWaterBottomRight ||
		statePlayerForWater.playerInWaterTopLeft || statePlayerForWater.playerInWaterTopRight)
	{
		movementOffset = SPEED_PLAYER_FOR_WATER * deltaTime;
	}

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.x - positionPlayer.x) <= 1 || !CanPlayerPass(positionPlayer + newDirection, SideForChechiking::Right))
	{
		return;
	}

	player.setPosition(positionPlayer + newDirection);

	UpdateFramesMovementRight(deltaTimeForMovement);
	ApplyToSpriteMovementRight();
}

void Player::PlayerMoveToLeftSide(float deltaTimeForMovement, StatePlayerInWater statePlayerForWater)
{
	isPlayerMovementToRight = false;

	numberFrameOfMovementRight = 0;
	timeForMovementRight = 0;

	sf::Vector2f positionPlayer = player.getPosition();
	sf::Vector2f endPoint = { -1, positionPlayer.y };

	sf::Vector2f motion = { endPoint.x - positionPlayer.x, endPoint.y - positionPlayer.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;
	float movementOffset = SPEED_PLAYER * deltaTime;

	if (statePlayerForWater.playerInWaterBottomLeft || statePlayerForWater.playerInWaterBottomRight ||
		statePlayerForWater.playerInWaterTopLeft || statePlayerForWater.playerInWaterTopRight)
	{
		movementOffset = SPEED_PLAYER_FOR_WATER * deltaTime;
	}

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.x - positionPlayer.x) <= 1 || !CanPlayerPass(positionPlayer + newDirection, SideForChechiking::Left))
	{
		return;
	}
	player.setPosition(positionPlayer + newDirection);

	UpdateFramesMovementLeft(deltaTimeForMovement);
	ApplyToSpriteMovementLeft();
}

void Player::PlayerMoveToBottomSide(StatePlayerInWater statePlayerForWater)
{
	sf::Vector2f positionPlayer = player.getPosition();
	sf::Vector2f endPoint = { positionPlayer.x, HEIGHT_TILE * WIDTH_MAP + 1 };

	sf::Vector2f motion = { endPoint.x - positionPlayer.x, endPoint.y - positionPlayer.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;
	float movementOffset = SPEED_PLAYER_FOR_FALL * deltaTime;

	if (statePlayerForWater.playerInWaterBottomLeft || statePlayerForWater.playerInWaterBottomRight ||
		statePlayerForWater.playerInWaterTopLeft || statePlayerForWater.playerInWaterTopRight)
	{
		movementOffset = SPEED_PLAYER_FOR_WATER_FOR_FALL * deltaTime;
	}

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.y - positionPlayer.y) <= 1 || !CanPlayerPass(positionPlayer + newDirection, SideForChechiking::Bottom))
	{
		m_movement.isBottom = false;
		return;
	}

	m_movement.isBottom = true;
	player.setPosition(positionPlayer + newDirection);
}

void Player::PlayerMoveToTopSide(StatePlayerInWater statePlayerForWater)
{
	sf::Vector2f positionPlayer = player.getPosition();
	sf::Vector2f endPoint = { positionPlayer.x, positionPlayer.y - PLAYER_HEIGHT * 1.5f };

	sf::Vector2f motion = { endPoint.x - positionPlayer.x, endPoint.y - positionPlayer.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;
	float movementOffset = SPEED_PLAYER * deltaTime;

	if (statePlayerForWater.playerInWaterBottomLeft || statePlayerForWater.playerInWaterBottomRight || 
		statePlayerForWater.playerInWaterTopLeft || statePlayerForWater.playerInWaterTopRight)
	{
		movementOffset = SPEED_PLAYER_FOR_WATER * deltaTime;
	}

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.y - positionPlayer.y) <= 5 || !CanPlayerPass(positionPlayer + newDirection, SideForChechiking::Top))
	{
		return;
	}

	player.setPosition(positionPlayer + newDirection);
}

void Player::PlayerDig(sf::Vector2f viewPosition)
{
	if ((m_digging.mouseCoord.x > player.getPosition().x - viewPosition.x + WIDTH_TILE * 2 + 1) ||
		(m_digging.mouseCoord.x < player.getPosition().x - viewPosition.x - WIDTH_TILE))
	{
		return;
	}
	
	if ((m_digging.mouseCoord.y > player.getPosition().y - viewPosition.y + HEIGHT_TILE * 3 + 1) ||
		(m_digging.mouseCoord.y < player.getPosition().y - viewPosition.y - HEIGHT_TILE - 1))
	{
		return;
	}

	sf::Vector2f coordOfTile = { std::floor((m_digging.mouseCoord.y + viewPosition.y) / HEIGHT_TILE),
		std::floor((m_digging.mouseCoord.x + viewPosition.x) / WIDTH_TILE) };

	if (coordOfTile.x == 0 || coordOfTile.y == 0 || 
		coordOfTile.x == WIDTH_MAP - 1 || coordOfTile.x == WIDTH_MAP || 
		coordOfTile.y == HEIGHT_MAP - 1 || coordOfTile.y == HEIGHT_MAP) return;

	int numberOfTile = coordOfTile.x * HEIGHT_MAP + coordOfTile.y;

	m_map->DeleteStone(numberOfTile, coordOfTile);
}

void Player::Update(sf::RenderTexture& castTexture, const sf::View& view, float deltaTimeForMovement)
{
	statePlayerInWater.playerInWaterBottomRight = m_map->CoordInWater({ player.getPosition().x + player.getLocalBounds().width,
		player.getPosition().y + player.getLocalBounds().height });
	statePlayerInWater.playerInWaterBottomLeft = m_map->CoordInWater({ player.getPosition().x, player.getPosition().y + player.getLocalBounds().height });

	statePlayerInWater.playerInWaterTopLeft = m_map->CoordInWater(player.getPosition());
	statePlayerInWater.playerInWaterTopRight = m_map->CoordInWater({ player.getPosition().x + player.getLocalBounds().width, player.getPosition().y });

	if (!m_movement.isTop)
	{
		PlayerMoveToBottomSide(statePlayerInWater);
	}
	if (m_movement.isRight)
	{
		PlayerMoveToRightSide(deltaTimeForMovement, statePlayerInWater);
	}
	if (m_movement.isLeft)
	{
		PlayerMoveToLeftSide(deltaTimeForMovement, statePlayerInWater);
	}
	if (!m_movement.isBottom)
	{
		PlayerMoveToTopSide(statePlayerInWater);
	}
	if (m_digging.isDig)
	{
		PlayerDig(view.getCenter() - view.getSize() / 2.0f);
	}
	if (m_map->DidPlayerFindTeam(player.getPosition()))
	{
		m_end->ChangeStateDialogue();
	}
	UpdateRectsOfStates(view.getCenter() - view.getSize() / 2.0f, castTexture);
	castTexture.draw(player);
}

void Player::ApplyToSpriteMovementRight()
{
	player.setTexture(textureMovingRight);
	player.setTextureRect(framesForMovementRight[numberFrameOfMovementRight]);
}

void Player::AdvanceMovementRight()
{
	if (++numberFrameOfMovementRight >= COUNT_SPRITE_MOVING)
	{
		numberFrameOfMovementRight = 0;
	}
}

void Player::UpdateFramesMovementRight(float deltaTime)
{
	timeForMovementRight += deltaTime;
	while (timeForMovementRight >= DELAY_MOVEMENT)
	{
		timeForMovementRight -= DELAY_MOVEMENT;
		AdvanceMovementRight();
	}
}

void Player::ApplyToSpriteMovementLeft()
{
	player.setTexture(textureMovingLeft);
	player.setTextureRect(framesForMovementLeft[numberFrameOfMovementLeft]);
}

void Player::AdvanceMovementLeft()
{
	if (++numberFrameOfMovementLeft >= COUNT_SPRITE_MOVING)
	{
		numberFrameOfMovementLeft = 0;
	}
}

void Player::UpdateFramesMovementLeft(float deltaTime)
{
	timeForMovementLeft += deltaTime;
	while (timeForMovementLeft >= DELAY_MOVEMENT)
	{
		timeForMovementLeft -= DELAY_MOVEMENT;
		AdvanceMovementLeft();
	}
}

bool Player::GetDirectionOfMovement()
{
	return isPlayerMovementToRight;
}

void Player::MakeRectsOfStates(sf::Vector2f viewPosition)
{
	rectForHp.setFillColor(sf::Color(198, 7, 14));
	rectForHp.setSize({ hp, HEIGHT_RECT_STATE_PLAYER });
	rectForHp.setPosition({ viewPosition.x + 20, viewPosition.y + Y_INDENT_FOR_RECT });

	wrapperRectForHp.setFillColor(sf::Color(0, 0, 0, 0));
	wrapperRectForHp.setOutlineColor(sf::Color(195, 195, 195));
	wrapperRectForHp.setOutlineThickness(THICKNESS_FOR_RECT);
	wrapperRectForHp.setSize({ MAX_HP, HEIGHT_RECT_STATE_PLAYER });
	wrapperRectForHp.setPosition(rectForHp.getPosition());

	rectForWater.setFillColor(sf::Color(43, 30, 106));
	rectForWater.setSize({ waterLevel, HEIGHT_RECT_STATE_PLAYER });
	rectForWater.setPosition({ viewPosition.x + 20, viewPosition.y + Y_INDENT_FOR_RECT + HEIGHT_RECT_STATE_PLAYER + Y_INDENT_FOR_RECT });

	wrapperRectForWater.setFillColor(sf::Color(0, 0, 0, 0));
	wrapperRectForWater.setOutlineColor(sf::Color(195, 195, 195));
	wrapperRectForWater.setOutlineThickness(THICKNESS_FOR_RECT);
	wrapperRectForWater.setSize({ MAX_WATER_LEVEL, HEIGHT_RECT_STATE_PLAYER });
	wrapperRectForWater.setPosition(rectForWater.getPosition());

	rectForOxygen.setFillColor(sf::Color(44, 134, 179));
	rectForOxygen.setSize({ oxygenLevel, HEIGHT_RECT_STATE_PLAYER });
	rectForOxygen.setPosition({ viewPosition.x + 20, viewPosition.y + Y_INDENT_FOR_RECT + HEIGHT_RECT_STATE_PLAYER + Y_INDENT_FOR_RECT
		+ HEIGHT_RECT_STATE_PLAYER + Y_INDENT_FOR_RECT });

	wrapperRectForOxygen.setFillColor(sf::Color(0, 0, 0, 0));
	wrapperRectForOxygen.setOutlineColor(sf::Color(195, 195, 195));
	wrapperRectForOxygen.setOutlineThickness(THICKNESS_FOR_RECT);
	wrapperRectForOxygen.setSize({ MAX_OXYGEN_LEVEL, HEIGHT_RECT_STATE_PLAYER });
	wrapperRectForOxygen.setPosition(rectForOxygen.getPosition());
}

void Player::UpdateRectsOfStates(sf::Vector2f viewPosition, sf::RenderTexture& castTexture)
{
	rectForHp.setPosition({ viewPosition.x + 20, viewPosition.y + Y_INDENT_FOR_RECT });
	rectForWater.setPosition({ viewPosition.x + 20, viewPosition.y + Y_INDENT_FOR_RECT + HEIGHT_RECT_STATE_PLAYER + Y_INDENT_FOR_RECT });
	rectForOxygen.setPosition({ viewPosition.x + 20, viewPosition.y + Y_INDENT_FOR_RECT + HEIGHT_RECT_STATE_PLAYER + Y_INDENT_FOR_RECT
		+ HEIGHT_RECT_STATE_PLAYER + Y_INDENT_FOR_RECT });

	wrapperRectForHp.setPosition(rectForHp.getPosition());
	wrapperRectForWater.setPosition(rectForWater.getPosition());
	wrapperRectForOxygen.setPosition(rectForOxygen.getPosition());

	castTexture.draw(rectForHp);
	castTexture.draw(rectForOxygen);
	castTexture.draw(rectForWater);

	castTexture.draw(wrapperRectForHp);
	castTexture.draw(wrapperRectForOxygen);
	castTexture.draw(wrapperRectForWater);

	if (timerForUpdateStatePlayer.getElapsedTime().asSeconds() <= 0.8f) return;

	if (statePlayerInWater.playerInWaterTopLeft || statePlayerInWater.playerInWaterTopRight)
	{
		if (oxygenLevel > MIN_OXYGEN_LEVEL)
		{
			oxygenLevel -= 2.5f;
		}
	}
	else
	{
		if (oxygenLevel < MAX_OXYGEN_LEVEL)
		{
			oxygenLevel += 5.f;
		}
	}

	if (statePlayerInWater.playerInWaterTopLeft || statePlayerInWater.playerInWaterTopRight ||
		statePlayerInWater.playerInWaterBottomLeft || statePlayerInWater.playerInWaterBottomRight)
	{
		if (waterLevel < MAX_WATER_LEVEL)
		{
			waterLevel += 3.0f;
		}
	}
	else
	{
		if (waterLevel > MIN_WATER_LEVEL)
		{
			waterLevel -= 0.1f;
		}
	}

	if (oxygenLevel == MIN_OXYGEN_LEVEL)
	{
		if (hp > MIN_HP)
		{
			hp -= 2.5f;
		}
	}

	if (waterLevel == MIN_WATER_LEVEL)
	{
		if (hp > MIN_HP)
		{
			hp -= 0.3f;
		}
	}

	rectForHp.setSize({ hp, HEIGHT_RECT_STATE_PLAYER });
	rectForWater.setSize({ waterLevel, HEIGHT_RECT_STATE_PLAYER });
	rectForOxygen.setSize({ oxygenLevel, HEIGHT_RECT_STATE_PLAYER });

	timerForUpdateStatePlayer.restart();
}