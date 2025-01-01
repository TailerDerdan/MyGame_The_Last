#pragma once
#include "includes.h"
#include "Player.h"
#include "ShadowLight.h"

const float CAMERA_ANGLE_SHAKE = 10.0f;
const float CAMERA_ANGLE_OFFSET = 20.0f;

const float MAX_SPEED_GHOST = 20.0f;
const float MIN_SPEED_GHOST = 10.0f;

enum TypeOfDisaster
{
	None,
	Rockfall,
	Siren,
	TurningOfTheLight,
	Ghost,
};

struct StoneForRockFall
{
	bool isThisStoneFrozen = false;
	sf::Vector2f coord;
};

struct CameraAnimation
{
	sf::Time current;
	sf::Time max;
	float shakingPower;
};


class Disaster
{
public:
	Disaster(Map* map, Player* player, sf::View view, ShadowLight* light, sf::Texture& ghostTexture);

	void MakeRandomDisaster(sf::Vector2f playerCoord, bool isPlayerMovementToRight);
	void FallingStone(float dTime, sf::RenderWindow& window);
	void Shake(float dTime, sf::RenderWindow& window);
	void DoTurningOnTheLight();
	void MoveGhost(sf::RenderTexture& castTexture);

private:
	void MakeTableOfWeight();
	void WriteDisaster(TypeOfDisaster disaster, sf::Vector2f playerCoord, bool isPlayerMovementToRight);

	void DoRockfall(sf::Vector2f playerCoord, bool isPlayerMovementToRight);
	void DoSiren();
	void DoTurningOffTheLight();
	void DoGhost(sf::Vector2f playerCoord);

	void FindSuitablesStonesForFall(sf::Vector2i tileCoordOfRightBottomRect, sf::Vector2i tileCoordOfLeftTopRect);
	void CheckStoneAroundFallingStone(sf::Vector2i coordOfStone);
	void CreateVectorOfStones(sf::Vector2i tileCoordOfRightBottomRect, sf::Vector2i tileCoordOfLeftTopRect);
	void EnumerationStones();
	void EnumerationStonesForNextIteration();

	float RandomAngleForShake();
	void SetParamsForShake(float shakingPower, float maxTime);

	void CheckCoordInField(sf::Vector2i& coord);

	float GetModuleVector(const sf::Vector2f& vect);

private:
	Map* m_map;
	Player* m_player;
	ShadowLight* m_light;
	sf::View m_view;

	sf::Vector2f centerView;

	int countOfFallingStone = 0;

	bool itItStonesNow = true;

	CameraAnimation m_animation;

	std::vector<std::pair<sf::Vector2i, bool>> stones;
	std::vector<std::pair<sf::Vector2i, bool>> stonesForNextIteration;

	sf::Music soundOfSiren;
	sf::Music soundOfRockfall;
	sf::Music soundOfTurningOn;
	sf::Music soundOfTurningOff;

	sf::Clock timerForRockfall;

	sf::Clock timerForLight;
	bool isLightWork = true;

	sf::Clock timerForDisaster;
	bool isFirstDisaster = true;
	bool isNextDisaster = false;

	std::map<TypeOfDisaster, int> tableOFWeightOfDisaster;
	int sumOfAllWeight = 0;

	sf::Sprite ghost;
	sf::Texture ghostTexture;
	sf::IntRect ghostIntRect;
	float speedForGhost = 10.0f;
	bool isGhostMove = false;
};