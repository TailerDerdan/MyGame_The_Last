#include "Camera.h"

Camera::Camera()
{
	m_window.create(
		sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
		"The Lost", sf::Style::Default);

	m_view.reset(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));
	m_window.setView(m_view);

	m_player = nullptr;

	viewPosition = m_view.getCenter() - m_view.getSize() / 2.0f;
}

void Camera::KeyCameraHandler()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && viewPosition.y > 0)
	{
		m_view.move(0, -SPEED_CAMERA);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && viewPosition.x > 0)
	{
		m_view.move(-SPEED_CAMERA, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) && viewPosition.y < WINDOW_HEIGHT)
	{
		m_view.move(0, SPEED_CAMERA);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && viewPosition.x < WINDOW_WIDTH)
	{
		m_view.move(SPEED_CAMERA, 0);
	}

	m_window.setView(m_view);
	viewPosition = m_view.getCenter() - m_view.getSize() / 2.0f;
}

void Camera::KeyPlayerHandler()
{
	PlayerMovement playerMovement;
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
	m_player->UpdateMovement(playerMovement);
}

void Camera::OnMousePressed(const sf::Event::MouseButtonEvent& event, sf::Vector2f& mousePosition)
{
	mousePosition = { float(event.x), float(event.y) };
}

void Camera::OnMouseMoved(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
	mousePosition = { float(event.x), float(event.y) };
}

void Camera::EventHandler(sf::Event& event, sf::Vector2f& mouseCoords, bool& isMouseMove)
{
	KeyPlayerHandler();
	KeyCameraHandler();

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
			isMouseMove = true;
			break;
		case sf::Event::MouseButtonReleased:
			isMouseMove = false;
			break;
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

void Camera::Update(sf::Vector2f& mouseCoords, bool& isMouseMove)
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		EventHandler(event, mouseCoords, isMouseMove);
	}
}

void Camera::SetPlayer(Player* player)
{
	m_player = player;
}