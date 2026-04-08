#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include "NPC.h"
#include "Player.h"
#include "AnimationHandler.h"
#include "Input Handler.h"
#include "Effects.h"	
#include "Audio.h"
#include <vector>
#include <optional>
#include "MathUtils.h"
enum class DialogueState
{
	NONE,
	ACTIVE,
	FINISHED
};
enum class MagicType
{
	Fireball,
	Thunder,
	Heal,
	Ice,
	Gravity
};
struct DamageText {

	sf::Text damageText;
	sf::Vector2f velocity;
	float lifetime;
};
class FinalLevel
{
public:
	FinalLevel(InputHandler& input);
	void start();
	void updateTB(float dt);
	void drawTB(sf::RenderWindow& window, sf::Vector2f screenSize);
	void startDialogue();
	bool isDialogueFinished() const;
	bool hasPlayerLost() const;
	bool hasPlayerWon() const;
	void dialogue();
	void trigger();
	void setPlayer(Player* player);
	void setBoss(NPC* boss);
	void spawnDamageText(int amount, sf::Vector2f position, sf::Color color);
	void setDialogueTexture(const sf::Texture& texture);
	void showSpellBanner(const std::string& text);

private:

	enum class TurnState {
		PLAYER_CHOICE,
		PLAYER_ACTION,
		ENEMY_ACTION,
		WIN,
		LOSE
	};
	DialogueState m_dialogueState = DialogueState::NONE;
	TurnState m_turnState{ TurnState::PLAYER_CHOICE };
	InputHandler& m_input;
	Player* m_playerRef{ nullptr };
	NPC* m_bossRef = nullptr;
	EffectType spell;
	MagicType m_selectedMagic = MagicType::Fireball;
	MagicType m_enemySelectedMagic = MagicType::Fireball;
	Audio m_audio;
	sf::RectangleShape m_spellBannerBox;
    std::vector<std::string> m_dialogueLines;
	std::vector<DamageText> m_damageTexts;
	std::vector<std::unique_ptr<Effects>> m_effects;
	std::optional<sf::Sprite> m_dialogueSprite;
	std::string m_visibleDialogueText;
	float m_typeTimer = 0.f;
	float m_typeSpeed = 0.05f;
	size_t m_charIndex = 0;
	bool m_lineFinished = false;
	float m_actionTimer = 0.f;
	bool m_actionStarted = false;
	bool m_magicTriggered = false;
	const float m_actionDuration = 0.6f;
	float m_deathDelay = 2.0f;
	float m_deathTimer = 0.f;
	float m_fadeAlpha = 0.f;
	float m_fadeSpeed = 200.f;
	bool m_fadingOut = false;
	float m_fadeDelay = 0.6f;
	bool m_playerDying = false;
	bool m_loseHandled = false;
	bool m_winHandled = false;
	float m_winTimer = 0.f;
	float m_winDelay = 3.f;
	bool m_inMagicMenu = false;
	bool m_showSpellBanner = false;
	float m_spellDuration = 1.2f;
	int m_currentLine = 0;
	int m_playerHealth{ 100 };
	int m_playerMP{ 78 };
	int m_enemyHealth{ 100 };
	int m_selectedOption = 0;
	sf::Font TBFont;
	sf::Text TBText{ TBFont };

	sf::Text m_attackText{ TBFont };
	sf::Text m_magicText{ TBFont };
	sf::Text m_itemText{ TBFont };
	sf::Text m_hpText{ TBFont };
	sf::Text m_mpText{ TBFont };
	sf::Text m_spellBannerText{ TBFont };

	bool m_inDialogue = true;
	bool m_enemyDoingMagic = false;
	bool m_enemyMagicTriggered = false;
};

