#include "AIBehaviour.h"

AIBehaviour::AIBehaviour()
{
}

void AIBehaviour::setMode(Mode mode)
{
    m_mode = mode;
}

sf::Vector2f AIBehaviour::computeSteering(const sf::Vector2f& agentPos, const sf::Vector2f& agentVelocity, const sf::Vector2f& targetPos, const sf::Vector2f& targetVelocity, float dt)
{   //get desired velocity based on mode
	sf::Vector2f desiredVelocity{ 0.f, 0.f };
    switch (m_mode) {
	case Mode::Seek:
		desiredVelocity = seek(agentPos, targetPos);
		break;
	case Mode::Arrive:
		desiredVelocity = arrive(agentPos, targetPos);
		break;
	case Mode::Flee:
		desiredVelocity = flee(agentPos, targetPos);
		break;
	case Mode::Pursue:
		desiredVelocity = pursue(agentPos, agentVelocity, targetPos, targetVelocity);
		break;
	case Mode::Wander:
		desiredVelocity = wander(agentVelocity, dt);
		break;
    }
	//calculate steering force
	sf::Vector2f steering = desiredVelocity - agentVelocity;
	steering = MathUtils::truncate(steering, m_maxAcceleration);
	//integrate to get new velocity
	sf::Vector2f newVelocity = agentVelocity + steering * dt;
	newVelocity = MathUtils::truncate(newVelocity, m_maxSpeed);
	//return displacement
	return newVelocity * dt;
}

sf::Vector2f AIBehaviour::seek(const sf::Vector2f& agentPos, const sf::Vector2f targetPos) const
{
    return MathUtils::normalize(targetPos - agentPos) * m_maxSpeed;
}

sf::Vector2f AIBehaviour::arrive(const sf::Vector2f& agentPos, const sf::Vector2f targetPos) const
{ // calculate distance to target
	sf::Vector2f toTarget = targetPos - agentPos;
	float distance = MathUtils::vectorLength(toTarget);
	if (distance < m_arrivalRadius) {
		return sf::Vector2f(0.f, 0.f);
	}
	float targetSpeed = m_maxSpeed;
	if (distance < m_slowRadius) {
		targetSpeed = m_maxSpeed * (distance / m_slowRadius);
	}
	return MathUtils::normalize(toTarget) * targetSpeed;
}

sf::Vector2f AIBehaviour::flee(const sf::Vector2f& agentPos, const sf::Vector2f targetPos) const
{
	//run away not today disco lady!
	return  MathUtils::normalize(agentPos - targetPos) * m_maxSpeed;
}

sf::Vector2f AIBehaviour::pursue(const sf::Vector2f& agentPos, const sf::Vector2f& agentVelocity, const sf::Vector2f& targetPos, const sf::Vector2f& targetVelocity) const
{
	//calculate distance and speed
	sf::Vector2f toTarget = targetPos - agentPos;
	float distance = MathUtils::vectorLength(toTarget);
	float speed = MathUtils::vectorLength(agentVelocity);
	//calculate prediction time
	float prediction = m_maxPredicition;
	if (speed > 0.0001f) {
		prediction = std::min(m_maxPredicition, distance / speed);
	}
	sf::Vector2f futurePosition = targetPos + targetVelocity * prediction;
	return MathUtils::normalize(futurePosition - agentPos) * m_maxSpeed;
}

sf::Vector2f AIBehaviour::wander(const sf::Vector2f& agentVelocity, float dt)
{   //update wander angle with random jitter (-1 to 1)
	float random = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
	m_wanderAngle += random * m_wanderJitter;
	//calculate wander force
	sf::Vector2f forward = (MathUtils::vectorLength(agentVelocity) > 0.01f) ? MathUtils::normalize(agentVelocity) : sf::Vector2f(1.f, 0.f);
	sf::Vector2f circleCenter = forward * m_wanderCircleDistance;
	sf::Vector2f displacement = sf::Vector2f(std::cos(m_wanderAngle), std::sin(m_wanderAngle));
	displacement *= m_wanderCircleRadius;
	//final wander force
	sf::Vector2f wanderForce = circleCenter + displacement;
	return MathUtils::normalize(wanderForce) * m_maxSpeed;
}
