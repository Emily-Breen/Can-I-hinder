#pragma once
#include <SFML/System.hpp>
#include <cstdlib>
#include <cmath>
#include "MathUtils.h"
#include "Entity.h"
class AIBehaviour
{
public:
	enum class Mode {
		Seek,
		Arrive,
		Pursue,
		Wander,
		Flee
	};
	AIBehaviour();

	//setting behaviour modes
	void setMode(Mode mode);
	Mode getMode() const { return m_mode; }
	//main output function for steering behaviours
	sf::Vector2f computeSteering(const sf::Vector2f& agentPos,
		const sf::Vector2f& agentVelocity,
		const sf::Vector2f& targetPos,
		const sf::Vector2f& targetVelocity,
		float dt, const sf::Vector2f& separation, const std::vector<sf::FloatRect>& walls, sf::FloatRect agentFeetBounds);

	//fine tuning parameters for behaviours
	void setMaxSpeed(float maxSpeed) { m_maxSpeed = maxSpeed; }
	void setMaxAcceleration(float maxAcceleration) { m_maxAcceleration = maxAcceleration; }
	void setArrivalRadius(float slowRadius, float arrivalRadius) { m_slowRadius = slowRadius; m_arrivalRadius = arrivalRadius; }
	void setPurusePredictionTime(float time) { m_maxPredicition = time; }
	void setWanderParameters(float circleDistance, float circleRadius, float jitter) {
		m_wanderCircleDistance = circleDistance;
		m_wanderCircleRadius = circleRadius;
		m_wanderJitter = jitter;
	}

private:
	void applyWallAvoidance(sf::Vector2f& desiredVelocity,const sf::FloatRect& agentFeetBounds,const std::vector<sf::FloatRect>& walls) const;

	Mode m_mode = Mode::Wander;
	float m_maxSpeed = 180.0f;
	float m_maxAcceleration = 1200.0f;
	float m_slowRadius = 220.0f;
	float m_arrivalRadius = 20.0f;
	float m_maxPredicition = 0.6f;

	float m_wanderCircleDistance = 80.0f;
	float m_wanderCircleRadius = 40.0f;
	float m_wanderJitter = 2.5f;
	float m_wanderAngle = 0.0f;
	float m_separationWeight{ 1.0f };
	float m_separationMax{ 1.0f };
	// Steering behaviour implementations
	sf::Vector2f seek(const sf::Vector2f& agentPos, const sf::Vector2f targetPos) const;
	sf::Vector2f arrive(const sf::Vector2f& agentPos, const sf::Vector2f targetPos) const;
	sf::Vector2f flee(const sf::Vector2f& agentPos, const sf::Vector2f targetPos) const;
	sf::Vector2f pursue(const sf::Vector2f& agentPos, const sf::Vector2f& agentVelocity,
		const sf::Vector2f& targetPos, const sf::Vector2f& targetVelocity) const;
	sf::Vector2f wander(const sf::Vector2f& agentVelocity, float dt);



};

