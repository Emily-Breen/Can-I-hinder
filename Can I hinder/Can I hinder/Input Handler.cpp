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
    m_controllerAttackJustPressed = false;
     m_menuUpPressed = false;
     m_menuDownPressed = false;
     m_menuSelectPressed = false;
     m_controllerConnected = sf::Joystick::isConnected(m_controllerId);
     m_movement = { 0.f, 0.f };
     m_attackPressed = false;

     if (m_controllerConnected)
     {
         float x = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::Axis::X);
         float y = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::Axis::Y);

         
         const bool aNow = sf::Joystick::isButtonPressed(m_controllerId, 0);

       
         const bool upNow = (y < -50.f);
         const bool downNow = (y > 50.f);

         m_menuUpPressed = upNow && !m_prevUp;
         m_menuDownPressed = downNow && !m_prevDown;
         m_prevUp = upNow;
         m_prevDown = downNow;

         m_menuSelectPressed = aNow && !m_prevSelect;
         m_prevSelect = aNow;

       
         if (std::abs(x) < m_deadZone) x = 0.f;
         if (std::abs(y) < m_deadZone) y = 0.f;

         m_movement = { x / 100.f, y / 100.f };

         float len = MathUtils::vectorLength(m_movement);
         if (len > 1.f)
             m_movement /= len;


         m_attackPressed = aNow;
         m_controllerAttackJustPressed = aNow && !m_prevControllerAttack;
         m_prevControllerAttack = aNow;
     }
     else
     {
         m_prevControllerAttack = false;
         const bool upNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up);
         const bool downNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down);
         const bool selectNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Enter);

         m_menuUpPressed = upNow && !m_prevUp;
         m_menuDownPressed = downNow && !m_prevDown;
         m_menuSelectPressed = selectNow && !m_prevSelect;

         m_prevUp = upNow;
         m_prevDown = downNow;
         m_prevSelect = selectNow;

         // gameplay movement
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) m_movement.y -= 1.f;
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) m_movement.y += 1.f;
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) m_movement.x -= 1.f;
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) m_movement.x += 1.f;

		 float len = MathUtils::vectorLength(m_movement);
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

bool InputHandler::menuUpPressed() const
{
    return m_menuUpPressed;
}

bool InputHandler::menuDownPressed() const
{
   return m_menuDownPressed;
}

bool InputHandler::menuSelectPressed() const
{
   return m_menuSelectPressed;
}

void InputHandler::handleEvent(const sf::Event& event,
    const sf::RenderWindow& window,
    const sf::View& menuView)
{
   
    m_menuMouseMoved = false;
    m_menuMouseClickReleased = false;

    if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
    {
        const sf::Vector2i pixelPos{ moved->position.x, moved->position.y };
        m_menuMousePos = window.mapPixelToCoords(pixelPos, menuView);
        m_menuMouseMoved = true;
        return;
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (released->button == sf::Mouse::Button::Left)
        {
            const sf::Vector2i pixelPos{ released->position.x, released->position.y };
            m_menuMousePos = window.mapPixelToCoords(pixelPos, menuView);
            m_menuMouseClickReleased = true;
        }
    }
}

bool InputHandler::controllerAttackJustPressed() const
{
    return m_controllerAttackJustPressed;
}

bool InputHandler::menuMouseMoved() const
{
    return m_menuMouseMoved;
}

bool InputHandler::menuMouseClickReleased() const
{
    return m_menuMouseClickReleased;
}

sf::Vector2f InputHandler::menuMousePosition() const
{
    return m_menuMousePos;
}

void InputHandler::setDeadZone(float deadZone)
{
	m_deadZone = deadZone;
}
