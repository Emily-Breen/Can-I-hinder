#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>
class Camera
{
public:
	Camera(float windowWidth, float windowHeight);
    void follow(const sf::Vector2f& targetPosition);
    void applyCam(sf::RenderWindow& m_window);
    void onResize(const sf::Vector2u& newWindowSize);
	void setSmoothness(float smoothness);
    void setPixelSnap(bool enabled);
	const sf::FloatRect& getViewport() const;
    const sf::Vector2f& getVirtualSize() const;


private:
	void updateViewport();

	sf::Vector2f m_virtualSize;
	sf::Vector2u m_windowSize;
	sf::View m_view;
	float m_smoothness{ 0.1f };
	bool m_pixelSnap{ true };


};

#endif // !CAMERA_H