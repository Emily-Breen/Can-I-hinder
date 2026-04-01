#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "AnimationHandler.h"
#include "NPC.h"


class Effects
{
public:
    Effects(std::shared_ptr<sf::Texture> tex, sf::Vector2f pos, EnemyType type);
    void updateEffects(float dt);
    void drawEffects(sf::RenderWindow& window);
    bool isFinished() const;
    bool shouldSpawn() const;
    sf::Vector2f getSpawnPosition() const;
    EnemyType getSpawnType() const;



private:
    void setupAnimation();
    AnimationHandler m_animationHandler;
    std::shared_ptr<sf::Texture> m_texture;
    sf::Sprite m_sprite;
    
    float m_lifetime = 0.f;
    float m_duration = 0.5f;
    bool m_finished = false;
	bool m_spawnReady = false;
    bool m_hasSpawned = false;
	sf::Vector2f m_spawnPosition;
	EnemyType m_spawnType;

};

