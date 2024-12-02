#pragma once
#include "includes.h"
#include "Player.h"

constexpr unsigned WINDOW_WIDTH = 1920;
constexpr unsigned WINDOW_HEIGHT = 1080;

const float SPEED_CAMERA = 1.0f;

class Camera
{
public:
	sf::RenderWindow m_window;

	sf::RenderTexture castTexture;
	sf::RenderTexture renderTextureForLight;

	Camera();

	void Update(sf::Vector2f& mouseCoords, bool& isMouseMove);
	sf::Vector2f GetViewPosition();

	void DrawRenderTexture(sf::RenderWindow& window, const sf::Shader& shadowShader);

	void SetPlayer(Player* player);
	sf::View GetView();

private:
	void OnMousePressed(const sf::Event::MouseButtonEvent& event, sf::Vector2f& mousePosition);
	void OnMouseMoved(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition);
	void EventHandler(sf::Event& event, sf::Vector2f& mouseCoords, bool& isMouseMove);

	void KeyCameraHandler();
	void PlayerHandler();

private:
	sf::View m_view;

	sf::Vector2f viewPosition;

	Player* m_player;
};