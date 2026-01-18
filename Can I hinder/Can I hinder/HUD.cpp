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
{
}

bool HUD::load()
{
	//loading textures for hud
    if (!m_chatBGTexture.loadFromFile("ASSETS/IMAGES/HUD/ChatBackground.png")) 
        return false;
    if (!m_hotBarTexture.loadFromFile("ASSETS/IMAGES/HUD/Hotbar.png")) 
        return false;
    if (!m_healthBarDecorTexture.loadFromFile("ASSETS/IMAGES/HUD/health_bar_decoration.png"))
        return false;
	if (!m_healthBarTexture.loadFromFile("ASSETS/IMAGES/HUD/health_bar.png")) 
		return false;
    if (!m_weaponBgTexture.loadFromFile("ASSETS/IMAGES/HUD/currentWeaponBackground.png")) 
        return false;
	if (!m_weaponBgTexture2.loadFromFile("ASSETS/IMAGES/HUD/currentWeapon.png"))
		return false;

	// Initialize sprites
    m_chatBGSprite = sf::Sprite(m_chatBGTexture);
    m_hotBarSprite = sf::Sprite(m_hotBarTexture);
    m_healthBarDecorSprite = sf::Sprite(m_healthBarDecorTexture);
	m_healthBarSprite = sf::Sprite(m_healthBarTexture);
    m_weaponBgSprite = sf::Sprite(m_weaponBgTexture);
	m_weaponBgSprite2 = sf::Sprite(m_weaponBgTexture2);

	// Scaling
	m_healthBarDecorSprite.setScale({ 6.f,6.f });
	m_chatBGSprite.setScale({ 8.f, 6.f });
	m_hotBarSprite.setScale({ 3.f, 3.f });
	m_weaponBgSprite.setScale({ 2.7f, 2.7f });
	m_weaponBgSprite2.setScale({ 2.7f, 2.7f });

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
	m_chatBGSprite.setPosition({ size.x - 420.f, 30.f });

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

	const int w = std::max(0, std::min(fullW, (int)std::round(fullW * m_hpShown)));

	sf::IntRect rect{
		sf::Vector2i{0, 0},
		sf::Vector2i{w, fullH}
	};

	m_healthBarSprite.setTextureRect(rect);
}

void HUD::draw(sf::RenderWindow& window)
{
	
	window.draw(m_healthBarDecorSprite);
	window.draw(m_healthBarSprite);
	window.draw(m_chatBGSprite);
	window.draw(m_hotBarSprite);
	window.draw(m_weaponBgSprite);
	window.draw(m_weaponBgSprite2);
}
