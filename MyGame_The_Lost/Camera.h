#pragma once
#include "includes.h"
#include "Player.h"
#include "Disaster.h"

const float SPEED_CAMERA = 4.0f;

class Camera
{
public:
	sf::RenderWindow m_window;

	sf::RenderTexture castTexture;
	sf::RenderTexture renderTextureForLight;
	sf::RenderTexture renderTextureForPlayerState;

	Camera();

	void Update(sf::Vector2f& mouseCoords, bool& isMouseMove, Disaster* disasters);
	sf::Vector2f GetViewPosition();

	void DrawRenderTexture(sf::RenderWindow& window, const sf::Shader& shadowShader, const sf::Shader& corosionShader);

	void SetPlayerCoordsBeforeMove(sf::Vector2f coord);
	void SetPlayerCoordsAfterMove(sf::Vector2f coord);
	void UpdatePostionCamera();

	void SetPlayer(Player* player);
	sf::View GetView();

private:
	void OnMousePressed(const sf::Event::MouseButtonEvent& event, sf::Vector2f& mousePosition);
	void OnMouseMoved(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition);
	void EventHandler(sf::Event& event, sf::Vector2f& mouseCoords, bool& isMouseMove, Disaster* disasters);

	void PlayerHandler();

private:
	sf::View m_view;

	sf::Vector2f viewPosition;

	sf::Vector2f positionPlayerBeforeMove;
	sf::Vector2f positionPlayerAfterMove;

	Player* m_player;
};