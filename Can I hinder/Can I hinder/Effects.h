#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "AnimationHandler.h"
#include "NPC.h"
#include "Player.h"
#include <functional>
enum class EffectType
{
    SpawnEnemy,
    Fireball,
    FireHit,
    Thunder,
	ThunderHit,
    Ice,
	IceHit,
    Gravity,
    GravityHit,
    Healing,
    SpeedBoost,
    PowerBoost,
    StealPower,
    SlowPlayer,
    SheildPlayer,
    GodMode,
    SpawnTrap
};


class Effects
{
public:
    Effects(std::shared_ptr<sf::Texture> tex, sf::Vector2f pos, EnemyType type, EffectType effectType);
    void updateEffects(float dt);
    void drawEffects(sf::RenderWindow& window);
    void markAsSpawned();
	void setEffectScale(sf::Vector2f scale);
    bool isFinished() const;
    bool shouldSpawn() const;
	bool hasSpawned() const;
    sf::Vector2f getSpawnPosition() const;
    EffectType getEffectType() const;
    void setEffectVelocity(sf::Vector2f vel);
	void setEffectColor(const sf::Color& color);
	void attachToPlayer(Player* player);
    void setOnFinish(std::function<void()> func);
    

    EnemyType getSpawnType() const;



private:
    void setupAnimation();
    AnimationHandler m_animationHandler;
    std::shared_ptr<sf::Texture> m_texture;
    std::function<void()> m_onFinish;
    sf::Sprite m_sprite;
    EffectType m_effectType;
	Player* m_followPlayer = nullptr;
    float m_lifetime = 0.f;
    float m_duration = 0.5f;
    float m_damage = 0.f;
    float m_heal = 0.f;
    bool m_finished = false;
    bool m_spawnReady = false;
    bool m_hasSpawned = false;
    bool m_triggered = false;
    bool m_isMoving = false;

    sf::Vector2f m_spawnPosition;
    sf::Vector2f m_velocity{ 0.f, 0.f };
    EnemyType m_spawnType;

    
};

