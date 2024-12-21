#include "Camera.h"

Camera::Camera()
{
	m_window.create(
		sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
		"The Lost", sf::Style::Default);

	m_view.reset(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));
	m_window.setView(m_view);

	renderTextureForLight.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	renderTextureForLight.setSmooth(true);

	castTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	castTexture.setSmooth(true);

	m_player = nullptr;

	viewPosition = m_view.getCenter() - m_view.getSize() / 2.0f;
}

void Camera::UpdatePostionCamera()
{
	sf::Vector2f deltaMove = { positionPlayerAfterMove.x - positionPlayerBeforeMove.x, positionPlayerAfterMove.y - positionPlayerBeforeMove.y };
	
	m_view.move(deltaMove.x, deltaMove.y);
	m_view.setCenter(m_player->GetPosition());
	m_window.setView(m_view);

	viewPosition = m_view.getCenter() - m_view.getSize() / 2.0f;
}

void Camera::SetPlayerCoordsBeforeMove(sf::Vector2f coord)
{
	positionPlayerBeforeMove = coord;
}

void Camera::SetPlayerCoordsAfterMove(sf::Vector2f coord)
{
	positionPlayerAfterMove = coord;
}

void Camera::PlayerHandler()
{
	PlayerMovement playerMovement;
	bool isPlayerDig = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		playerMovement.isLeft = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		playerMovement.isRight = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		playerMovement.isTop = true;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		isPlayerDig = true;
	}
	m_player->UpdateMovement(playerMovement);
	m_player->UpdateDigging(isPlayerDig);
}

void Camera::OnMousePressed(const sf::Event::MouseButtonEvent& event, sf::Vector2f& mousePosition)
{
	mousePosition = { float(event.x), float(event.y) };
}

void Camera::OnMouseMoved(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
	mousePosition = { float(event.x), float(event.y) };
}

void Camera::EventHandler(sf::Event& event, sf::Vector2f& mouseCoords, bool& isMouseMove, Disaster* disasters)
{
	PlayerHandler();

	switch (event.type)
	{
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::MouseButtonPressed:
			OnMousePressed(event.mouseButton, mouseCoords);
			break;
		case sf::Event::MouseMoved:
			OnMouseMoved(event.mouseMove, mouseCoords);
			m_player->UpdateMouseCoord(mouseCoords);
			isMouseMove = true;
			break;
		case sf::Event::MouseButtonReleased:
			isMouseMove = false;
			break;
		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				disasters->MakeRandomDisaster(m_player->GetPosition(), m_player->GetDirectionOfMovement());
				//light->ChangeWorkingLight();
			}
		default:
			break;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window.close();
	}
}

sf::Vector2f Camera::GetViewPosition()
{
	return viewPosition;
}

void Camera::Update(sf::Vector2f& mouseCoords, bool& isMouseMove, Disaster* disasters)
{
	castTexture.setView(m_view);
	castTexture.clear();

	renderTextureForLight.setView(m_view);
	renderTextureForLight.clear();

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		EventHandler(event, mouseCoords, isMouseMove, disasters);
	}
}

void Camera::DrawRenderTexture(sf::RenderWindow& window, const sf::Shader& shadowShader)
{
	window.draw(sf::Sprite(castTexture.getTexture()));
	//window.draw(sf::Sprite(renderTextureForLight.getTexture()), &shadowShader);
}

void Camera::SetPlayer(Player* player)
{
	m_player = player;
	m_view.setCenter(m_player->GetPosition());
}

sf::View Camera::GetView()
{
	return m_view;
}