#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void Entity::init()
{

}

void Entity::updateAnimation(float dt)
{
	m_animationTimer += dt;
	if (m_animationTimer >= m_animationDelay) {
		m_animationTimer = 0.0f;
		m_currentFrame++;
		if (m_currentFrame > m_lastFrame) {
			m_currentFrame = m_firstFrame;
		}

		m_sprite.setTextureRect(sf::IntRect({ m_currentFrame * frameWidth, 0 }, { frameWidth, frameHeight }));
	}
}

void Entity::updateSpriteRect()
{
}
