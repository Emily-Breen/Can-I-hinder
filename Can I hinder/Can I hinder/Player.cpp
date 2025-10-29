#include "Player.h"

Player::Player() : m_animationHandler(m_texture)
{
	playerInit();
}

Player::~Player()
{
	playerInit();
}
void Player::draw(sf::RenderWindow& window)
{
	Entity::draw(window);
}

void Player::movePlayer(sf::Vector2f direction)
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



void Player::playerInit()
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/Player.png"))
	{
		std::cout << "Failed to load player texture!" << std::endl;
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(sf::Vector2f(100.f, 100.f));
	m_sprite.setScale(sf::Vector2f(2.f, 2.f));


	//addAnimation(PlayerState m_state, Direction m_direction, int startFrame, int frameCount, float frameDuration, int offsetX, int offsetY, int frameWidth) - remember to remove just here to remind of params
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::DOWN, 0, 5, 0.08f, 0 ,150,48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::LEFT, 0, 5, 0.08f, 0 ,400,48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::RIGHT, 0, 5, 0.08f, 0,650,48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::UP, 0, 5, 0.08f, 0 ,900,48);

	m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 8, 0.08f,0, 200,48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::LEFT, 0, 8, 0.08f,0, 450,48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::RIGHT, 0, 8, 0.08f,0,700,48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::UP, 0, 8, 0.08f, 0,950,48);

	m_animationHandler.addAnimation(PlayerState::ATTACK, Direction::DOWN, 0, 6, 0.10f, 0,0,46);
	m_animationHandler.addAnimation(PlayerState::ATTACK, Direction::LEFT, 0, 6, 0.10f, 0,250,48);
	m_animationHandler.addAnimation(PlayerState::ATTACK, Direction::RIGHT, 0, 6, 0.10f, 0,500,48);
	m_animationHandler.addAnimation(PlayerState::ATTACK, Direction::UP, 0, 6, 0.10f,0,750,50);

}


void Player::update(float dt)
{
	sf::Vector2f movement(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
	{
		movement.y -= 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
	{
		movement.y += 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
	{
		movement.x -= 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
	{
		movement.x += 1.f;
	}
	movePlayer(movement);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
	{
		m_State = PlayerState::ATTACK;
	}
	m_animationHandler.changeState(m_State);
	m_animationHandler.changeDirection(m_direction);
	m_animationHandler.update(dt);
	m_animationHandler.applyToSprite(m_sprite);
}



