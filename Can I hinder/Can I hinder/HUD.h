#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <deque> //for chat messages
#include <array> //for hotbar slots
constexpr int HOTBAR_SLOTS = 6;
struct ChatMessage
{
	std::string username;
	std::string message;
	sf::Color userColor;
	sf::Color messageColor;
	float timeToLive;
};
class HUD
{
public:
	HUD();
	bool load();
	void layout(const sf::Vector2u& windowSize);
	void update(float hp01,float dt);
	void draw(sf::RenderWindow& window);
	void pushChatMessage(const std::string& username, const std::string& message, const sf::Color& userColor, const sf::Color& messageColor = sf::Color::White, float timeToLive = 6.0f);

private:
	

	sf::Texture m_chatBGTexture;
	sf::Texture m_hotBarTexture;
	sf::Texture m_hotBarBackgroundTexture;
	sf::Texture m_hotBarBackgroundTexture2;
	sf::Texture m_healthBarDecorTexture;
	sf::Texture m_healthBarTexture;
	sf::Texture m_weaponBgTexture;
	sf::Texture m_weaponBgTexture2;
	sf::Texture m_weaponTexture;

	sf::Sprite m_chatBGSprite;
	sf::Sprite m_hotBarSprite;
	sf::Sprite m_hotBarSlotBackground;
	sf::Sprite m_hotBarCircleBackground;
	sf::Sprite m_healthBarDecorSprite;
	sf::Sprite m_healthBarSprite;
	sf::Sprite m_weaponBgSprite;
	sf::Sprite m_weaponBgSprite2;

	

	sf::Sprite m_weaponSprite;
	
	float m_healthPercentage = 1.0f; //health percentage
	float m_hpTarget = 1.0f;
	float m_hpShown = 1.0f;
	float m_chatMessageLineSpacing = 18.f;

	sf::Font m_font;
	
	std::deque<ChatMessage> m_chatMessages; 
	std::size_t m_maxChatMessages = 10;
	sf::Vector2f m_chatMessageTopLeft{ 0.f, 0.f };
	

	std::array<sf::Vector2f, HOTBAR_SLOTS> m_slotPositions{}; 
	sf::Vector2f m_leftCirclePos{};
	sf::Vector2f m_rightCirclePos{};



};

