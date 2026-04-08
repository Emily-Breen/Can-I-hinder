#include "Effects.h"
//constructor to initialize the effect with texture, position and enemy type it will spawn
Effects::Effects(std::shared_ptr<sf::Texture> tex, sf::Vector2f pos, EnemyType type, EffectType effectType)
    : m_animationHandler(*tex), m_texture(std::move(tex)), m_sprite(*m_texture),m_effectType(effectType)
{
    m_sprite.setPosition(pos);
    setupAnimation();
    m_spawnPosition = pos;
    m_spawnType = type;
    m_animationHandler.changeState(PlayerState::WALK);

    m_animationHandler.update(0.f);
    m_animationHandler.applyToSprite(m_sprite);
   
}
// Update the effect's animation and check if it has finished
void Effects::updateEffects(float dt)
{
    m_lifetime += dt;

    if (m_isMoving)
    {
        m_sprite.move(m_velocity * dt);
    }
    if (m_effectType == EffectType::SpawnEnemy)
    {
        m_spawnReady = true;
        std::cout << "SpawnEnemy effect ready!\n"; // debug
    }
    if (!m_finished)
    {
        m_animationHandler.update(dt);
        m_animationHandler.applyToSprite(m_sprite);
    }

    if (m_lifetime >= m_duration && !m_finished)
    {
        m_finished = true;
        if (m_onFinish)
            m_onFinish();
    }
  if (m_followPlayer)
{
    sf::Vector2f playerPos = m_followPlayer->getPosition();
    m_sprite.setPosition(playerPos + sf::Vector2f(0.f, -50.f));
}
}
// Draw the effect's sprite to the window
void Effects::drawEffects(sf::RenderWindow& window) 
{
    window.draw(m_sprite);
}
void Effects::markAsSpawned()
{
	m_hasSpawned = true;
}
void Effects::setEffectScale(sf::Vector2f scale)
{
	m_sprite.setScale(scale);
}
// Check if the effect's animation has finished
bool Effects::isFinished() const
{
    if (m_effectType == EffectType::SpawnEnemy)
        return m_finished && m_hasSpawned;

    return m_finished; 
}
// Check if the effect is ready to spawn the enemy
bool Effects::shouldSpawn() const
{
    return m_finished && !m_hasSpawned;
}
bool Effects::hasSpawned() const
{
	return m_hasSpawned;
}
// Get the position where the enemy should spawn
sf::Vector2f Effects::getSpawnPosition() const
{
	return m_spawnPosition;
}

EffectType Effects::getEffectType() const
{
	return m_effectType;
}

void Effects::setEffectVelocity(sf::Vector2f vel)
{
    m_velocity = vel;
    m_isMoving = true;
}

void Effects::setEffectColor(const sf::Color& color)
{
	m_sprite.setColor(color);
}

void Effects::attachToPlayer(Player* player)
{
    m_followPlayer = player;
}
void Effects::setOnFinish(std::function<void()> func)
{
    m_onFinish = func;
}
// Get the type of enemy that should spawn
EnemyType Effects::getSpawnType() const
{
	return m_spawnType;
}
// Set up the animation frames for the effect
void Effects::setupAnimation()
{
   
    if (m_effectType == EffectType::Fireball)
    {
       
       m_animationHandler.addAnimation(PlayerState::WALK,Direction::RIGHT,0,6,0.05f,0, 0,64, 64);

        m_animationHandler.changeDirection(Direction::RIGHT);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({4.f, 4.f});
        m_duration = 2.0f; 
    }
    else if (m_effectType == EffectType::FireHit)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 1.0f; 
	}
    else if(m_effectType == EffectType::Thunder)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.1f, 0, 0, 32, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 16.f, 64.f });
        m_sprite.setScale({6.f, 15.f});
        m_duration = 1.5f; 
	}
    else if (m_effectType == EffectType::ThunderHit)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 1.0f;
    }
    else if (m_effectType == EffectType::SpeedBoost)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);

        m_animationHandler.changeDirection(Direction::DOWN);

        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 2.5f, 2.5f });

        m_duration = 0.6f; // quick burst
    }
    else if (m_effectType == EffectType::PowerBoost)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);

        m_animationHandler.changeDirection(Direction::DOWN);

        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 2.5f, 2.5f });

        m_duration = 0.6f; // quick burst
    }
    else if(m_effectType == EffectType::StealPower)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 2.5f, 2.5f });
        m_duration = 0.6f; 
	}
    else if (m_effectType == EffectType::SlowPlayer) {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 0.6f; 
    }
    else if (m_effectType == EffectType::SheildPlayer)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 7, 0.08f, 0, 0, 64, 64);

        m_animationHandler.changeDirection(Direction::DOWN);

        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });

        m_duration = 6.0f; 
    }
    else if (m_effectType == EffectType::SpawnEnemy)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 5, 0.08f, 0, 0, 50, 50);

        m_animationHandler.changeDirection(Direction::DOWN);

        m_sprite.setOrigin({ 25.f, 25.f });
        m_sprite.setScale({ 4.f, 4.f });

        m_duration = 0.5f;

        setOnFinish([this]()
            {
                m_spawnReady = true;
            });
    }
    else if (m_effectType == EffectType::Gravity)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 2.0f; 
	}
    else if (m_effectType == EffectType::GravityHit)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 1.0f;
    }
    else if (m_effectType == EffectType::Ice)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 1.0f;
    }
    else if(m_effectType == EffectType::IceHit)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 1.0f;
	}
    else if (m_effectType == EffectType::Healing)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 128);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 64.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 1.0f;
    }
    else if (m_effectType == EffectType::GodMode)
    {
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 6, 0.05f, 0, 0, 64, 64);
        m_animationHandler.changeDirection(Direction::DOWN);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 3.f, 3.f });
        m_duration = 0.5f; 
	}
    else
    {
       
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::DOWN, 0, 5, 0.08f, 0, 0, 50, 50);
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::LEFT, 0, 5, 0.08f, 0, 0, 50, 50);
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::RIGHT, 0, 5, 0.08f, 0, 0, 50, 50);
        m_animationHandler.addAnimation(PlayerState::WALK, Direction::UP, 0, 5, 0.08f, 0, 0, 50, 50);

        m_sprite.setOrigin({ 25.f, 25.f });
        m_sprite.setScale({4.f, 4.f});

        m_duration = 0.5f;
    }
   
}
