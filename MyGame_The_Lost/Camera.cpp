#include "Camera.h"

Camera::Camera()
{
	m_window.create(
		sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
		"The Lost", sf::Style::Default);

	m_view.reset(sf::FloatRect({ 0.f, 0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));
	m_window.setView(m_view);

	viewPosition = m_view.getCenter() - m_view.getSize() / 2.0f;
}

void Camera::EventHandler(sf::Event& event)
{
	bool isKeyD_Pressed = false;
	bool isKeyA_Pressed = false;
	bool isKeyW_Pressed = false;
	bool isKeyS_Pressed = false;

	switch (event.type)
	{
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::A)
			{
				isKeyA_Pressed = true;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				isKeyD_Pressed = true;
			}
			if (event.key.code == sf::Keyboard::W)
			{
				isKeyW_Pressed = true;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				isKeyS_Pressed = true;
			}
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_window.close();
			}
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::A)
			{
				isKeyA_Pressed = false;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				isKeyD_Pressed = false;
			}
			if (event.key.code == sf::Keyboard::W)
			{
				isKeyW_Pressed = false;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				isKeyS_Pressed = false;
			}
		default:
			break;
	}

	if (isKeyA_Pressed && viewPosition.x > 0) {
		m_view.move(-25.f, 0.f);
	}
	if (isKeyD_Pressed && viewPosition.x < WINDOW_WIDTH) {
		m_view.move(25.f, 0.f);
	}
	if (isKeyW_Pressed && viewPosition.y > 0) {
		m_view.move(0.f, -25.f);
	}
	if (isKeyS_Pressed && viewPosition.y < WINDOW_HEIGHT) {
		m_view.move(0.f, 25.f);
	}

	m_window.setView(m_view);
	viewPosition = m_view.getCenter() - m_view.getSize() / 2.0f;
}

sf::Vector2f Camera::GetViewPosition()
{
	return viewPosition;
}

void Camera::Update()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		EventHandler(event);
	}
}