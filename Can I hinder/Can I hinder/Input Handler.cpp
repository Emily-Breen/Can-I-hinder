#include "Input Handler.h"

InputHandler::InputHandler() :m_controllerConnected(false), m_deadZone(15.f), m_controllerId(0)
{
	for (unsigned int i = 0; i < sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i))
		{
			m_controllerConnected = true;
			m_controllerId = i;
			break;
		}
	}
}

void InputHandler::update()
{
	m_controllerConnected = sf::Joystick::isConnected(m_controllerId);
	m_movement = { 0.f,0.f };
	m_actionPressed = false;
	m_attackPressed = false;

	if (m_controllerConnected) {
		//purley left stick movement 
		float xAxis = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::Axis::X);
		float yAxis = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::Axis::Y);

		//deadzone check so no drifting 
		if(std::abs(xAxis) < m_deadZone)
		{
			xAxis = 0.f;
		}
		if (std::abs(yAxis) < m_deadZone)
		{
			yAxis = 0.f;
		}
		//normalise to 1.f
		m_movement = sf::Vector2f(xAxis / 100.f, yAxis / 100.f);

		// just for future ref - 0 = A & 1 is B on xbox
		m_attackPressed = sf::Joystick::isButtonPressed(m_controllerId, 0); 
		//m_actionPressed = sf::Joystick::isButtonPressed(m_controllerId, 1); 
		
	}
	else {
		// keyboard movement if not controller is connected 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
		{
			m_movement.y -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
		{
			m_movement.y += 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
		{
			m_movement.x -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
		{
			m_movement.x += 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
		{
			m_attackPressed = true;
		}
	}
	//have to clamp to 1 for diagonal movement on controller/keyboard
	float len = std::sqrt(m_movement.x * m_movement.x + m_movement.y * m_movement.y);
	if (len > 1.f)
		m_movement /= len;
}

bool InputHandler::isControllerConnected()
{
	return m_controllerConnected;
}

sf::Vector2f InputHandler::getMovement()
{
	return m_movement;
}

bool InputHandler::isAttackPressed()
{
	return m_attackPressed;
}

bool InputHandler::isActionPressed()
{
	return m_actionPressed;
}

void InputHandler::setDeadZone(float deadZone)
{
	m_deadZone = deadZone;
}
