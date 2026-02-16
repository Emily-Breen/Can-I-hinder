#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "MathUtils.h"

class InputHandler {

public:
	InputHandler();
	void update();
	bool isControllerConnected();
	sf::Vector2f getMovement();
	bool isAttackPressed();
	bool isActionPressed();
	bool menuUpPressed() const;
	bool menuDownPressed() const;
	bool menuSelectPressed() const;
	void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& menuView);
	bool controllerAttackJustPressed() const;
	bool menuMouseMoved() const;
	bool menuMouseClickReleased() const;
	sf::Vector2f menuMousePosition() const;
	void setDeadZone(float deadZone);

private:
	bool m_controllerConnected;
	sf::Vector2f m_movement{ 0.f,0.f };
	bool m_attackPressed{ false };
	bool m_actionPressed{ false };
	bool m_menuUpPressed{ false };
	bool m_menuDownPressed{ false };
	bool m_menuSelectPressed{ false };
	bool m_controllerAttackJustPressed{ false };
	bool m_prevControllerAttack{ false };
	bool m_prevUp{ false };
	bool m_prevDown{ false };
	bool m_prevSelect{ false };
	float m_deadZone{ 15.f };

	// Menu mouse input
	bool m_menuMouseMoved{ false };
	bool m_menuMouseClickReleased{ false };
	sf::Vector2f m_menuMousePos{ 0.f, 0.f };

	unsigned int m_controllerId{ 0 };
};
