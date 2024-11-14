#pragma once
#include "includes.h"

constexpr unsigned WINDOW_WIDTH = 1920;
constexpr unsigned WINDOW_HEIGHT = 1080;

class Camera
{
public:
	sf::RenderWindow m_window;	

	Camera();

	void Update();
	sf::Vector2f GetViewPosition();

private:
	void EventHandler(sf::Event& event);

private:
	sf::View m_view;

	bool m_dragging = false;
	sf::Vector2f viewPosition;
};