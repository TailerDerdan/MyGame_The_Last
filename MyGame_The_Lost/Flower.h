#pragma once
#include "includes.h"
#include "Map.h"

const int COUNT_FRIENDLY_FLOWER = 10;
const int COUNT_ANGRY_FLOWER = 15;

enum FlowerType
{
	Friendly,
	Angry,
};

struct FlowerSprite
{
	FlowerType flowerType;
	sf::IntRect texture;
	sf::Sprite sprite;
	sf::Vector2f coord;
};	

class Flower
{
public:
	Flower(Map* map, sf::Texture& textureForFlowers);
	void MakeRandomGeneration(int countOfFlowerFriendlyBlock, int countOfFlowerAngryBlock);
	void DrawFlowers(sf::RenderTexture& castTexture);

	void Update();

	bool IsCoordInFriendlyFlower(sf::Vector2f coord, bool isKeyEPress);
	bool IsCoordInAngryFlower(sf::Vector2f coord, float xCoordErosionShader, bool& isAngryFlower, bool isKeyEPress);

private:
	int Random(int min, int max);
	std::pair<bool, bool> IsPlaceForFlower(int iterX, int iterY);

	bool IsPointInFlower(sf::Vector2f point, sf::Sprite flower);

private:
	Map* m_map;
	std::vector<FlowerSprite> flowersSprite;

	sf::Texture m_textureForFlowers;
	sf::Texture m_textureOfFlowerAngry;
};