#pragma once
#include "Entity.h"
#include "AnimationHandler.h"
#include "SFML/Graphics.hpp"
#include <string>
#include "AIBehaviour.h"
#include <cmath>
class NPC : public Entity
{
public:

	NPC(std::shared_ptr<sf::Texture> tex);
		
	~NPC();
	void draw(sf::RenderWindow& window) override;
	void update(float dt) override;
	sf::FloatRect getBounds();
	sf::Vector2f getDirection() const;
	void movement(sf::Vector2f t_movement);
	sf::Vector2f setPosition(float x, float y);
	sf::Vector2f getPosition() const;
	void setAIMode(AIBehaviour::Mode mode);
	sf::Vector2f getVelocity(sf::Vector2f vel) const;
	void setVelocity(sf::Vector2f vel);
	sf::Vector2f computeAIMovement(const sf::Vector2f& targetPos, const sf::Vector2f& targetVel, float dt);
	void setAttacking(bool attacking);
	bool isAttacking() const;
	bool attackTimer(float dt);
	float getAttackRange() const;
	bool inAttackZone() const;
	void setInAttackZone(bool v);
	void facing(const sf::Vector2f& targetPos);

private:
	void NPCInit();
	std::shared_ptr<sf::Texture> m_texture;
	AnimationHandler m_animationHandler;
	PlayerState m_State{ PlayerState::IDLE };
	Direction m_direction{ Direction::DOWN };

	float m_speed{ 150.f };
	float m_detectionRadius{ 200.f };
	float m_idleTime{ 0.f };
	bool m_isMoving{ false };
	bool m_attacking{ false };
	bool m_inAttackZone{ false };
	float m_attackRange{ 50.f }; //close but not too close so no overlap
	float m_attackCooldown{ 0.8f };
	float m_attackTimer{ 0.f };


	AIBehaviour m_aiBehaviour;
	sf::Vector2f m_velocity{ 0.f, 0.f };
};

