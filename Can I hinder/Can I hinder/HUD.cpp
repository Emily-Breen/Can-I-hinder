#include "HUD.h"
// Health bar texture coordinates and dimensions using constexpr better at compile time
namespace
{
	constexpr float DECOR_TEX_WIDTH = 256.f;   
	constexpr float FILL_START_X_TEX = 56.f;    
	constexpr float FILL_Y_TEX = 0.f;     
}
HUD::HUD()
	: m_chatBGSprite(m_chatBGTexture)
	, m_hotBarSprite(m_hotBarTexture)
	, m_healthBarDecorSprite(m_healthBarDecorTexture)
	, m_healthBarSprite(m_healthBarTexture)
	, m_weaponBgSprite(m_weaponBgTexture)
	, m_weaponBgSprite2(m_weaponBgTexture2)
	,m_weaponSprite(m_weaponTexture)
	, m_hotBarSlotBackground(m_hotBarBackgroundTexture)
	, m_hotBarCircleBackground(m_hotBarBackgroundTexture2)
	, m_keySprite(m_keyTexture)
	, m_healthPotionSprite(m_healthPotionTexture)
	, m_speedPotionSprite(m_speedPotionTexture)
	, m_powerPotionSprite(m_powerPotionTexture)
{
}

bool HUD::load()
{
	//loading textures for hud
    if (!m_chatBGTexture.loadFromFile("ASSETS/IMAGES/HUD/ChatBackground.png")) 
        return false;
    if (!m_hotBarTexture.loadFromFile("ASSETS/IMAGES/HUD/Hotbar.png")) 
        return false;
	if (!m_hotBarBackgroundTexture.loadFromFile("ASSETS/IMAGES/HUD/HotbarBackground.png"))
		return false;
	if (!m_hotBarBackgroundTexture2.loadFromFile("ASSETS/IMAGES/HUD/HotBarBackground2.png"))
		return false;
    if (!m_healthBarDecorTexture.loadFromFile("ASSETS/IMAGES/HUD/health_bar_decoration.png"))
        return false;
	if (!m_healthBarTexture.loadFromFile("ASSETS/IMAGES/HUD/health_bar.png")) 
		return false;
    if (!m_weaponBgTexture.loadFromFile("ASSETS/IMAGES/HUD/currentWeaponBackground.png")) 
        return false;
	if (!m_weaponBgTexture2.loadFromFile("ASSETS/IMAGES/HUD/currentWeapon.png"))
		return false;
	if (!m_keyTexture.loadFromFile("ASSETS/IMAGES/ITEMS/key.png"))
		return false;
	if(!m_healthPotionTexture.loadFromFile("ASSETS/IMAGES/Items/BigHealthPotion.png"))
		return false;
	if (!m_speedPotionTexture.loadFromFile("ASSETS/IMAGES/Items/BigSpeedBoostPotion.png"))
		return false;
	if (!m_powerPotionTexture.loadFromFile("ASSETS/IMAGES/Items/BigStrengthPotion.png"))
		return false;
	if (!m_font.openFromFile("ASSETS/FONTS/Jersey20-Regular.ttf"))
		return false;

	// Initialize sprites
    m_chatBGSprite = sf::Sprite(m_chatBGTexture);
    m_hotBarSprite = sf::Sprite(m_hotBarTexture);
    m_healthBarDecorSprite = sf::Sprite(m_healthBarDecorTexture);
	m_healthBarSprite = sf::Sprite(m_healthBarTexture);
    m_weaponBgSprite = sf::Sprite(m_weaponBgTexture);
	m_weaponBgSprite2 = sf::Sprite(m_weaponBgTexture2);
	m_hotBarSlotBackground = sf::Sprite(m_hotBarBackgroundTexture);
	m_hotBarCircleBackground = sf::Sprite(m_hotBarBackgroundTexture2);
	m_keySprite = sf::Sprite(m_keyTexture);
	m_healthPotionSprite = sf::Sprite(m_healthPotionTexture);
	m_speedPotionSprite = sf::Sprite(m_speedPotionTexture);
	m_powerPotionSprite = sf::Sprite(m_powerPotionTexture);

	m_healthPotionSprite.setTextureRect({ {0, 0}, {16, 16} });
	m_speedPotionSprite.setTextureRect({ {0, 0}, {16, 16} });
	m_powerPotionSprite.setTextureRect({ {0, 0}, {16, 16} });

	// Scaling
	m_healthBarDecorSprite.setScale({ 6.f,6.f });
	m_chatBGSprite.setScale({ 12.f, 6.f });
	m_hotBarSprite.setScale({ 3.f, 3.f });
	m_weaponBgSprite.setScale({ 2.7f, 2.7f });
	m_weaponBgSprite2.setScale({ 2.7f, 2.7f });
	m_hotBarSlotBackground.setScale({ 3.f, 3.f });
	m_hotBarCircleBackground.setScale({ 3.2f, 3.2f });
	m_keySprite.setScale({ 2.f,2.f });
	m_healthPotionSprite.setScale({ 3.f, 3.f });
	m_speedPotionSprite.setScale({ 3.f, 3.f });
	m_powerPotionSprite.setScale({ 3.f, 3.f });
	


    return true;
}
void HUD::layout(const sf::Vector2u& windowSize)
{
	//this whole function is to keep HUD achored (hopefully!)
	sf::Vector2f size{ (float)windowSize.x, (float)windowSize.y };

	m_healthBarDecorSprite.setPosition({ 30.f, 25.f });
	m_weaponBgSprite.setPosition({ 0.f, size.y - 160.f });
	m_weaponBgSprite2.setPosition({ 0.f, size.y - 160.f });
	
	m_hotBarSprite.setPosition({ size.x * 0.5f - 250.f, size.y - 140.f });

	const sf::Vector2f hotbarPos = m_hotBarSprite.getPosition();

	const sf::Vector2f firstSlotPos = hotbarPos + sf::Vector2f(127.f, 67.f); 
	const float paddingX = 60.f; 
	const sf::Vector2f leftCirclePos = hotbarPos + sf::Vector2f(10.f, 10.f);  
	const sf::Vector2f rightCirclePos = hotbarPos + sf::Vector2f(490.f, 10.f);

	for (int i = 0; i < 6; ++i)
	{
		m_slotPositions[i] = firstSlotPos + sf::Vector2f(paddingX * i, 0.f);
	}

	m_leftCirclePos = leftCirclePos;
	m_rightCirclePos = rightCirclePos;

	m_chatBGSprite.setPosition({ size.x - 420.f, 30.f });

	const float margin = 30.f;
	sf::FloatRect chatLocal = m_chatBGSprite.getLocalBounds();

	m_chatBGSprite.setPosition({
		size.x - chatLocal.size.x * m_chatBGSprite.getScale().x - margin,
		margin
		});

	// Text starts inside the chat box with some padding
	m_chatMessageTopLeft = m_chatBGSprite.getPosition() + sf::Vector2f(60.f, 28.f);


	//for inner healthbar fill position and scale
	const sf::Vector2f decorScale = m_healthBarDecorSprite.getScale();
	const sf::Vector2f decorPos = m_healthBarDecorSprite.getPosition();

	m_healthBarSprite.setScale(decorScale);

	const float fillOffsetX =
		(FILL_START_X_TEX / DECOR_TEX_WIDTH) *
		m_healthBarDecorSprite.getGlobalBounds().size.x;

	m_healthBarSprite.setPosition({
		decorPos.x + fillOffsetX,
		decorPos.y + FILL_Y_TEX * decorScale.y
	});


}
void HUD::update(float hp01, float dt)
{
	//smooth lerp for health bar rather than snapping
	m_hpTarget = std::clamp(hp01, 0.f, 1.f);

	const float speed = 10.f; 
	const float alpha = 1.f - std::exp(-speed * dt);

	m_hpShown += (m_hpTarget - m_hpShown) * alpha;

	const auto texSize = m_healthBarTexture.getSize();
	const int fullW = static_cast<int>(texSize.x);
	const int fullH = static_cast<int>(texSize.y);
	
	const int visibleWidth = std::max(0, std::min(fullW, (int)std::round(fullW * m_hpShown)));
	
	sf::IntRect rect{
		sf::Vector2i{0, 0},
		sf::Vector2i{visibleWidth, fullH}
	};

	m_healthBarSprite.setTextureRect(rect);
	//update chat messages time to live
	for (auto& msg : m_chatMessages)
	{
		msg.timeToLive -= dt;
	}
	while (!m_chatMessages.empty() && m_chatMessages.front().timeToLive <= 0.f)
	{
		m_chatMessages.pop_front();
	}
}

void HUD::draw(sf::RenderWindow& window)
{
	
	window.draw(m_healthBarDecorSprite);
	window.draw(m_healthBarSprite);

	window.draw(m_chatBGSprite);
	sf::Text userNameText(m_font);
	sf::Text messageText(m_font);
	userNameText.setCharacterSize(20);
	messageText.setCharacterSize(20);

	sf::Vector2f chatPos = m_chatMessageTopLeft;

	for (const auto& msg : m_chatMessages)
	{
		auto fade = [&](sf::Color color)
			{
				// last 1 second fades out
				if (msg.timeToLive < 1.f)
				{
					
					color.a = static_cast<std::uint8_t>(
						255.f * std::clamp(msg.timeToLive / 1.f, 0.f, 1.f)
						);
				}
				return color;
			};

		userNameText.setString(msg.username + ": ");
		userNameText.setFillColor(fade(msg.userColor));
		userNameText.setPosition(chatPos);
		window.draw(userNameText);

		sf::FloatRect userBounds = userNameText.getLocalBounds();

		messageText.setString(msg.message);
		messageText.setFillColor(fade(msg.messageColor));
		messageText.setPosition({ chatPos.x + userBounds.size.x, chatPos.y });
		window.draw(messageText);

		chatPos.y += m_chatMessageLineSpacing;
	}
	//for left circle
	m_hotBarCircleBackground.setPosition(m_leftCirclePos);
	window.draw(m_hotBarCircleBackground);

	// draw the 6 HB slots
	for (int i = 0; i < 6; ++i)
	{
		m_hotBarSlotBackground.setPosition(m_slotPositions[i]);
		window.draw(m_hotBarSlotBackground);

		if (i == m_selectedSlot)
		{
			sf::RectangleShape highlight({ 35.f, 35.f });
			highlight.setPosition(m_slotPositions[i]);
			highlight.setFillColor(sf::Color::Transparent);
			highlight.setOutlineColor(sf::Color::Yellow);
			highlight.setOutlineThickness(3.f);

			window.draw(highlight);
		}
	}
	//keys in hotbar 
	for (int i = 0; i < std::min(m_keys, HOTBAR_SLOTS); i++)
	{
		sf::Sprite key = m_keySprite;

		key.setPosition({
			m_slotPositions[i].x + 12.f,
			m_slotPositions[i].y + 10.f
			});

		window.draw(key);
	}
	sf::Text countText(m_font);
	countText.setCharacterSize(20);
	countText.setFillColor(sf::Color::White);
	if (m_healthPotions > 0)
	{
		auto sprite = m_healthPotionSprite;
		sprite.setPosition(m_slotPositions[3]);
		window.draw(sprite);

		countText.setString(std::to_string(m_healthPotions));
		countText.setPosition(m_slotPositions[3] + sf::Vector2f(30.f, 20.f));
		window.draw(countText);
	}
	if (m_speedPotions > 0)
	{
		auto sprite = m_speedPotionSprite;
		sprite.setPosition(m_slotPositions[4]);
		window.draw(sprite);

		countText.setString(std::to_string(m_speedPotions));
		countText.setPosition(m_slotPositions[4] + sf::Vector2f(30.f, 20.f));
		window.draw(countText);
	}
	if (m_powerPotions > 0)
	{
		auto sprite = m_powerPotionSprite;
		sprite.setPosition(m_slotPositions[5]);
		window.draw(sprite);

		countText.setString(std::to_string(m_powerPotions));
		countText.setPosition(m_slotPositions[5] + sf::Vector2f(30.f, 20.f));
		window.draw(countText);
	}
	
	//for the right circle
	m_hotBarCircleBackground.setPosition(m_rightCirclePos);
	window.draw(m_hotBarCircleBackground);

	window.draw(m_hotBarSprite);
	//window.draw(m_weaponBgSprite);
	//window.draw(m_weaponBgSprite2);


}

void HUD::pushChatMessage(const std::string& username, const std::string& message, const sf::Color& userColor, const sf::Color& messageColor, float timeToLive)
{
	m_chatMessages.push_back(ChatMessage{ username, message, userColor, messageColor, timeToLive });
	while (m_chatMessages.size() > m_maxChatMessages)
	{
		m_chatMessages.pop_front();
	}
}

void HUD::addKey()
{
	m_keys++;
}

void HUD::addHealthPotion()
{
	m_healthPotions++;
}

void HUD::addSpeedPotion()
{
	m_speedPotions++;
}

void HUD::addPowerPotion()
{
	m_powerPotions++;	
}

void HUD::useHealthPotion()
{
	if (m_healthPotions > 0)
	{
		m_healthPotions--;
	}
}

bool HUD::hasHealthPotion() const
{
	return m_healthPotions > 0;
}

void HUD::useSpeedPotion()
{
	if (m_speedPotions > 0)
	{
		m_speedPotions--;
	}
}

bool HUD::hasSpeedPotion() const
{
	return m_speedPotions > 0;
}

void HUD::usePowerPotion()
{
	if (m_powerPotions > 0)
	{
		m_powerPotions--;
	}
}

bool HUD::hasPowerPotion() const
{
	return m_powerPotions > 0;
}

void HUD::clearKeys()
{
	m_keys = 0;
}

void HUD::clearPotions()
{
	m_healthPotions = 0;
	m_speedPotions = 0;
	m_powerPotions = 0;
}

int HUD::getHealthPotionCount() const
{
	return m_healthPotions;
}

int HUD::getSpeedPotionCount() const
{
	return m_speedPotions;
}

int HUD::getPowerPotionCount() const
{
	return m_powerPotions;
}

void HUD::setSelectedSlot(int slot)
{
	m_selectedSlot = slot;
}

sf::Vector2f HUD::getSlotPosition(int index) const
{
	return m_slotPositions[index];
}
