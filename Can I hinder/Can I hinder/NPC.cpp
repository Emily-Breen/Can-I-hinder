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

    if (m_isMoving)
    {
        sf::Vector2f direction(0.f, 0.f);
        switch (m_direction)
        {
        case Direction::DOWN: direction.y += 1.f; break;
        case Direction::LEFT: direction.x -= 1.f; break;
        case Direction::RIGHT: direction.x += 1.f; break;
        case Direction::UP: direction.y -= 1.f; break;
        }
        moveNPC(direction);
    }

    // Animation updates
    m_animationHandler.changeState(m_State);
    m_animationHandler.changeDirection(m_direction);
    m_animationHandler.update(dt);
    m_animationHandler.applyToSprite(m_sprite);

}

void NPC::moveNPC(sf::Vector2f direction)
{
    if (direction == sf::Vector2f(0, 0))
    {
        m_State = PlayerState::IDLE;
        return;
    }

    if (std::abs(direction.x) > std::abs(direction.y))
        m_direction = (direction.x > 0) ? Direction::RIGHT : Direction::LEFT;
    else
        m_direction = (direction.y > 0) ? Direction::DOWN : Direction::UP;

    m_State = PlayerState::WALK;
    m_sprite.move(direction * m_speed * (1.f / 60.f));
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
