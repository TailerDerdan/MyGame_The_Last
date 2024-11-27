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
	sf::Shader shadowShader;

	ShadowLight(Map* map, sf::Vector2f mouseCoords, const int WINDOW_WIDTH, const int WINDOW_HEIGHT);

	void UpdateMouseCoords(sf::Vector2f& newCoord);
	void Update(const sf::Vector2f& mouseCoords, const bool& isMouseMove, sf::RenderWindow& window);

	void DrawLines(sf::RenderWindow& window);

	void DrawTriangles(sf::RenderWindow& window, const sf::View& view);

	void SetVecRays(sf::Vector2f mouseCoords);
	void SetViewForRenderTexture(const sf::View& view);

private:
	float toDegrees(float radians);
	float toRadians(float degrees);

	void ConvertTileMapToPolyMap();
	void CalculateVisibilityPolygon(sf::Vector2f start, float radius);

private:
	sf::RenderTexture castTexture;
	std::vector<sf::Vertex> vertices;

	sf::Vector2f m_mouseCoord;

	Map* m_map = nullptr;

	std::vector<Edge*> vecEdges;
	std::vector<std::tuple<float, float, float>> vecVisibilityPolygonPoints;

	std::vector<Edge*> vecRays;
};