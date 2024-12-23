#include "Erosion.h"

Erosion::Erosion()
{
	noiseTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	MakeTexture();
}

sf::Vector2f Erosion::RandomGradient(sf::Vector2i coord)
{
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = coord.x, b = coord.y;
	a *= 3284157443;

	b ^= a << s | a >> w - s;
	b *= 1911520717;

	a ^= b << s | b >> w - s;
	a *= 2048419325;
	float random = a * (3.14159265 / ~(~0u >> 1));

	sf::Vector2f v;
	v.x = sin(random);
	v.y = cos(random);

	return v;
}

float Erosion::Interpolate(sf::Vector2f coord, float weight)
{
	return (coord.y - coord.x) * (3.0 - weight * 2.0) * weight * weight + coord.x;
}

float Erosion::DotGridGradient(sf::Vector2i coordDotAtIntersection, sf::Vector2f coordDotInTile)
{
	sf::Vector2f gradient = RandomGradient(coordDotAtIntersection);

	sf::Vector2f delta = { coordDotInTile.x - (float)coordDotAtIntersection.x, coordDotInTile.y - (float)coordDotAtIntersection.y };

	return (gradient.x * delta.x + gradient.y * delta.y);
}

float Erosion::Perlin(sf::Vector2f coord)
{
	sf::Vector2i topLeftCoord = { (int)coord.x, (int)coord.y };
	sf::Vector2i bottomRightCoord = { topLeftCoord.x + 1, topLeftCoord.y + 1 };

	float sx = coord.x - (float)topLeftCoord.x;
	float sy = coord.y - (float)topLeftCoord.y;

	float n0 = DotGridGradient(topLeftCoord, coord);
	float n1 = DotGridGradient(sf::Vector2i(bottomRightCoord.x, topLeftCoord.y), coord);
	float interpolateX0 = Interpolate(sf::Vector2f(n0, n1), sx);

	n0 = DotGridGradient(sf::Vector2i(topLeftCoord.x, bottomRightCoord.y), coord);
	n1 = DotGridGradient(bottomRightCoord, coord);
	float interpolateX1 = Interpolate(sf::Vector2f(n0, n1), sx);

	float value = Interpolate(sf::Vector2f(interpolateX0, interpolateX1), sy);

	return value;
}

void Erosion::MakeTexture()
{
	for (int x = 0; x < WINDOW_WIDTH; x++)
	{
		for (int y = 0; y < WINDOW_HEIGHT; y++)
		{
			int index = (y * WINDOW_WIDTH + x) * 4;

			float val = 0;

			float freq = 1;
			float amp = 1;

			for (int i = 0; i < octavesForNoise; i++)
			{
				val += Perlin(sf::Vector2f(x * freq / gridSize, y * freq / gridSize)) * amp;

				freq *= 2;
				amp /= 2;
			}

			val *= 1.2;
			if (val > 1.0f)
			{
				val = 1.0f;
			}
			else if (val < -1.0f)
			{
				val = -1.0f;
			}

			int color = (int)(((val + 1.0f) * 0.5f) * 255);

			pixels[index + 0] = color;
			pixels[index + 1] = color;
			pixels[index + 2] = color;
			pixels[index + 3] = 255;
		}
	}

	noiseTexture.update(pixels);
}