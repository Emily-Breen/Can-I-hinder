#include "Items.h"

Items::Items(ItemEffect effect, sf::Vector2f worldPos, float pickupRadius)
	: m_effect(effect)
	, m_texture(loadItemTextures(effect.type))
	, m_sprite(m_texture)         
	, m_animation(m_texture)       
	, m_pickupRadius(pickupRadius)
{
	itemInit(worldPos);

}

void Items::update(float dt)
{
	if (m_collected) 
		return;

	//just for a bob look onscreen
	m_bobTimer += dt;
	const float bob = std::sin(m_bobTimer * 4.f) * 3.f; 
	auto pos = m_sprite.getPosition();
	pos.y = m_baseY + bob;
	m_sprite.setPosition(pos);

	m_animation.update(dt);
	m_animation.applyToSprite(m_sprite);
}

void Items::draw(sf::RenderWindow& window) const
{
	if (m_collected) 
		return;
	window.draw(m_sprite);
}

sf::FloatRect Items::getBounds() const
{
	return m_sprite.getGlobalBounds();
}

sf::Vector2f Items::getPosition() const
{
	return m_sprite.getPosition();
}

bool Items::isCollected() const
{
	return m_collected;
}

void Items::collect()
{
	m_collected = true;
}

const ItemEffect& Items::getEffect() const
{
	return m_effect;
}

sf::Texture& Items::loadItemTextures(ItemType type) //doing it this way to avoid loading multiple times
{
	static sf::Texture textures[4];
	static bool loaded[4]{ false };

	const int i = static_cast<int>(type);

	if (!loaded[i])
	{
		static const char* paths[]{
			"ASSETS/IMAGES/Items/BigHealthPotion.png",
			"ASSETS/IMAGES/Items/BigSpeedBoostPotion.png",
			"ASSETS/IMAGES/Items/BigStrengthPotion.png",
			"ASSETS/IMAGES/Items/key.png"
		};

		if (!textures[i].loadFromFile(paths[i]))
		{
			std::cout << "Failed to load item texture: " << paths[i] << "\n";
		}
		loaded[i] = true;
	}

	return textures[i];
}

void Items::itemInit(sf::Vector2f worldPos)
{
	m_texture = loadItemTextures(m_effect.type);  
	
	m_sprite.setPosition(worldPos);
	m_sprite.setScale({ 2.f, 2.f });
	m_sprite.setOrigin({ 8.f, 8.f }); 

	m_baseY = worldPos.y;

	
	const int frameW = 16;
	const int frameH = 16;

	
	const int frames = static_cast<int>(m_texture.getSize().x) / frameW;

	m_animation.addAnimation(PlayerState::IDLE, Direction::DOWN,
		0, frames, 0.12f,
		0, 0, frameW, frameH);

	m_animation.changeState(PlayerState::IDLE);
	m_animation.changeDirection(Direction::DOWN);
}
