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
	sf::FloatRect hb = getBounds();
	sf::RectangleShape box;
	box.setPosition(hb.position);
	box.setSize(hb.size);
	box.setFillColor(sf::Color(255, 0, 0, 100));
	window.draw(box);
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
	
}

void Player::movement(sf::Vector2f t_movement)
{
	m_sprite.move(t_movement);
}

sf::Vector2f Player::getInputDirection()
{
	return m_inputHandler.getMovement();
}
sf::Vector2f Player::getMovement()
{
	return m_inputHandler.getMovement();
}
sf::Vector2f Player::getPosition()
{
	sf::Vector2f pos = m_sprite.getPosition();
	std::cout << "Player Position: (" << pos.x << ", " << pos.y << ")\n";
	return pos;
}

sf::FloatRect Player::getBounds()
{
	sf::FloatRect global = m_sprite.getGlobalBounds();

	// Feet hitbox: narrow width, short height
	float hitboxWidth = global.size.x * 0.40f;
	float hitboxHeight = global.size.y * 0.25f;

	float hitboxLeft = global.position.x + (global.size.x - hitboxWidth) / 2.f;
	float hitboxTop = global.position.y + (global.size.y - hitboxHeight);

	return sf::FloatRect(
		{ hitboxLeft, hitboxTop },
		{ hitboxWidth, hitboxHeight }
	);
}



void Player::playerInit()
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/Player.png"))
	{
		std::cout << "Failed to load player texture!" << std::endl;
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(sf::Vector2f(1590, 4621.f));
	m_sprite.setScale(sf::Vector2f(2.5f, 2.5f));
	m_sprite.setOrigin(sf::Vector2f(24.f, 48.f)); // Center origin based on frame size (48x48)

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
	m_inputHandler.update();
	movePlayer(m_inputHandler.getMovement());
	if (m_inputHandler.isAttackPressed()) {
		m_State = PlayerState::ATTACK;
	}
	
	m_animationHandler.changeState(m_State);
	m_animationHandler.changeDirection(m_direction);
	m_animationHandler.update(dt);
	m_animationHandler.applyToSprite(m_sprite);
	
}



