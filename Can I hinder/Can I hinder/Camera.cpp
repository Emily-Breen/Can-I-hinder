#include "Camera.h"

Camera::Camera(float windowWidth, float windowHeight)
{
	m_view = sf::View(sf::FloatRect({ 0.f, 0.f }, { windowWidth, windowHeight }));
}

void Camera::follow(const sf::Vector2f& targetPosition)
{
	sf::Vector2f currentCenter = m_view.getCenter();
	sf::Vector2f newCenter = currentCenter + (targetPosition - currentCenter) * smoothness;
	m_view.setCenter(newCenter);
}

void Camera::applyCam(sf::RenderWindow& m_window)
{
	m_window.setView(m_view);
}
