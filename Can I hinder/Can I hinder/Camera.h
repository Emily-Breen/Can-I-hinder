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

private:

	sf::View m_view;
	float smoothness{ 0.1f };


};

#endif // !CAMERA_H