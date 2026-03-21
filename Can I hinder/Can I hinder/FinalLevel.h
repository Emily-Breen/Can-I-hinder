#pragma once
#include <SFML/Graphics.hpp>

class FinalLevel
{
public:
	FinalLevel();
	void start();
	void updateTB(float dt);
	void drawTB(sf::RenderWindow& window);

private:

	enum class TurnState {
		PLAYER_CHOICE,
		PLAYER_ACTION,
		ENEMY_ACTION,
		WIN,
		LOSE
	};
	TurnState m_turnState{ TurnState::PLAYER_CHOICE };

	int m_playerHealth{ 100 };
	int m_enemyHealth{ 100 };

	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;

	sf::Texture m_playerTexture;
	sf::Sprite m_playerSprite;

	sf::Texture m_bossTexture;
	sf::Sprite m_bossSprite;
};

