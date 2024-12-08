#pragma once
#include "includes.h"
#include "Map.h"
#include "ShadowLight.h"

const float CAMERA_ANGLE_SHAKE = 10.0f;
const float CAMERA_ANGLE_OFFSET = 20.0f;

enum TypeOfDisaster
{
	None,
	Rockfall,
	Siren,
	TurningOfTheLight,
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
	Disaster(Map* map, sf::View view, ShadowLight* light);

	void MakeRandomDisaster(sf::Vector2f playerCoord, bool isPlayerMovementToRight);
	void FallingStone(int time, float dTime, sf::RenderWindow& window);
	void Shake(float dTime, sf::RenderWindow& window);
	void DoTurningOnTheLight();

private:
	void MakeTableOfWeight();
	void WriteDisaster(TypeOfDisaster disaster, sf::Vector2f playerCoord, bool isPlayerMovementToRight);

	void DoRockfall(sf::Vector2f playerCoord, bool isPlayerMovementToRight);
	void DoSiren();
	void DoTurningOffTheLight();

	void FindSuitablesStonesForFall(sf::Vector2i tileCoordOfRightBottomRect, sf::Vector2i tileCoordOfLeftTopRect);
	void CheckStoneAroundFallingStone(sf::Vector2i coordOfStone);
	void CreateVectorOfStones(sf::Vector2i tileCoordOfRightBottomRect, sf::Vector2i tileCoordOfLeftTopRect);
	void EnumerationStones();
	void EnumerationStonesForNextIteration();

	float RandomAngleForShake();
	void SetParamsForShake(float shakingPower, float maxTime);

	void CheckCoordInField(sf::Vector2i& coord);

private:
	Map* m_map;
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

	sf::Clock timerForLight;
	bool isLightWork = true;

	sf::Clock timerForDisaster;
	bool isFirstDisaster = true;
	bool isNextDisaster = false;

	std::map<TypeOfDisaster, int> tableOFWeightOfDisaster;
	int sumOfAllWeight = 0;
};