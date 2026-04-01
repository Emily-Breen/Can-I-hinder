#include "Effects.h"
//constructor to initialize the effect with texture, position and enemy type it will spawn
Effects::Effects(std::shared_ptr<sf::Texture> tex, sf::Vector2f pos, EnemyType type)
    : m_animationHandler(*tex), m_texture(std::move(tex)), m_sprite(*m_texture)
{
    m_sprite.setPosition(pos);
    setupAnimation();
    m_spawnPosition = pos;
    m_spawnType = type;
    m_animationHandler.changeState(PlayerState::WALK);
    m_animationHandler.changeDirection(Direction::DOWN);
	m_sprite.setScale(sf::Vector2f(4.f, 4.f));
    m_sprite.setOrigin({ 25.f, 50.f });
}
// Update the effect's animation and check if it has finished
void Effects::updateEffects(float dt)
{
    m_lifetime += dt;

    if (!m_finished)
    {
        m_animationHandler.update(dt);
        m_animationHandler.applyToSprite(m_sprite);
    }

    if (m_lifetime >= m_duration && !m_finished)
    {
        m_finished = true;
        m_spawnReady = true;
    }
}
// Draw the effect's sprite to the window
void Effects::drawEffects(sf::RenderWindow& window) 
{
    window.draw(m_sprite);
}
// Check if the effect's animation has finished
bool Effects::isFinished() const
{
    return m_finished; 
}
// Check if the effect is ready to spawn the enemy
bool Effects::shouldSpawn() const
{
   return m_spawnReady;
}
// Get the position where the enemy should spawn
sf::Vector2f Effects::getSpawnPosition() const
{
	return m_spawnPosition;
}
// Get the type of enemy that should spawn
EnemyType Effects::getSpawnType() const
{
	return m_spawnType;
}
// Set up the animation frames for the effect
void Effects::setupAnimation()
{
   
    m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN,0, 5,0.08f, 0,0, 50, 50);
    m_animationHandler.addAnimation(PlayerState::WALK, Direction::LEFT, 0, 5, 0.08f, 0, 0, 50, 50);
    m_animationHandler.addAnimation(PlayerState::WALK, Direction::RIGHT, 0, 5, 0.08f, 0, 0, 50, 50);
	m_animationHandler.addAnimation(PlayerState::WALK, Direction::UP, 0, 5, 0.08f, 0, 0, 50, 50);
   
}
