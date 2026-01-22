#include "AIBehaviour.h"

AIBehaviour::AIBehaviour()
{
}

void AIBehaviour::setMode(Mode mode)
{
    m_mode = mode;
}

sf::Vector2f AIBehaviour::computeSteering(
    const sf::Vector2f& agentPos,
    const sf::Vector2f& agentVelocity,
    const sf::Vector2f& targetPos,
    const sf::Vector2f& targetVelocity,
    float dt,
    const sf::Vector2f& separation,
    const std::vector<sf::FloatRect>& walls,
    const sf::FloatRect agentFeetBounds
)
{
	//different steering behaviours
    sf::Vector2f desiredVelocity{ 0.f, 0.f };
    switch (m_mode)
    {
    case Mode::Seek:   desiredVelocity = seek(agentPos, targetPos); break;
    case Mode::Arrive: desiredVelocity = arrive(agentPos, targetPos); break;
    case Mode::Flee:   desiredVelocity = flee(agentPos, targetPos); break;
    case Mode::Pursue: desiredVelocity = pursue(agentPos, agentVelocity, targetPos, targetVelocity); break;
    case Mode::Wander: desiredVelocity = wander(agentVelocity, dt); break;
    }

    //Separttion so NPCS dont overlap
    const sf::Vector2f separationForce = MathUtils::truncate(separation, m_separationMax);
    desiredVelocity += separationForce * (m_separationWeight * m_maxSpeed);

	//applies wall avoidance to desired velocity
    applyWallAvoidance(desiredVelocity, agentFeetBounds, walls);

	//Steering force and new velocity
    const sf::Vector2f steeringForce =
        MathUtils::truncate(desiredVelocity - agentVelocity, m_maxAcceleration);

    sf::Vector2f newVelocity =
        MathUtils::truncate(agentVelocity + steeringForce * dt, m_maxSpeed);

    return newVelocity * dt; // displacement
}

//needs a bit of work NPCS are still getting stuck on walls and not traversing corridors well
void AIBehaviour::applyWallAvoidance(sf::Vector2f& desiredVelocity, const sf::FloatRect& agentFeetBounds, const std::vector<sf::FloatRect>& walls) const
{
	// No walls to avoid return
    if (walls.empty())
        return;
	// Parameters for wall avoidance
    const float lookAhead = 60.f;
    const float avoidStrength = 0.8f;
	// Direction the NPC is moving
    sf::Vector2f moveDir = MathUtils::normalize(desiredVelocity);
    if (MathUtils::vectorLength(moveDir) <= 0.0001f)
        return;
	// Create a probe box in front of the agent
    sf::FloatRect probeBox = agentFeetBounds;
    probeBox.position += moveDir * lookAhead;
	// Center point of the probe box
    const sf::Vector2f probeCenter{
        probeBox.position.x + probeBox.size.x * 0.5f,
        probeBox.position.y + probeBox.size.y * 0.5f
    };

    for (const auto& wall : walls)
    {
        if (!Entity::rectsIntersect(probeBox, wall))
            continue;

        const sf::Vector2f wallContactPoint =
            MathUtils::nearestPointOnRect(wall, probeCenter);

        sf::Vector2f wallNormal = MathUtils::normalize(probeCenter - wallContactPoint);

		// Fallback normal if calculation fails
        if (MathUtils::vectorLength(wallNormal) <= 0.0001f)
            wallNormal = sf::Vector2f(-moveDir.y, moveDir.x);

        // Slide along the wall
        float velocityIntoWall = MathUtils::dotProduct(desiredVelocity, wallNormal);
        if (velocityIntoWall < 0.f)
            desiredVelocity -= wallNormal * velocityIntoWall;

        // Small push outward for clearance
        desiredVelocity += wallNormal * (avoidStrength * m_maxSpeed);

        break; 
    }
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
