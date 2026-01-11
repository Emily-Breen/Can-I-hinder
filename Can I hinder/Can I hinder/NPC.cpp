#include "NPC.h"
#include <iostream>

NPC::NPC(std::shared_ptr<sf::Texture> tex) : m_animationHandler(*tex), 
    m_texture(std::move(tex))
{
    NPCInit();
}

NPC::~NPC()
{
}

void NPC::draw(sf::RenderWindow& window)
{
    Entity::draw(window);
}

void NPC::update(float dt)
{
    
    m_idleTime+= dt;

    if (m_idleTime > 2.5f) 
    {
        m_idleTime = 0.f;
        m_isMoving = !m_isMoving;

        if (m_isMoving)
        {
            int m_npcdirection = rand() % 4;
            switch (m_npcdirection)
            {
            case 0: m_direction = Direction::DOWN; break;
            case 1: m_direction = Direction::LEFT; break;
            case 2: m_direction = Direction::RIGHT; break;
            case 3: m_direction = Direction::UP; break;
            }
            m_State = PlayerState::WALK;
        }
        else
        {
            m_State = PlayerState::IDLE;
        }
    }
    // Animation updates
    m_animationHandler.changeState(m_State);
    m_animationHandler.changeDirection(m_direction);
    m_animationHandler.update(dt);
    m_animationHandler.applyToSprite(m_sprite);

}

sf::FloatRect NPC::getBounds()
{
    sf::FloatRect global = m_sprite.getGlobalBounds();

    //hitbox for the NPC feet area for collision AABB
    float hitboxWidth = global.size.x * 0.40f;
    float hitboxHeight = global.size.y * 0.25f;

    float hitboxLeft = global.position.x + (global.size.x - hitboxWidth) / 2.f;
    float hitboxTop = global.position.y + (global.size.y - hitboxHeight);

    return sf::FloatRect(
        { hitboxLeft, hitboxTop },
        { hitboxWidth, hitboxHeight }
    );
}

sf::Vector2f NPC::getDirection() const
{
    if (!m_isMoving) return { 0.f, 0.f };

    switch (m_direction)
    {
    case Direction::DOWN:  return { 0.f, 1.f };
    case Direction::LEFT:  return { -1.f, 0.f };
    case Direction::RIGHT: return { 1.f, 0.f };
    case Direction::UP:    return { 0.f, -1.f };
    }
    return { 0.f, 0.f };
}

void NPC::movement(sf::Vector2f t_movement)
{
	m_sprite.move(t_movement);
}

sf::Vector2f NPC::setPosition(float x, float y)
{
	sf::Vector2f pos = sf::Vector2f(x, y);
	m_sprite.setPosition(pos);
	return pos;
}


void NPC::NPCInit()
{
    if (!m_texture || !m_texture->getSize().x) {
        std::cout << "No texture \n";
    }

    m_sprite.setTexture(*m_texture);
    m_sprite.setPosition(sf::Vector2f(1000, 3000.f));
    m_sprite.setScale(sf::Vector2f(2.5f, 2.5f));
	// IDLE Animations
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::DOWN, 0, 5, 0.08f, 0, 150, 48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::LEFT, 0, 5, 0.08f, 0, 400, 48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::RIGHT, 0, 5, 0.08f, 0, 650, 48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::UP, 0, 5, 0.08f, 0, 900, 48);

	// WALK Animations
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.08f, 0, 200, 48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::LEFT, 0, 6, 0.08f, 0, 450, 48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::RIGHT, 0, 6, 0.08f, 0, 700, 48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::UP, 0, 6, 0.08f, 0, 950, 48);
}
