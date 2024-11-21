#include "Player.h"

Player::Player(Map* map)
{
	texture.loadFromFile("../assets/moving.png");
	player.setTexture(texture);
	player.setTextureRect({ 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT });
	player.setPosition({ 200, 50 });

	m_map = map;
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

void Player::PlayerMoveToRightSide()
{
	sf::Vector2f positionPlayer = player.getPosition();
	sf::Vector2f endPoint = { WIDTH_TILE * HEIGHT_MAP + 1, positionPlayer.y };

	sf::Vector2f motion = { endPoint.x - positionPlayer.x, endPoint.y - positionPlayer.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;
	float movementOffset = SPEED_PLAYER * deltaTime;

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.x - positionPlayer.x) <= 1 || !CanPlayerPass(positionPlayer + newDirection, SideForChechiking::Right))
	{
		return;
	}

	player.setPosition(positionPlayer + newDirection);
}

void Player::PlayerMoveToLeftSide()
{
	sf::Vector2f positionPlayer = player.getPosition();
	sf::Vector2f endPoint = { -1, positionPlayer.y };

	sf::Vector2f motion = { endPoint.x - positionPlayer.x, endPoint.y - positionPlayer.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;
	float movementOffset = SPEED_PLAYER * deltaTime;

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.x - positionPlayer.x) <= 1 || !CanPlayerPass(positionPlayer + newDirection, SideForChechiking::Left))
	{
		return;
	}

	player.setPosition(positionPlayer + newDirection);
}

void Player::PlayerMoveToBottomSide()
{
	sf::Vector2f positionPlayer = player.getPosition();
	sf::Vector2f endPoint = { positionPlayer.x, HEIGHT_TILE * WIDTH_MAP + 1 };

	sf::Vector2f motion = { endPoint.x - positionPlayer.x, endPoint.y - positionPlayer.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;
	float movementOffset = SPEED_PLAYER * deltaTime;

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.y - positionPlayer.y) <= 1 || !CanPlayerPass(positionPlayer + newDirection, SideForChechiking::Bottom))
	{
		m_movement.isBottom = false;
		return;
	}

	m_movement.isBottom = true;
	player.setPosition(positionPlayer + newDirection);
}

void Player::PlayerMoveToTopSide()
{
	sf::Vector2f positionPlayer = player.getPosition();
	sf::Vector2f endPoint = { positionPlayer.x, -1 };

	sf::Vector2f motion = { endPoint.x - positionPlayer.x, endPoint.y - positionPlayer.y };
	float moduleMotion = GetModuleVector(motion);
	sf::Vector2f direction = { motion.x / moduleMotion, motion.y / moduleMotion };

	float deltaTime = 0.016;
	float movementOffset = SPEED_PLAYER * deltaTime + PLAYER_HEIGHT;

	sf::Vector2f newDirection = { direction.x * movementOffset, direction.y * movementOffset };

	if (std::abs(endPoint.y - positionPlayer.y) <= 1 || !CanPlayerPass(positionPlayer + newDirection, SideForChechiking::Top))
	{
		return;
	}

	player.setPosition(positionPlayer + newDirection);
}

void Player::Update()
{
	PlayerMoveToBottomSide();
	if (m_movement.isRight)
	{
		PlayerMoveToRightSide();
	}
	if (m_movement.isLeft)
	{
		PlayerMoveToLeftSide();
	}
	if (!m_movement.isBottom && m_movement.isTop)
	{
		PlayerMoveToTopSide();
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(player);
}