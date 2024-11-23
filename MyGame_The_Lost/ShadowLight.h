#pragma once
#include "includes.h"
#include "Tile.h"
#include "Map.h"

struct Edge
{
	sf::Vector2f start;
	sf::Vector2f end;
};

class ShadowLight
{
public:
	ShadowLight(Map* map, sf::Vector2f mouseCoords);

	void UpdateMouseCoords(sf::Vector2f& newCoord);
	void Update(sf::Vector2f& mouseCoords, const bool& isMouseMove);

	void DrawLines(sf::RenderWindow& window);

	void DrawTriangles(sf::RenderWindow& window);

private:
	float toDegrees(float radians);
	float toRadians(float degrees);

	void ConvertTileMapToPolyMap();
	void CalculateVisibilityPolygon(sf::Vector2f start, float radius);

private:
	std::vector<sf::ConvexShape*> triangles;

	sf::Vector2f m_mouseCoord;

	Map* m_map = nullptr;

	std::vector<Edge*> vecEdges;
	std::vector<std::tuple<float, float, float>> vecVisibilityPolygonPoints;
};