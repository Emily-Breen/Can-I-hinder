#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <iostream>
#include "AnimationHandler.h"
#include "Input Handler.h"



class Player : public Entity
{
public:
	Player();
	~Player();

	void update(float dt)override;
	void draw(sf::RenderWindow& window) override;
	void movePlayer(sf::Vector2f direction);
	void movement(sf::Vector2f t_movement);
	sf::Vector2f getInputDirection();
	sf::Vector2f getMovement();
	sf::Vector2f getPosition();
	sf::FloatRect getBounds();
private:
	void playerInit();
	sf::Texture m_playerTexture;
	AnimationHandler m_animationHandler;
	PlayerState m_State{ PlayerState::IDLE };
	Direction m_direction{ Direction::DOWN };
	float m_speed{ 300.f };
	InputHandler m_inputHandler;
	int m_health{ 100 };
	int attackPower{ 10 };
	sf::FloatRect m_hitbox;


};