#pragma once
#include "Entity.h"
#include "AnimationHandler.h"
#include "SFML/Graphics.hpp"
#include <string>
class NPC : public Entity
{
public:

	NPC(std::shared_ptr<sf::Texture> tex);
		
	~NPC();
	void draw(sf::RenderWindow& window) override;
	void update(float dt) override;

	void moveNPC(sf::Vector2f direction);
	sf::Vector2f setPosition(float x, float y);
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
};

