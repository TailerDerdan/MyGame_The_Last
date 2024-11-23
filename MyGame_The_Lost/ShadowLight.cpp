#include "ShadowLight.h"

ShadowLight::ShadowLight(Map* map, sf::Vector2f mouseCoords)
{
	m_map = map;
	Update(mouseCoords, false);
}

void ShadowLight::UpdateMouseCoords(sf::Vector2f& newCoord)
{
	m_mouseCoord = newCoord;
}

void ShadowLight::Update(sf::Vector2f& mouseCoords, const bool& isMouseMove)
{
	m_mouseCoord = mouseCoords;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (isMouseMove)
		{
			ConvertTileMapToPolyMap();
			CalculateVisibilityPolygon(m_mouseCoord, 10.f);
		}
	}
	else
	{
		vecVisibilityPolygonPoints.clear();
		triangles.clear();
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && vecVisibilityPolygonPoints.size() > 1)
	{
		triangles.clear();
		std::cout << vecVisibilityPolygonPoints.size() << std::endl;
		for (size_t iter = 0; iter < vecVisibilityPolygonPoints.size() - 1; iter++)
		{
			sf::ConvexShape* triangle = new sf::ConvexShape();

			triangle->setPointCount(3);

			triangle->setFillColor(sf::Color(255, 255, 255));

			sf::Vector2f startLineCoord = { std::get<1>(vecVisibilityPolygonPoints[iter]),
				std::get<2>(vecVisibilityPolygonPoints[iter]) };
			sf::Vector2f endLineCoord = { std::get<1>(vecVisibilityPolygonPoints[iter + 1]),
				std::get<2>(vecVisibilityPolygonPoints[iter + 1]) };

			triangle->setPoint(0, m_mouseCoord);
			triangle->setPoint(1, startLineCoord);
			triangle->setPoint(2, endLineCoord);

			triangles.push_back(triangle);

			startLineCoord = { std::get<1>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),
				std::get<2>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]) };
			endLineCoord = { std::get<1>(vecVisibilityPolygonPoints[0]),
				std::get<2>(vecVisibilityPolygonPoints[0]) };

			triangle->setPoint(0, m_mouseCoord);
			triangle->setPoint(1, startLineCoord);
			triangle->setPoint(2, endLineCoord);

			triangles.push_back(triangle);
		}
	}
}

void ShadowLight::DrawLines(sf::RenderWindow& window)
{
	for (auto edge : vecEdges)
	{
		sf::VertexArray lines(sf::LinesStrip, 2);

		lines[0].position = edge->start;
		lines[0].color = sf::Color::Red;

		lines[1].position = edge->end;
		lines[1].color = sf::Color::Red;

		window.draw(lines);
	}
}

void DisplayEdge(Edge edge)
{
	std::cout << edge.start.x << " " << edge.start.y << " " << edge.end.x << " " << edge.end.y << std::endl;
}

void ShadowLight::DrawTriangles(sf::RenderWindow& window)
{
	for (auto triangle : triangles)
	{
		window.draw(*triangle);
	}
}

void ShadowLight::ConvertTileMapToPolyMap()
{
	sf::Vector2f startCoord = { 0, 0 };
	vecEdges.clear();

	std::vector<Tile*> tiles = m_map->GetVectorTiles();

	for (int iter = 0; iter < tiles.size(); iter++)
	{
		if (!tiles[iter])
		{
			continue;
		}
		if (tiles[iter]->typeTile == TypeTile::Wall)
		{
			continue;
		}

		int numberOfTile = tiles[iter]->number;

		int yCoord = std::floor(numberOfTile / HEIGHT_MAP);
		int xCoord = numberOfTile - HEIGHT_MAP * yCoord;

		int indexNorth = iter - WIDTH_MAP / 2;
		int indexWest = iter - 1;

		if (!tiles[iter]->neighboursExist[WEST])
		{
			if (indexNorth < 0)
			{
				continue;
			}
			if (tiles[indexNorth]->edgeExist[WEST])
			{
				vecEdges[tiles[indexNorth]->edgeId[WEST]]->end.y += HEIGHT_TILE;
				tiles[iter]->edgeId[WEST] = tiles[indexNorth]->edgeId[WEST];
				tiles[iter]->edgeExist[WEST] = true;
			}
			else
			{
				Edge* edge = new Edge();
				edge->start.x = (startCoord.x + xCoord) * WIDTH_TILE;
				edge->start.y = (startCoord.y + yCoord) * HEIGHT_TILE;

				edge->end.x = edge->start.x;
				edge->end.y = edge->start.y + HEIGHT_TILE;

				int edgeId = vecEdges.size();
				vecEdges.push_back(edge);

				tiles[iter]->edgeId[WEST] = edgeId;
				tiles[iter]->edgeExist[WEST] = true;
			}
		}

		if (!tiles[iter]->neighboursExist[EAST])
		{
			if (indexNorth < 0)
			{
				continue;
			}
			if (tiles[indexNorth]->edgeExist[EAST])
			{
				vecEdges[tiles[indexNorth]->edgeId[EAST]]->end.y += HEIGHT_TILE;
				tiles[iter]->edgeId[EAST] = tiles[indexNorth]->edgeId[EAST];
				tiles[iter]->edgeExist[EAST] = true;
			}
			else
			{
				Edge* edge = new Edge();
				edge->start.x = (startCoord.x + xCoord + 1) * WIDTH_TILE;
				edge->start.y = (startCoord.y + yCoord) * HEIGHT_TILE;

				edge->end.x = edge->start.x;
				edge->end.y = edge->start.y + HEIGHT_TILE;

				int edgeId = vecEdges.size();
				vecEdges.push_back(edge);

				tiles[iter]->edgeId[EAST] = edgeId;
				tiles[iter]->edgeExist[EAST] = true;
			}
		}

		if (!tiles[iter]->neighboursExist[NORTH])
		{
			if (indexWest < 0)
			{
				continue;
			}
			if (tiles[indexWest]->edgeExist[NORTH])
			{
				vecEdges[tiles[indexWest]->edgeId[NORTH]]->end.x += WIDTH_TILE;
				tiles[iter]->edgeId[NORTH] = tiles[indexWest]->edgeId[NORTH];
				tiles[iter]->edgeExist[NORTH] = true;
			}
			else
			{
				Edge* edge = new Edge();
				edge->start.x = (startCoord.x + xCoord) * WIDTH_TILE;
				edge->start.y = (startCoord.y + yCoord) * HEIGHT_TILE;

				edge->end.x = edge->start.x + WIDTH_TILE;
				edge->end.y = edge->start.y;

				int edgeId = vecEdges.size();
				vecEdges.push_back(edge);

				tiles[iter]->edgeId[NORTH] = edgeId;
				tiles[iter]->edgeExist[NORTH] = true;
			}
		}

		if (!tiles[iter]->neighboursExist[SOUTH])
		{
			if (indexWest < 0)
			{
				continue;
			}
			if (tiles[indexWest]->edgeExist[SOUTH])
			{
				vecEdges[tiles[indexWest]->edgeId[SOUTH]]->end.x += WIDTH_TILE;
				tiles[iter]->edgeId[SOUTH] = tiles[indexWest]->edgeId[SOUTH];
				tiles[iter]->edgeExist[SOUTH] = true;
			}
			else
			{
				Edge* edge = new Edge();
				edge->start.x = (startCoord.x + xCoord) * WIDTH_TILE;
				edge->start.y = (startCoord.y + yCoord + 1) * HEIGHT_TILE;

				edge->end.x = edge->start.x + WIDTH_TILE;
				edge->end.y = edge->start.y;

				int edgeId = vecEdges.size();
				vecEdges.push_back(edge);

				tiles[iter]->edgeId[SOUTH] = edgeId;
				tiles[iter]->edgeExist[SOUTH] = true;
			}
		}
	}
}

float ShadowLight::toDegrees(float radians)
{
	return float(double(radians) * 180.0 / M_PI);
}

float ShadowLight::toRadians(float degrees)
{
	return float(degrees * M_PI / 180);
}

void ShadowLight::CalculateVisibilityPolygon(sf::Vector2f start, float radius)
{
	vecVisibilityPolygonPoints.clear();

	for (auto &edge : vecEdges)
	{
		for (int iter = 0; iter < 2; iter++)
		{
			sf::Vector2f rd;

			rd.x = (iter == 0 ? edge->start.x : edge->end.x) - start.x;
			rd.y = (iter == 0 ? edge->start.y : edge->end.y) - start.y;

			float base_ang = toDegrees(atan2f(rd.y, rd.x));

			float angular = 0;

			for (int iter2 = 0; iter2 < 3; iter2++)
			{
				if (iter2 == 0)
				{
					angular = base_ang - 0.0001f;
				}
				if (iter2 == 1)
				{
					angular = base_ang;
				}
				if (iter2 == 2)
				{
					angular = base_ang + 0.0001f;
				}

				rd.x = radius * std::cosf(toRadians(angular));
				rd.y = radius * std::sinf(toRadians(angular));

				float minT1 = INFINITY;
				sf::Vector2f minPoint = { 0, 0 };
				float minAngle = 0;

				bool isValidTriangle = false;

				for (auto &edge2 : vecEdges)
				{
					sf::Vector2f vectorEdge = { edge2->end.x - edge2->start.x, edge2->end.y - edge2->start.y };

					if (std::abs(vectorEdge.x - rd.x) > 0.0f && std::abs(vectorEdge.y - rd.y) > 0.0f)
					{
						float paramT2 = (rd.x * (edge2->start.y - start.y) + (rd.y * (start.x - edge2->start.x))) / (vectorEdge.x * rd.y - vectorEdge.y * rd.x);
						float paramT1 = (edge2->start.x + vectorEdge.x * paramT2 - start.x) / rd.x;

						if (paramT1 > 0 && paramT2 >= 0 && paramT2 <= 1.0f)
						{
							if (paramT1 < minT1)
							{
								minT1 = paramT1;
								minPoint.x = start.x + rd.x * paramT1;
								minPoint.y = start.y + rd.y * paramT1;
								minAngle = toDegrees(atan2f(minPoint.y - start.y, minPoint.x - start.x));

								isValidTriangle = true;
							}
						}
					}
				}

				if (isValidTriangle)
				{
					std::tuple<float, float, float> line = { minAngle, minPoint.x, minPoint.y };
					vecVisibilityPolygonPoints.push_back(line);
				}
			}
		}
	}

	std::sort(vecVisibilityPolygonPoints.begin(), vecVisibilityPolygonPoints.end(), 
		[&](const std::tuple<float, float, float> &point1, const std::tuple<float, float, float> &point2)
		{
			return std::get<0>(point1) < std::get<0>(point2);
		});
}