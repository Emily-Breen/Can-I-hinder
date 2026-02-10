#include "Camera.h"

Camera::Camera(float windowWidth, float windowHeight): m_virtualSize(windowWidth,windowHeight)
{
	m_view.setSize(m_virtualSize);
	m_view.setCenter(m_virtualSize * 0.5f);
	m_windowSize = { static_cast<unsigned>(windowWidth), static_cast<unsigned>(windowHeight) };
	updateViewport();
}

void Camera::follow(const sf::Vector2f& targetPosition)
{
	sf::Vector2f currentCenter = m_view.getCenter();
	sf::Vector2f newCenter = currentCenter + (targetPosition - currentCenter) * m_smoothness;

    if (m_pixelSnap)
    {
        
        constexpr float mapScale = 3.f;          
        const float step = 1.f / mapScale;      

        newCenter.x = std::round(newCenter.x / step) * step;
        newCenter.y = std::round(newCenter.y / step) * step;
    }
	m_view.setCenter(newCenter);
}

void Camera::applyCam(sf::RenderWindow& m_window)
{
	m_window.setView(m_view);
}

void Camera::onResize(const sf::Vector2u& newWindowSize)
{
	m_windowSize = newWindowSize;
	updateViewport();
}

void Camera::setSmoothness(float smoothness)
{ 
	m_smoothness = smoothness; 
}

void Camera::setPixelSnap(bool enabled)
{
    m_pixelSnap = enabled;
}

const sf::FloatRect& Camera::getViewport() const
{
    return m_view.getViewport();

}

const sf::Vector2f& Camera::getVirtualSize() const
{
    return m_virtualSize;
}

void Camera::updateViewport()
{
    if (m_windowSize.x == 0 || m_windowSize.y == 0)
        return;

    const float windowRatio = static_cast<float>(m_windowSize.x) / static_cast<float>(m_windowSize.y);
    const float virtualRatio = m_virtualSize.x / m_virtualSize.y;

    float sizeX = 1.f, sizeY = 1.f;
    float posX = 0.f, posY = 0.f;

    if (windowRatio > virtualRatio)
    {
        
        sizeX = virtualRatio / windowRatio;
        posX = (1.f - sizeX) * 0.5f;
    }
    else
    {
 
        sizeY = windowRatio / virtualRatio;
        posY = (1.f - sizeY) * 0.5f;
    }

    m_view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));
}
