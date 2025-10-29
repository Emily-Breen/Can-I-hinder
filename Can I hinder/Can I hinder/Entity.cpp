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

sf::Sprite Entity::getSprite()
{
	return m_sprite;
}



