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

bool Entity::rectsIntersect(const sf::FloatRect& a, const sf::FloatRect& b)
{
	return !(
		a.position.x + a.size.x <= b.position.x ||
		a.position.x >= b.position.x + b.size.x ||
		a.position.y + a.size.y <= b.position.y ||
		a.position.y >= b.position.y + b.size.y
		);
}



