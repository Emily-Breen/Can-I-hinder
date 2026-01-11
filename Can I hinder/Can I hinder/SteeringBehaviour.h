#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

struct SteeringOutput;

class SteeringBehaviour
{
public:
	virtual ~SteeringBehaviour() = default;

	//agent is NPC target is player 
	virtual SteeringOutput calculate(const sf::Vector2f& agentPos, 
		const sf::Vector2f& agentVelocity, 
		const sf::Vector2f& targetPos, 
		const sf::Vector2f& targetVelocity, 
		float dt) = 0;
};
