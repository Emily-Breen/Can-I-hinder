#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class InputHandler {

public:
	InputHandler();
	void update();
	bool isControllerConnected();
	sf::Vector2f getMovement();
	bool isAttackPressed();
	bool isActionPressed();

	void setDeadZone(float deadZone);

private:
	bool m_controllerConnected;
	sf::Vector2f m_movement{ 0.f,0.f };
	bool m_attackPressed{ false };
	bool m_actionPressed{ false };

	float m_deadZone{ 15.f };

	unsigned int m_controllerId{ 0 };
};
