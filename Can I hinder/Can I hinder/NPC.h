#pragma once
#include "Entity.h"
#include "AnimationHandler.h"
#include "SFML/Graphics.hpp"
#include <string>
#include "AIBehaviour.h"
#include <cmath>
enum class EnemyType
{
	Skeleton,
	Goblin
};
class NPC : public Entity
{
public:

	NPC(std::shared_ptr<sf::Texture> tex);
		
	~NPC();
	void draw(sf::RenderWindow& window) override;
	void update(float dt) override;
	sf::FloatRect getBounds();
	static sf::Vector2f rectCenter(const sf::FloatRect& rect);
	sf::Vector2f getDirection() const;
	void movement(sf::Vector2f t_movement);;
	void moveWithCollision(const sf::Vector2f& delta, const std::vector<sf::FloatRect>& walls);
	sf::Vector2f setPosition(float x, float y);
	sf::Vector2f getPosition() const;
	void setAIMode(AIBehaviour::Mode mode);
	sf::Vector2f getVelocity(sf::Vector2f vel) const;
	void setVelocity(sf::Vector2f vel);
	sf::Vector2f computeAIMovement(const sf::Vector2f& targetPos, const sf::Vector2f& targetVel, float dt, const sf::Vector2f& separation,
	const std::vector<sf::FloatRect>& walls);
	sf::Vector2f computeSeparation(std::vector<NPC>& npcs, float separationRadius);
	void setAttacking(bool attacking);
	bool isAttacking() const;
	bool attackTimer(float dt);
	float getAttackRange() const;
	bool inAttackZone() const;
	void setInAttackZone(bool v);
	void facing(const sf::Vector2f& targetPos);
	void takeDamage(float amount, float hurtDuration = 0.25f);
	bool isDead() const;
	float heathRatio() const;
	//these may or may not stay
	bool hasDroppedLoot() const;
	void markDroppedLoot();
	EnemyType getType() const;
	void setType(EnemyType type);

private:
	void NPCInit();
	
	AnimationHandler m_animationHandler;
	AIBehaviour m_aiBehaviour;
	PlayerState m_State{ PlayerState::IDLE };
	Direction m_direction{ Direction::DOWN };
	sf::Vector2f m_healthBarOffset{ 0.f, -100.f };
	sf::Vector2f m_velocity{ 0.f, 0.f };
	std::shared_ptr<sf::Texture> m_texture;

	float m_speed{ 150.f };
	float m_detectionRadius{ 200.f };
	float m_idleTime{ 0.f };
	float m_attackRange{ 50.f };
	float m_attackCooldown{ 0.8f };
	float m_attackTimer{ 0.f };
	float m_hpMax{ 1.0f };
	float m_hp{ 1.0f };
	float m_hurtTimer{ 0.f };
	float m_invulnerabilityTimer{ 0.f };
	float m_deathTimer{ 0.f };
	float m_deathDuration{ 0.64f };
	float m_healthBarWidth{ 40.f };
	float m_healthBarHeight{ 6.f };
	bool m_isMoving{ false };
	bool m_attacking{ false };
	bool m_inAttackZone{ false };
	bool m_droppedLoot{ false }; //this may or may not stay 
	bool m_dead{ false };
	EnemyType m_type;




	
	


};

