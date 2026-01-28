#include "Player.h"

Player::Player() : m_animationHandler(m_texture)
{
	playerInit();
}

Player::~Player()
{

}
void Player::draw(sf::RenderWindow& window)
{
	Entity::draw(window);
	sf::FloatRect hb = getBounds();
	sf::RectangleShape box;
	box.setPosition(hb.position);
	box.setSize(hb.size);
	//box.setFillColor(sf::Color(255, 0, 0, 100));
	//window.draw(box);
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
	return pos;
}

void Player::setPosition(sf::Vector2f position)
{
	m_sprite.setPosition(position);
}

sf::FloatRect Player::getBounds() const
{
	sf::FloatRect global = m_sprite.getGlobalBounds();

	//hitbox for the player feet area for collision AABB
	float hitboxWidth = global.size.x * 0.40f;
	float hitboxHeight = global.size.y * 0.25f;

	float hitboxLeft = global.position.x + (global.size.x - hitboxWidth) / 2.f;
	float hitboxTop = global.position.y + (global.size.y - hitboxHeight);

	return sf::FloatRect(
		{ hitboxLeft, hitboxTop },
		{ hitboxWidth, hitboxHeight }
	);
}

sf::FloatRect Player::getAttackBounds() const
{
	sf::FloatRect feet = getBounds();
	const float range = 35.f;
	const float swingWidth = 30.f;

	sf::FloatRect attackBox = feet;

	switch (m_direction) {
	case Direction::UP:
		attackBox.position.y -= range;
		attackBox.size.y = range;
		attackBox.position.x -= swingWidth * 0.5f;
		attackBox.size.x += swingWidth;
		break;
	case Direction::DOWN:
		attackBox.position.y += feet.size.y;
		attackBox.size.y = range;
		attackBox.position.x -= swingWidth * 0.5f;
		attackBox.size.x += swingWidth;
		break;
	case Direction::LEFT:
		attackBox.position.x -= range;
		attackBox.size.x = range;
		attackBox.position.y -= swingWidth * 0.5f;
		attackBox.size.y += swingWidth;
		break;
	case Direction::RIGHT:
		attackBox.position.x += feet.size.x;
		attackBox.size.x = range;
		attackBox.position.y -= swingWidth * 0.5f;
		attackBox.size.y += swingWidth;
		break;
	}
	return attackBox;

	
}

void Player::takeDamage(float duration)
{
	
	if (m_hurtTimer > 0.f) 
		return;
	if (m_invulnerabilityTimer > 0.f) 
		return;

	m_invulnerabilityTimer = 0.6f;
	m_isInvulnerable = true;
	m_hurtTimer = duration;
}

bool Player::isHurt() const
{
	return m_hurtTimer > 0.f;
}

void Player::dead()
{
	if (m_isDead)
		return;
	m_isDead = true;

	m_hurtTimer = 0.f;
	m_invulnerabilityTimer = 0.f;

	m_deathTimer = m_deathDuration;
	m_animationHandler.changeState(PlayerState::DEATH);
}

bool Player::isDead() const
{
	return m_isDead;
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
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::DOWN, 0, 5, 0.08f, 0 ,150,48, 48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::LEFT, 0, 5, 0.08f, 0 ,400,48, 48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::RIGHT, 0, 5, 0.08f, 0,650,48, 48);
	m_animationHandler.addAnimation(PlayerState::IDLE, Direction::UP, 0, 5, 0.08f, 0 ,900,48, 48);

	m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 8, 0.08f,0, 200,48, 48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::LEFT, 0, 8, 0.08f,0, 450,48, 48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::RIGHT, 0, 8, 0.08f,0,700,48, 48);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::UP, 0, 8, 0.08f, 0,950,48, 48);
	
	m_animationHandler.addAnimation(PlayerState::ATTACK, Direction::DOWN, 0, 6, 0.10f, 0,0,46, 48);
	m_animationHandler.addAnimation(PlayerState::ATTACK, Direction::LEFT, 0, 6, 0.10f, 0,250,48, 48);
	m_animationHandler.addAnimation(PlayerState::ATTACK, Direction::RIGHT, 0, 6, 0.10f, 0,500,48, 48);
	m_animationHandler.addAnimation(PlayerState::ATTACK, Direction::UP, 0, 6, 0.10f,0,750,50, 48);

	m_animationHandler.addAnimation(PlayerState::HURT, Direction::DOWN, 0, 4, 0.10f, 0, 93, 48, 48);
	m_animationHandler.addAnimation(PlayerState::HURT, Direction::LEFT, 0, 4, 0.10f, 0, 350, 48, 48);
	m_animationHandler.addAnimation(PlayerState::HURT, Direction::RIGHT, 0, 4, 0.10f, 0, 600, 48, 48);
	m_animationHandler.addAnimation(PlayerState::HURT, Direction::UP, 0, 4, 0.10f, 0, 845, 48, 48);

	m_animationHandler.addAnimation(PlayerState::DEATH, Direction::DOWN, 0, 5, 0.10f, 0, 50, 48, 48);
	m_animationHandler.addAnimation(PlayerState::DEATH, Direction::LEFT, 0, 5, 0.10f, 0, 305, 48, 48);
	m_animationHandler.addAnimation(PlayerState::DEATH, Direction::RIGHT, 0, 5, 0.10f, 0, 550, 48, 48);
	m_animationHandler.addAnimation(PlayerState::DEATH, Direction::UP, 0, 5, 0.10f, 0, 800, 48, 48);

}


void Player::update(float dt)
{
	if (m_isDead)
	{
		
		if (m_deathTimer > 0.f)
		{
			m_deathTimer -= dt;
			if (m_deathTimer < 0.f) m_deathTimer = 0.f;
			m_animationHandler.update(dt);
		}
		m_animationHandler.changeState(PlayerState::DEATH);
		m_animationHandler.changeDirection(m_direction);
		m_animationHandler.applyToSprite(m_sprite);
		return;
	}
	if (m_hurtTimer > 0.f)
	{
		m_hurtTimer -= dt;
		if (m_hurtTimer < 0.f) 
			m_hurtTimer = 0.f;
	}

	if (m_invulnerabilityTimer > 0.f)
	{
		m_invulnerabilityTimer -= dt;
		if (m_invulnerabilityTimer < 0.f)
		{
			m_invulnerabilityTimer = 0.f;
			m_isInvulnerable = false;
		}
	}
	
	m_inputHandler.update();
	movePlayer(m_inputHandler.getMovement());
	const bool attackHeld = m_inputHandler.isAttackPressed();
	const bool attackJustPressed = attackHeld && !m_attackWasHeld;
	m_attackWasHeld = attackHeld;

	// Start attack on press (only if not hurt/dead)
	if (attackJustPressed && m_attackTimer <= 0.f && m_hurtTimer <= 0.f)
	{
		m_attackTimer = m_attackDuration;
	}
	if (m_attackTimer > 0.f)
	{
		m_attackTimer -= dt;
		if (m_attackTimer < 0.f) m_attackTimer = 0.f;
	}
	PlayerState baseState = m_State;

	if (m_attackTimer > 0.f)
		baseState = PlayerState::ATTACK;

	PlayerState animState = (m_hurtTimer > 0.f) ? PlayerState::HURT : baseState;

	m_State = baseState;

	m_animationHandler.changeState(animState);
	m_animationHandler.changeDirection(m_direction);
	m_animationHandler.update(dt);
	m_animationHandler.applyToSprite(m_sprite);
}



