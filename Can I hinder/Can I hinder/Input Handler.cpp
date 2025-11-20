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
     m_movement = { 0.f, 0.f };
     m_attackPressed = false;

     if (m_controllerConnected)
     {
         float x = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::Axis::X);
         float y = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::Axis::Y);

         if (std::abs(x) < m_deadZone) x = 0.f;
         if (std::abs(y) < m_deadZone) y = 0.f;

         // Convert from -100..100 to -1..1
         m_movement.x = x / 100.f;
         m_movement.y = y / 100.f;

         // Prevent drift
         float len = std::sqrt(m_movement.x * m_movement.x + m_movement.y * m_movement.y);
         if (len > 1.f)
             m_movement /= len;

         m_attackPressed = sf::Joystick::isButtonPressed(m_controllerId, 0);
     }
     else
     {
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) m_movement.y -= 1.f;
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) m_movement.y += 1.f;
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) m_movement.x -= 1.f;
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) m_movement.x += 1.f;

         float len = std::sqrt(m_movement.x * m_movement.x + m_movement.y * m_movement.y);
         if (len > 1.f)
             m_movement /= len;

         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
             m_attackPressed = true;
     }

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
