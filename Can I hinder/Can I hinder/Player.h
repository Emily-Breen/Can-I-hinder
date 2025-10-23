#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <iostream>

enum class PlayerState
{
	IDLE,
	WALK
};
enum class PlayerDirection
{
	Down = 0,
	Left = 1,
	Right = 2,
	Up = 3
};

class Player : public Entity
{
public:
	Player();
	~Player();
	PlayerState m_state{ PlayerState::IDLE };
	PlayerDirection m_direction = PlayerDirection::Down;
	void playerInit();
	void updateAnimation(float dt) override;
	void playerUpdate(float dt);
	void draw(sf::RenderWindow& window) override;
	void movePlayer(sf::Vector2f direction);
	void updateSpriteRect() override;

private:

};