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
	Flower(Map* map, sf::Texture& textureForFlowers, sf::Texture& textureOfFlowerAngry);
	void MakeRandomGeneration(int countOfFlowerFriendlyBlock, int countOfFlowerAngryBlock);
	void DrawFlowers(sf::RenderTexture& castTexture);

	bool IsCoordInFriendlyFlower(sf::Vector2f coord);
	bool IsCoordInAngryFlower(sf::Vector2f coord, float xCoordErosionShader);

private:
	int Random(int min, int max);
	std::pair<bool, bool> IsPlaceForFlower(int iterX, int iterY);

private:
	Map* m_map;
	std::vector<FlowerSprite> flowersSprite;

	sf::Texture m_textureForFlowers;
	sf::Texture m_textureOfFlowerAngry;
};