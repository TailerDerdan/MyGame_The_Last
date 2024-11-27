#pragma once
#include "includes.h"
#include "Map.h"

const int MAX_HP = 100;
const int MIN_HP = 0;

const float SPEED_CAMERA_FOR_MOVING_PLAYER = 3.f;
const float SPEED_PLAYER = 5.0f;
const float SPEED_PLAYER_FOR_FALL = 10.0f;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 35;

enum SideForChechiking {
	Top,
	Right,
	Bottom,
	Left,
};

struct PlayerMovement {
	bool isTop = false;
	bool isRight = false;
	bool isBottom = false;
	bool isLeft = false;
};

class Player
{
public:

	Player(Map* map);

	void Draw(sf::RenderWindow& window);
	void Update();

	void UpdateMovement(PlayerMovement movement);
	void DisplayMovement();

	sf::Vector2f GetPosition();

	~Player();

private:
	bool CanPlayerPass(const sf::Vector2f& positionPlayer, SideForChechiking side);

	float GetModuleVector(const sf::Vector2f& vect);

	void PlayerMoveToTopSide();
	void PlayerMoveToRightSide();
	void PlayerMoveToBottomSide();
	void PlayerMoveToLeftSide();

private:
	PlayerMovement m_movement;

	sf::Texture texture;
	sf::Sprite player;

	int hp = 0;

	Map* m_map;
};