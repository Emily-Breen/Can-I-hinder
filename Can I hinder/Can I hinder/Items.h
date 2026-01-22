#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <cmath>
#include <iostream>
#include "AnimationHandler.h"

enum class ItemType {
	healthPotion,
	SpeedBoost,
	StrengthBoost,
	Key
};
struct ItemEffect {
	ItemType type{ ItemType::Key };
	float amount{ 0.f };
	float duration{ 0.f };
	int quantity{ 0 };
};
class Items
{
public:
	Items(ItemEffect effect, sf::Vector2f worldPos, float pickupRadius = 0.f);
	void update(float dt);
	void draw(sf::RenderWindow& window) const;
	sf::FloatRect getBounds() const;
	sf::Vector2f getPosition() const;
	bool isCollected() const;
	void collect();
	const ItemEffect& getEffect() const;

private:
	static sf::Texture& loadItemTextures(ItemType type);
	void itemInit(sf::Vector2f worldPos);
	ItemEffect m_effect;
	sf::Texture& m_texture;       
	sf::Sprite m_sprite;          
	AnimationHandler m_animation;  

	bool m_collected{ false };
	float m_bobTimer{ 0.f };
	float m_baseY{ 0.f };
	float m_pickupRadius{ 0.f };
	
};

