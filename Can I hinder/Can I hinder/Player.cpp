#include "Player.h"

Player::Player()
{
	playerInit();
}

Player::~Player()
{
}
void Player::draw(sf::RenderWindow& window)
{
	Entity::draw(window);
}

void Player::movePlayer(sf::Vector2f direction)
{
	if (direction.x != 0.f || direction.y != 0) {
		m_sprite.move(direction);
		m_state = PlayerState::WALK;
	}
	else {
		m_state = PlayerState::IDLE;
	}
	
}

void Player::updateSpriteRect()
{
	
	m_sprite.setTextureRect(sf::IntRect(
		{ m_currentFrame * frameWidth, 0 },
		{ frameWidth, frameHeight }
	));
}

void Player::playerInit()
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/test.png"))
	{
		std::cout << " Problem loading player texture!" << std::endl;
	
	}
	else
	{
		std::cout << " Loaded player texture successfully!" << std::endl;
		std::cout << "Texture size: " << m_texture.getSize().x << "x" << m_texture.getSize().y << std::endl;
		m_sprite = sf::Sprite(m_texture);
	}
	
	int frameCount = 4;
	int frameWidth = 5424/113;
	int frameHeight = 48;
	int frameIndex = 0;

	m_sprite.setTextureRect(sf::IntRect({ frameIndex * frameWidth, 0 }, { frameWidth, frameHeight }));
	m_sprite.setOrigin(sf::Vector2f(frameWidth / 2.f, frameHeight / 2.f));
	m_sprite.setScale(sf::Vector2f(0.2f, 0.2f));
	m_sprite.setPosition(sf::Vector2f(400.f, 300.f));


}
void Player::updateAnimation(float dt)
{
	m_animationTimer += dt;
	if (m_animationTimer >= m_animationDelay) {
		m_animationTimer = 0.0f;
		m_currentFrame++;
		if (m_currentFrame > m_lastFrame) {
			m_currentFrame = m_firstFrame;
		}
		updateSpriteRect();
	}
}

void Player::playerUpdate(float dt)
{
	if (m_state == PlayerState::WALK) {
		updateAnimation(dt);
	}
	else {
		m_currentFrame = 0;
		updateSpriteRect();
	}
}


