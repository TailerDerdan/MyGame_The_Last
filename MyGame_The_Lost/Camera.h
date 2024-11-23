#pragma once
#include "includes.h"
#include "Player.h"

constexpr unsigned WINDOW_WIDTH = 1920;
constexpr unsigned WINDOW_HEIGHT = 1080;

const float SPEED_CAMERA = 12.5f;

class Camera
{
public:
	sf::RenderWindow m_window;

	Camera();

	void Update(sf::Vector2f& mouseCoords, bool& isMouseMove);
	sf::Vector2f GetViewPosition();

	void SetPlayer(Player* player);

private:
	void OnMousePressed(const sf::Event::MouseButtonEvent& event, sf::Vector2f& mousePosition);
	void OnMouseMoved(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition);
	void EventHandler(sf::Event& event, sf::Vector2f& mouseCoords, bool& isMouseMove);

	void KeyCameraHandler();
	void KeyPlayerHandler();

private:
	sf::View m_view;

	sf::Vector2f viewPosition;

	Player* m_player;
};