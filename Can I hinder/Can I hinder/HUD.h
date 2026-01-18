#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class HUD
{
public:
	HUD();
	bool load();
	void layout(const sf::Vector2u& windowSize);
	void update(float hp01,float dt);
	void draw(sf::RenderWindow& window);

private:
	

	sf::Texture m_chatBGTexture;
	sf::Texture m_hotBarTexture;
	sf::Texture m_healthBarDecorTexture;
	sf::Texture m_healthBarTexture;
	sf::Texture m_weaponBgTexture;
	sf::Texture m_weaponBgTexture2;
	sf::Texture m_weaponTexture;

	sf::Sprite m_chatBGSprite;
	sf::Sprite m_hotBarSprite;
	sf::Sprite m_healthBarDecorSprite;
	sf::Sprite m_healthBarSprite;
	sf::Sprite m_weaponBgSprite;
	sf::Sprite m_weaponBgSprite2;

	

	sf::Sprite m_weaponSprite;
	
	float m_hp01 = 1.0f; //health percentage
	float m_hpTarget = 1.0f;
	float m_hpShown = 1.0f;


};

