#pragma once
#include "includes.h"
#include "Map.h"

const float SPEED_CAMERA_FOR_MOVING_PLAYER = 3.f;
const float SPEED_PLAYER = 5.0f;
const float SPEED_PLAYER_FOR_FALL = 10.0f;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 35;

const int COUNT_SPRITE_MOVING = 12;
const int COUNT_SPRITE_DIGGING = 6;

const float DELAY_MOVEMENT = 0.1;
const float DELAY_SLEEPING = 0.5;

enum SideForChechiking {
	Top,
	Right,
	Bottom,
	Left,
};

struct PlayerMovement 
{
	bool isTop = false;
	bool isRight = false;
	bool isBottom = false;
	bool isLeft = false;
};

struct PlayerDigging
{
	bool isDig = false;
	sf::Vector2f mouseCoord;
	int countOfHit = 0;
};

class Player
{
public:

	Player(Map* map);

	void Update(sf::RenderTexture& castTexture, const sf::View& view, float deltaTimeForMovement);

	void UpdateMovement(PlayerMovement movement);
	void UpdateMouseCoord(sf::Vector2f mouseCoord);
	void UpdateDigging(bool isPlayerDig);
	void DisplayMovement();

	sf::Vector2f GetPosition();

	~Player();

private:
	bool CanPlayerPass(const sf::Vector2f& positionPlayer, SideForChechiking side);

	float GetModuleVector(const sf::Vector2f& vect);

	void PlayerMoveToTopSide();
	void PlayerMoveToRightSide(float deltaTimeForMovement);
	void PlayerMoveToBottomSide();
	void PlayerMoveToLeftSide(float deltaTimeForMovement);

	void PlayerDig(sf::Vector2f viewPosition);

	void SetIntRectsForMoving();
	void SetIntRectForDigging();

	void ApplyToSpriteMovementRight();
	void AdvanceMovementRight();
	void UpdateFramesMovementRight(float deltaTime);

	void ApplyToSpriteMovementLeft();
	void AdvanceMovementLeft();
	void UpdateFramesMovementLeft(float deltaTime);

private:
	PlayerMovement m_movement;
	PlayerDigging m_digging;

	sf::Texture textureMovingRight;
	sf::Texture textureMovingLeft;
	sf::Texture textureDigging;
	sf::Sprite player;

	Map* m_map;

	std::vector<sf::IntRect> framesForMovementRight;
	std::vector<sf::IntRect> framesForMovementLeft;

	std::vector<sf::IntRect> framesForDigging;

	int numberFrameOfMovementRight = 0;
	int numberFrameOfMovementLeft = 0;

	int numberFrameOfDigging = 0;

	float timeForMovementRight = 0;
	float timeForMovementLeft = 0;

	float timeForDigging = 0;
};