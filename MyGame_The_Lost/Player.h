#pragma once
#include "includes.h"
#include "Map.h"
#include "EndGame.h"
#include "Flower.h"

const float SPEED_PLAYER = 7.5f;
const float SPEED_PLAYER_FOR_JUMP = 25.0f;
const float SPEED_PLAYER_FOR_FALL = 10.0f;

const float SPEED_PLAYER_FOR_WATER = 3.0f;
const float SPEED_PLAYER_FOR_JUMP_FOR_WATER = 5.0f;
const float SPEED_PLAYER_FOR_WATER_FOR_FALL = 5.0f;

const int COUNT_SPRITE_MOVING = 12;
const int COUNT_SPRITE_DIGGING = 6;

const float DELAY_MOVEMENT = 0.1;
const float DELAY_SLEEPING = 0.5;

const float MAX_HP = 100.f;
const float MIN_HP = 0.f;

const float MAX_OXYGEN_LEVEL = 100.f;
const float MIN_OXYGEN_LEVEL = 0.f;

const float MAX_WATER_LEVEL = 100.f;
const float MIN_WATER_LEVEL = 0.f;

const float MAX_FEAR_LEVEL = 100.f;
const float MIN_FEAR_LEVEL = 0.f;

const float HEIGHT_RECT_STATE_PLAYER = 100.f;
const float KOEF_WIDTH_RECT_STATE_PLAYER = 5.f;
const float THICKNESS_FOR_RECT = 5.f;
const float Y_INDENT_FOR_RECT = 20.f;

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

struct StatePlayerInWater
{
	bool playerInWaterBottomRight = false;
	bool playerInWaterBottomLeft = false;
	bool playerInWaterTopLeft = false;
	bool playerInWaterTopRight = false;
};

class Player
{
public:

	Player(Map* map, EndGame* end, sf::Vector2f viewPosition, sf::RenderTexture& castTexture);

	void Update(sf::RenderTexture& castTexture, sf::RenderTexture& renderTextureForPlayerState, const sf::View& view, 
		float deltaTimeForMovement, sf::RenderWindow& window, Flower* flower);

	void UpdateMovement(PlayerMovement& movement);
	void UpdateMouseCoord(sf::Vector2f mouseCoord);
	void UpdateDigging(bool isPlayerDig);
	void DisplayMovement();

	void MakeInitialState();

	float GetElapsedTimeAfterStartLevel();

	bool GetDirectionOfMovement();
	bool GetIsPlayerFall();

	sf::Vector2f GetPosition();

	bool GetIsNextLevel();
	void SetIsNextLevel(bool state);

	void ChangeFearLevel(float delta);
	
	sf::Vector2f GetFirstCoordForCorosion();
	void ChangeFirstCoordForCorosion();

	void SetBadState(bool state);

	~Player();

private:
	bool CanPlayerPass(const sf::Vector2f& positionPlayer, SideForChechiking side);

	float GetModuleVector(const sf::Vector2f& vect);

	void PlayerMoveToTopSide(StatePlayerInWater statePlayerForWater);
	void PlayerMoveToRightSide(float deltaTimeForMovement, StatePlayerInWater statePlayerForWater);
	void PlayerMoveToBottomSide(StatePlayerInWater statePlayerForWater);
	void PlayerMoveToLeftSide(float deltaTimeForMovement, StatePlayerInWater statePlayerForWater);

	void PlayerDig(sf::Vector2f viewPosition);

	void SetIntRectsForMoving();
	void SetIntRectForDigging();

	void ApplyToSpriteMovementRight();
	void AdvanceMovementRight();
	void UpdateFramesMovementRight(float deltaTime);

	void ApplyToSpriteMovementLeft();
	void AdvanceMovementLeft();
	void UpdateFramesMovementLeft(float deltaTime);

	void UpdateRectsOfStates(sf::Vector2f viewPosition, sf::RenderTexture& castTexture);
	void MakeRectsOfStates(sf::Vector2f viewPosition);

	void PrintTimeLevel(sf::RenderTexture& texturePlayerState, sf::Vector2f viewPosition);

private:
	PlayerMovement m_movement;
	PlayerDigging m_digging;

	sf::Texture textureMovingRight;
	sf::Texture textureMovingLeft;
	sf::Texture textureDigging;
	sf::Sprite player;

	Map* m_map;

	EndGame* m_end;

	sf::Vector2f endCoordForJump;
	bool isJump = false;

	bool isPlayerMovementToRight = true;

	StatePlayerInWater statePlayerInWater;

	std::vector<sf::IntRect> framesForMovementRight;
	std::vector<sf::IntRect> framesForMovementLeft;

	std::vector<sf::IntRect> framesForDigging;

	sf::RectangleShape rectForHp;
	sf::RectangleShape wrapperRectForHp;

	sf::RectangleShape rectForOxygen;
	sf::RectangleShape wrapperRectForOxygen;

	sf::RectangleShape rectForWater;
	sf::RectangleShape wrapperRectForWater;

	sf::RectangleShape rectForFear;
	sf::RectangleShape wrapperRectForFear;

	float hp = MAX_HP;
	float oxygenLevel = MAX_OXYGEN_LEVEL;
	float waterLevel = MAX_WATER_LEVEL;
	float fearLevel = MIN_FEAR_LEVEL;

	sf::Clock timerForUpdateStatePlayer;

	int numberFrameOfMovementRight = 0;
	int numberFrameOfMovementLeft = 0;

	int numberFrameOfDigging = 0;

	float timeForMovementRight = 0;
	float timeForMovementLeft = 0;

	float timeForDigging = 0;

	bool isNextLevel = false;
	sf::Clock timerForPauseBetweenLevel;

	bool isBadState = false;
	sf::Vector2f firstCoordForCorrosion = { 0.001, 0.001 };
	sf::Clock timerForErosion;

	sf::Clock timerForLevel;
	sf::Font fontForTimer;
};