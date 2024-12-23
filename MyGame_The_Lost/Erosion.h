#pragma once
#include "includes.h"

class Erosion
{
public:
	Erosion();
	sf::Texture noiseTexture;

private:
	float Perlin(sf::Vector2f coord);
	sf::Vector2f RandomGradient(sf::Vector2i coord);
	float DotGridGradient(sf::Vector2i coordDotAtIntersection, sf::Vector2f coordDotInTile);
	float Interpolate(sf::Vector2f coord, float weight);

private:
	const int octavesForNoise = 12;
	const int gridSize = 400;

	sf::Uint8* pixels = new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * 4];

	void MakeTexture();
};