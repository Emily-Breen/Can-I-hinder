#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <iostream>
#include "AnimationHandler.h"
#include <algorithm>
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
	void setPosition(sf::Vector2f position);
	sf::FloatRect getBounds() const;
	sf::FloatRect getAttackBounds() const;
	void takeDamage(float duration = 0.35f);
	bool isHurt() const;
	void dead();
	bool isDead() const;
	
private:
	void playerInit();
	InputHandler m_inputHandler;
	AnimationHandler m_animationHandler;
	PlayerState m_State{ PlayerState::IDLE };
	Direction m_direction{ Direction::DOWN };
	sf::FloatRect m_hitbox;
	sf::Texture m_playerTexture;
	
	int attackPower{ 10 };
	float m_speed{ 300.f };
	float m_hurtTimer{ 0.f };
	float m_invulnerabilityTimer{ 0.f };
	float m_deathTimer{ 0.f };
	float m_deathDuration{ 0.5f };
	float m_attackTimer{ 0.f };
	float m_attackDuration{ 0.60f };
	bool  m_attackWasHeld{ false };
	bool m_isInvulnerable{ false };
	bool m_isDead{ false };

};