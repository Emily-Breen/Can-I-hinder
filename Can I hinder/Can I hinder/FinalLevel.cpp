#include "FinalLevel.h"


//this is for the super secret ending :D

FinalLevel::FinalLevel(InputHandler& input): m_input(input)
{
   if(TBFont.openFromFile("assets/Fonts/MagicalWorld.ttf"))
   {
       TBText.setFont(TBFont);
   }
  
   auto setupText = [this](sf::Text& text, const std::string& str)
       {
           text.setString(str);
           text.setCharacterSize(50);
           text.setFillColor(sf::Color::White);
       };

   setupText(m_attackText, "Attack");
   setupText(m_magicText, "Magic");
   setupText(m_itemText, "Item");

   setupText(m_hpText, "HP: 100");
   setupText(m_mpText, "MP: 50");

   spell = EffectType::Fireball;

   m_spellBannerText.setFont(TBFont);
   m_spellBannerText.setCharacterSize(60);
   m_spellBannerText.setFillColor(sf::Color::White);

   m_spellBannerBox.setFillColor(sf::Color(0, 0, 100));
   m_spellBannerBox.setOutlineColor(sf::Color::White);
   m_spellBannerBox.setOutlineThickness(5.f);

   dialogue();
}

void FinalLevel::start()
{
    m_playerHealth = 100;
    m_enemyHealth = 100;
    m_turnState = TurnState::PLAYER_CHOICE;
}

void FinalLevel::updateTB(float dt)
{
    m_hpText.setString("HP: " + std::to_string(m_playerHealth));
    m_mpText.setString("MP: " + std::to_string(m_playerMP));

    if (m_dialogueState == DialogueState::ACTIVE)
    {
        const std::string& fullLine = m_dialogueLines[m_currentLine];

        if (!m_lineFinished)
        {
            m_typeTimer += dt;

            while (m_typeTimer >= m_typeSpeed && m_charIndex < fullLine.size())
            {
                char c = fullLine[m_charIndex];
                m_visibleDialogueText += c;
                m_charIndex++;

                
                if (c == '.' || c == ',' || c == '!' || c == '?')
                    m_typeTimer -= m_typeSpeed * 4.f;
                else
                    m_typeTimer -= m_typeSpeed;
            }
            if (m_charIndex >= fullLine.size())
            {
                m_lineFinished = true;
            }
        }
        if (m_input.menuSelectPressed())
        {
            if (!m_lineFinished)
            {
                // skip typing
                m_visibleDialogueText = fullLine;
                m_lineFinished = true;
            }
            else
            {
                // next line
                m_currentLine++;

                if (m_currentLine >= m_dialogueLines.size())
                {
                    m_dialogueState = DialogueState::FINISHED;
                }
                else
                {
                    m_visibleDialogueText = "";
                    m_charIndex = 0;
                    m_typeTimer = 0.f;
                    m_lineFinished = false;
                }
            }
        }
        return;
    }
    for (auto& dmg : m_damageTexts)
    {
        dmg.damageText.move(dmg.velocity * dt);
        dmg.lifetime -= dt;
    }

    m_damageTexts.erase(
        std::remove_if(m_damageTexts.begin(), m_damageTexts.end(),
            [](const DamageText& d) 
            { return d.lifetime <= 0.f; }),
        m_damageTexts.end()
    );

    switch (m_turnState) {

    case TurnState::PLAYER_CHOICE:
    {
        if (m_inMagicMenu)
        {
            std::vector<MagicType> magicList = {
               MagicType::Heal,
               MagicType::Fireball,
               MagicType::Thunder,
               MagicType::Ice
            };
            int currentIndex = 0;

            // find current index
            for (int i = 0; i < magicList.size(); i++)
            {
                if (magicList[i] == m_selectedMagic)
                {
                    currentIndex = i;
                    break;
                }
            }

            if (m_input.menuUpPressed())
            {
                currentIndex = (currentIndex - 1 + magicList.size()) % magicList.size();
            }

            if (m_input.menuDownPressed())
            {
                currentIndex = (currentIndex + 1) % magicList.size();
            }

            m_selectedMagic = magicList[currentIndex];

            if (m_input.menuSelectPressed())
            {
                m_magicTriggered = true;
                m_inMagicMenu = false;
                m_turnState = TurnState::PLAYER_ACTION;
            }
            if (m_input.menuCancelPressed())
            {
                m_inMagicMenu = false;
                m_selectedMagic = MagicType::Fireball;
            }

            return;
        }
        if (m_inItemMenu)
        {
            const int itemCount = 3; 

            if (m_input.menuUpPressed())
                m_selectedItem = (m_selectedItem - 1 + itemCount) % itemCount;

            if (m_input.menuDownPressed())
                m_selectedItem = (m_selectedItem + 1) % itemCount;

            if (m_input.menuSelectPressed())
            {
                m_itemTriggered = true;
                m_inItemMenu = false;
                m_turnState = TurnState::PLAYER_ACTION;
            }

            if (m_input.menuCancelPressed())
            {
                m_inItemMenu = false;
            }

            return;
        }
        if (m_input.menuUpPressed())
            m_selectedOption = (m_selectedOption + 2) % 3;

        if (m_input.menuDownPressed())
            m_selectedOption = (m_selectedOption + 1) % 3;

        if (m_input.menuSelectPressed())
        {
            if (m_selectedOption == 0) // Attack
                m_turnState = TurnState::PLAYER_ACTION;

            else if (m_selectedOption == 1)
            {
                m_inMagicMenu = true;
            }
            else if (m_selectedOption == 2)
            {
                m_inItemMenu = true;
            }
        }
        break;
    }
    case TurnState::PLAYER_ACTION:
    {
        if (!m_actionStarted)
        {
            m_actionStarted = true;
            m_actionTimer = 0.f;
            if (m_itemTriggered)
            {
                if (m_selectedItem == 0) // HEALTH
                {
                    int healAmount = 30;

                    if (m_hudRef && m_hudRef->hasHealthPotion())
                    {
                        m_hudRef->useHealthPotion();
						m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Heal.ogg");
                        m_playerHealth += healAmount;
                        m_playerHealth = std::min(m_playerHealth, 100);

                        showSpellBanner("Potion!");

                        spawnDamageText(
                            healAmount,
                            m_playerRef->getPosition(),
                            sf::Color::Green
                        );
                    }
                    else
                    {
                        showSpellBanner("No Potions!");
                    }
                    if (m_playerHealth > 100)
                        m_playerHealth = 100;

                    showSpellBanner("Potion!");

                    spawnDamageText(
                        healAmount,
                        m_playerRef->getPosition(),
                        sf::Color::Green
                    );
                }
                else if (m_selectedItem == 1) //MP
                {
                    int mpAmount = 20;

                    if (m_hudRef && m_hudRef->hasSpeedPotion())
                    {
                        m_hudRef->useSpeedPotion();
                        m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Heal.ogg");
                        int mpAmount = 20;

                        m_playerMP += mpAmount;
                        m_playerMP = std::min(m_playerMP, 50);
                        auto tex = std::make_shared<sf::Texture>();

                        tex->loadFromFile("ASSETS/IMAGES/Effects/SpeedBoost.png");

                        sf::Vector2f spawnPos = m_playerRef->getPosition();
                        spawnPos.y -= 100.f;

                        auto effect = std::make_unique<Effects>(
                            tex,
                            spawnPos,
                            EnemyType::None,
                            EffectType::SpeedBoost
                        );

                        m_effects.emplace_back(std::move(effect));
                        showSpellBanner("Ether!");

                        spawnDamageText(
                            mpAmount,
                            m_playerRef->getPosition(),
                            sf::Color::Cyan
                        );
                    }
                    else
                    {
                        showSpellBanner("No Ether!");
                    }
                    if (m_playerMP > 50)
                        m_playerMP = 50;

                    showSpellBanner("Ether!");

                    spawnDamageText(
                        mpAmount,
                        m_playerRef->getPosition(),
                        sf::Color::Cyan
                    );
                }
				if (m_hudRef && m_hudRef->hasPowerPotion()) //DAMAGE BOOST
                {
                    m_hudRef->usePowerPotion();
                    m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Heal.ogg");
                    showSpellBanner("Power Boost!");
                    m_tempDamageBoost = true;
                    auto tex = std::make_shared<sf::Texture>();
                    tex->loadFromFile("ASSETS/IMAGES/Effects/PowerBoost.png");

                    sf::Vector2f spawnPos = m_playerRef->getPosition();
                    spawnPos.y -= 100.f;

                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::None,
                        EffectType::PowerBoost
                    );

                    m_effects.emplace_back(std::move(effect));

                }
                else
                {
                    showSpellBanner("No Power!");
                }
                m_itemTriggered = false;

                if (m_itemUsed)
                {
                    m_turnState = TurnState::ENEMY_ACTION;
                }
                return;
            }
            if (m_magicTriggered)
            {
                int mpCost = 0;

                switch (m_selectedMagic)
                {
                case MagicType::Fireball: mpCost = 10; break;
                case MagicType::Thunder:  mpCost = 10; break;
                case MagicType::Ice:      mpCost = 8;  break;
                case MagicType::Heal:     mpCost = 8; break;
                }
                if (m_playerMP < mpCost)
                {
                    showSpellBanner("Not enough MP!");
                    m_magicTriggered = false;
                    return;
                }

                m_playerMP -= mpCost;
                if (m_playerRef)
                {
                    m_playerRef->setDirection(Direction::LEFT);
                    m_playerRef->forceAttack();
                    m_playerRef->useAttack2(true);
                }
               
                if (m_selectedMagic == MagicType::Fireball)
                {
                    std::cout << "Player casts Fireball!\n";
                    showSpellBanner("Fireball");
					m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Fireball.ogg");
                    auto tex = std::make_shared<sf::Texture>();
                    if (!tex->loadFromFile("ASSETS/IMAGES/Effects/fireball.png"))
                    {
                        std::cout << "Failed to load fireball\n";
                    }

                    sf::Vector2f spawnPos = m_playerRef->getPosition();
                    spawnPos.y -= 80.f;
                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::Boss,
                        EffectType::Fireball
                    );
                    sf::Vector2f dir = m_bossRef->getPosition() - m_playerRef->getPosition();

                    float length = MathUtils::vectorLength(dir);
                    if (length != 0)
                        dir /= length;

                    effect->setEffectVelocity(dir * 600.f);
                    m_effects.emplace_back(std::move(effect));
                    
                }
                else if (m_selectedMagic ==MagicType::Thunder)
                {
                    showSpellBanner("Thunder");
					m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Thunder.ogg");
                    auto tex = std::make_shared<sf::Texture>();
                    tex->loadFromFile("ASSETS/IMAGES/Effects/thunder.png");

                    sf::Vector2f spawnPos = m_bossRef->getPosition();
                    spawnPos.y -= 100.f;

                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::Boss,
                        EffectType::Thunder
                    );

                
                    m_effects.emplace_back(std::move(effect));
                }
                else if (m_selectedMagic == MagicType::Ice) {
					showSpellBanner("Ice");
					m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Ice.ogg");
                    auto tex = std::make_shared<sf::Texture>();
                    tex->loadFromFile("ASSETS/IMAGES/Effects/Ice.png");

                    sf::Vector2f spawnPos = m_bossRef->getPosition();
                    spawnPos.y -=150.f;

                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::Boss,
                        EffectType::Ice
                    );
                    effect->setEffectScale({ 5.f,5.f });

                    m_effects.emplace_back(std::move(effect));

                }
                else if (m_selectedMagic == MagicType::Heal)
                {
                    showSpellBanner("Cure");
                    m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Heal.ogg");

                    int healAmount = 20;

                    m_playerHealth += healAmount;

                    
                    if (m_playerHealth > 100)
                        m_playerHealth = 100;

                    spawnDamageText(
                        healAmount,
                        m_playerRef->getPosition(),
                        sf::Color::Green
                    );

                    // visual effect (no damage)
                    auto tex = std::make_shared<sf::Texture>();
                    tex->loadFromFile("ASSETS/IMAGES/Effects/Healing.png");

                    sf::Vector2f spawnPos = m_playerRef->getPosition();
                    spawnPos.y -= 150.f;

                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::None,
                        EffectType::Healing
                    );

                    effect->setEffectColor(sf::Color(100, 255, 100, 200));

                    m_effects.emplace_back(std::move(effect));
                }

				m_magicTriggered = false; // reset mafic trigger
            }
            else
            {
                
                int damage = rand() % 13 + 4;
                if (m_tempDamageBoost)
                {
                    damage *= 2;
                    m_tempDamageBoost = false;
                }
                if (m_bossRef)
                    m_bossRef->takeDamage(damage);

                if (m_playerRef)
                {
                    m_playerRef->setDirection(Direction::LEFT);
                    m_playerRef->forceAttack();
                }

                spawnDamageText(
                    -damage,
                    m_bossRef->getPosition(),
                    sf::Color::Red
                );
                
            }
        }
        m_actionTimer += dt;
        std::vector<std::unique_ptr<Effects>> newEffects;

        for (auto& effect : m_effects)
        {
            effect->updateEffects(dt);

            if (effect->shouldSpawn() && !effect->hasSpawned())
            {
                effect->markAsSpawned();

                    EffectType type = effect->getEffectType();

                    bool isDamageEffect =
                        type == EffectType::Fireball ||
                        type == EffectType::Thunder ||
                        type == EffectType::Ice;

                    if (isDamageEffect)
                    {
                        int damage = rand() % 20 + 10;
                        m_bossRef->takeDamage(damage);

                        spawnDamageText(
                            -damage,
                            m_bossRef->getPosition(),
                            sf::Color::Red
                        );
                    }

                    sf::Vector2f spawnPos = m_bossRef->getPosition();
                    spawnPos.y -= 150.f;

                    if (effect->getEffectType() == EffectType::Fireball)
                    {
                        auto tex = std::make_shared<sf::Texture>();
                        tex->loadFromFile("ASSETS/IMAGES/Effects/FireHit.png");

                        auto hit = std::make_unique<Effects>(
                            tex, spawnPos, EnemyType::None, EffectType::FireHit);

                        hit->setEffectScale({ 4.f, 4.f }); 

                        newEffects.emplace_back(std::move(hit));
                    }
                    else if (effect->getEffectType() == EffectType::Thunder)
                    {
                        auto tex = std::make_shared<sf::Texture>();
                        tex->loadFromFile("ASSETS/IMAGES/Effects/ThunderHit.png");

                        auto hit = std::make_unique<Effects>(
                            tex, spawnPos, EnemyType::None, EffectType::ThunderHit);

                        hit->setEffectScale({ 6.f, 6.f }); 

                        newEffects.emplace_back(std::move(hit));
                    }
                    else if (effect->getEffectType() == EffectType::Ice)
                    {
                        auto tex = std::make_shared<sf::Texture>();
                        tex->loadFromFile("ASSETS/IMAGES/Effects/IceHit.png");

                        auto hit = std::make_unique<Effects>(
                            tex, spawnPos, EnemyType::None, EffectType::IceHit);

                        hit->setEffectScale({ 5.f, 5.f });

                        newEffects.emplace_back(std::move(hit));
                    }
               
            }
        }
        for (auto& e : newEffects)
        {
            m_effects.emplace_back(std::move(e));
        }
        
        bool allEffectsFinished = true;

        for (auto& effect : m_effects)
        {
            if (!effect->isFinished())
            {
                allEffectsFinished = false;
                break;
            }
        }
       
        if (m_actionTimer >= m_actionDuration && allEffectsFinished)
        {
            m_actionStarted = false;

            if (m_bossRef && m_bossRef->isDead())
                m_turnState = TurnState::WIN;
            else
                m_turnState = TurnState::ENEMY_ACTION;
        }
        // clean up finished effects
        m_effects.erase(
            std::remove_if(m_effects.begin(), m_effects.end(),
                [](const std::unique_ptr<Effects>& e)
                {
                    return e->isFinished();
                }),
            m_effects.end()
        );
        if (m_effects.empty() && m_actionTimer > 0.2f && m_turnState != TurnState::LOSE && m_turnState != TurnState::WIN)
        {
            m_showSpellBanner = false;
        }
        break;
    }
    case TurnState::ENEMY_ACTION:
    {
        if (!m_actionStarted)
        {
            m_actionStarted = true;
            m_actionTimer = 0.f;
            int action = rand() % 2; // 0 = attack, 1 = magic

            if (action == 0)
            {
                // NORMAL ATTACK
                m_enemyDoingMagic = false;

                if (m_bossRef)
                {
                    m_bossRef->setNPCDirection(Direction::RIGHT);
                    m_bossRef->startAttack();
                }
            }
            else
            {
                // MAGIC
                m_enemyDoingMagic = true;
                m_enemyMagicTriggered = true;

                int roll = rand() % 4;

                switch (roll)
                {
                case 0: m_enemySelectedMagic = MagicType::Fireball; break;
                case 1: m_enemySelectedMagic = MagicType::Thunder; break;
                case 2: m_enemySelectedMagic = MagicType::Gravity; break;
                case 3: m_enemySelectedMagic = MagicType::Ice; break;
                }
            }
        }

        m_actionTimer += dt;

        if (!m_enemyDoingMagic && m_actionTimer >= m_actionDuration)
        {
            int damage = rand() % 15 + 5;
            m_playerHealth -= damage;
            if (m_playerRef)
            {
                m_playerRef->takeDamage(0.4f);
            }
            spawnDamageText(
                -damage,
                m_playerRef->getPosition(),
                sf::Color::Red
            );
            if (m_playerHealth < 0)
            {
                if (m_playerRef)
                {
                   
                    m_playerRef->setDirection(Direction::LEFT); // face boss
                    m_playerRef->dead();
                    m_playerRef->setInputEnabled(false);
                    showSpellBanner("Game Over");
                }

                m_playerDying = true;
                m_deathTimer = 0.f;
                m_turnState = TurnState::LOSE;
               
            }
            else
                m_turnState = TurnState::PLAYER_CHOICE;

            m_actionStarted = false;
        }
        if (m_enemyDoingMagic)
        {
            if (m_enemyMagicTriggered)
            {
                if (m_bossRef)
                {
                    m_bossRef->setNPCDirection(Direction::RIGHT);
                    m_bossRef->useBossAttack2(true);
                    m_bossRef->startAttack();
                }
                if (m_enemySelectedMagic == MagicType::Fireball)
                {
                    showSpellBanner("Skeleton King casts: Fireball");
					m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Fireball.ogg");
                    auto tex = std::make_shared<sf::Texture>();
                    tex->loadFromFile("ASSETS/IMAGES/Effects/fireball.png");

                    sf::Vector2f spawnPos = m_bossRef->getPosition();
                    spawnPos.y -= 100.f;

                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::None,
                        EffectType::Fireball
                    );

                    sf::Vector2f dir = m_playerRef->getPosition() - m_bossRef->getPosition();
                    float len = MathUtils::vectorLength(dir);
                    if (len != 0) dir /= len;
                    effect->setEffectScale({ -4.f,4.f });
                    effect->setEffectVelocity(dir * 600.f);

                    m_effects.emplace_back(std::move(effect));
                }
				else if (m_enemySelectedMagic == MagicType::Thunder)
                {
                    showSpellBanner("Skeleton King casts: Thunder");
					m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Thunder.ogg");
                    auto tex = std::make_shared<sf::Texture>();
                    tex->loadFromFile("ASSETS/IMAGES/Effects/thunder.png");

                    sf::Vector2f spawnPos = m_playerRef->getPosition();
                    spawnPos.y -= 100.f;

                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::None,
                        EffectType::Thunder
                    );

                    m_effects.emplace_back(std::move(effect));
                }
                else if (m_enemySelectedMagic == MagicType::Gravity) {
                    showSpellBanner("Skeleton King casts: Gravity");
					m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Gravity.ogg");
                    auto tex = std::make_shared<sf::Texture>();
                    tex->loadFromFile("ASSETS/IMAGES/Effects/Gravity.png");

                    sf::Vector2f spawnPos = m_bossRef->getPosition();
                    spawnPos.y -= 100.f;

                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::None,
                        EffectType::Gravity
                    );

                    sf::Vector2f dir = m_playerRef->getPosition() - m_bossRef->getPosition();
                    float len = MathUtils::vectorLength(dir);
                    if (len != 0) dir /= len;
                    effect->setEffectScale({ -4.f,4.f });
                    effect->setEffectVelocity(dir * 600.f);

					m_effects.emplace_back(std::move(effect));
                }
                else if (m_enemySelectedMagic == MagicType::Ice)
                {
                    showSpellBanner("Skeleton King casts: Ice");
					m_audio.playSoundEffect("ASSETS/AUDIO/SFX/Ice.ogg");
                    auto tex = std::make_shared<sf::Texture>();
                    tex->loadFromFile("ASSETS/IMAGES/Effects/Ice.png");

                    sf::Vector2f spawnPos = m_playerRef->getPosition();
                    spawnPos.y -= 100.f;

                    auto effect = std::make_unique<Effects>(
                        tex,
                        spawnPos,
                        EnemyType::None,
                        EffectType::Ice
                    );

                    m_effects.emplace_back(std::move(effect));
                }

                m_enemyMagicTriggered = false;
            }
            std::vector<std::unique_ptr<Effects>> newEffects;

            for (auto& effect : m_effects)
            {
                effect->updateEffects(dt);

                if (effect->shouldSpawn() && !effect->hasSpawned())
                {
                    effect->markAsSpawned();

                    EffectType type = effect->getEffectType();

                    bool isDamageEffect =
                        type == EffectType::Fireball ||
                        type == EffectType::Thunder ||
                        type == EffectType::Ice ||
                        type == EffectType::Gravity;

                    if (isDamageEffect)
                    {
                        int damage = rand() % 18 + 8;
                        m_playerHealth -= damage;
                        if (m_playerRef)
                            m_playerRef->takeDamage(0.4f);
                        spawnDamageText(
                            -damage,
                            m_playerRef->getPosition(),
                            sf::Color::Red
                        );
                    }

                    

                    sf::Vector2f spawnPos = m_playerRef->getPosition();
                    spawnPos.y -= 100.f;

                    if (effect->getEffectType() == EffectType::Fireball)
                    {
                        auto tex = std::make_shared<sf::Texture>();
                        tex->loadFromFile("ASSETS/IMAGES/Effects/FireHit.png");

                        newEffects.emplace_back(std::make_unique<Effects>(
                            tex, spawnPos, EnemyType::None, EffectType::FireHit));
                    }
                    else if (effect->getEffectType() == EffectType::Thunder)
                    {
                        auto tex = std::make_shared<sf::Texture>();
                        tex->loadFromFile("ASSETS/IMAGES/Effects/ThunderHit.png");

                        newEffects.emplace_back(std::make_unique<Effects>(
                            tex, spawnPos, EnemyType::None, EffectType::ThunderHit));
                    }
                    else if (effect->getEffectType() == EffectType::Gravity)
                    {
                        auto tex = std::make_shared<sf::Texture>();
                        tex->loadFromFile("ASSETS/IMAGES/Effects/GravityHit.png");

                        newEffects.emplace_back(std::make_unique<Effects>(
                            tex, spawnPos, EnemyType::None, EffectType::GravityHit));
                    }
                    else if (effect->getEffectType() == EffectType::Ice)
                    {
                        auto tex = std::make_shared<sf::Texture>();
                        tex->loadFromFile("ASSETS/IMAGES/Effects/IceHit.png");

                        newEffects.emplace_back(std::make_unique<Effects>(
                            tex, spawnPos, EnemyType::None, EffectType::IceHit));
                    }
                }
            }
            for (auto& e : newEffects) {
                m_effects.emplace_back(std::move(e));
            }
            if (m_playerHealth < 0)
            {
                if (m_playerRef)
                {
                    m_playerRef->dead();
                    m_playerRef->setInputEnabled(false);
                    showSpellBanner("Game Over");
                }

                m_turnState = TurnState::LOSE;
                return;
            }
            bool allFinished = true;
            for (auto& effect : m_effects)
            {
                if (!effect->isFinished())
                {
                    allFinished = false;
                    break;
                }
            }

            if (m_actionTimer >= m_actionDuration && allFinished)
            {
                m_actionStarted = false;
                if (m_bossRef)
                {
                    m_bossRef->useBossAttack2(false);
                }
                if (m_playerHealth < 0)
                    m_turnState = TurnState::LOSE;
                else
                    m_turnState = TurnState::PLAYER_CHOICE;
            }
        }
        m_effects.erase(
            std::remove_if(m_effects.begin(), m_effects.end(),
                [](const std::unique_ptr<Effects>& e)
                {
                    return e->isFinished();
                }),
            m_effects.end()
        );
        if (m_effects.empty() && m_actionTimer > 0.2f)
        {
            m_showSpellBanner = false;
        }
        break;
    }
    case TurnState::WIN:
    {
        if (m_bossRef && m_bossRef->isDead())
        {
            // wait until animation finishes
            
        }
        if (!m_winHandled)
        {
            showSpellBanner("YOU WIN");

            if (m_playerRef)
            {
                m_playerRef->setInputEnabled(false);
                m_playerRef->setDirection(Direction::LEFT);
                m_playerRef->setVictoryPose(true);
            }

            m_winHandled = true;
            m_winTimer = 0.f;
        }

        m_winTimer += dt;

        // start fade
        if (m_winTimer >= m_fadeDelay)
        {
            m_fadingOut = true;
        }

        // fade effect
        if (m_fadingOut)
        {
            m_fadeAlpha += m_fadeSpeed * dt;
            if (m_fadeAlpha > 255.f)
                m_fadeAlpha = 255.f;
        }
        break;
    }
    case TurnState::LOSE:
    {
        m_deathTimer += dt;

        // fade to black
        if (m_deathTimer >= m_fadeDelay)
        {
            m_fadingOut = true;
        }

       
        if (m_fadingOut)
        {
            m_fadeAlpha += m_fadeSpeed * dt;
            if (m_fadeAlpha > 255.f)
                m_fadeAlpha = 255.f;
        }

        if (m_deathTimer >= m_deathDelay && !m_loseHandled)
        {
            m_loseHandled = true;
            std::cout << "YOU LOSE\n";
        }

        break;
    }
    default:
        break;
    }
}

void FinalLevel::drawTB(sf::RenderWindow& window, sf::Vector2f screenSize)
{
    float width = screenSize.x;
    float height = screenSize.y;

    // for dialogue
    if (m_dialogueState == DialogueState::ACTIVE)
    {
        if (!m_dialogueSprite) return;

        auto& sprite = *m_dialogueSprite;

        auto texSize = sprite.getTexture().getSize();

        float scaleX = width / texSize.x;
        float scaleY = 250.f / texSize.y;

        sprite.setScale({ scaleX, scaleY });
        sprite.setPosition({ 0.f, height - 250.f });

        window.draw(sprite);

        if (m_currentLine < m_dialogueLines.size())
        {
            TBText.setString(m_visibleDialogueText);
            TBText.setCharacterSize(40);
            TBText.setFillColor(sf::Color::White);
            TBText.setPosition({ 400.f, height - 150.f });

            window.draw(TBText);
        }
        
     
        return; 
    }

    //ui in game
    sf::RectangleShape uiBox({ width, 400.f });
    uiBox.setFillColor(sf::Color(0, 0, 100));
    uiBox.setOutlineColor(sf::Color::White);
    uiBox.setOutlineThickness(5.f);
    uiBox.setPosition({ 0.f, height - 250.f });

    window.draw(uiBox);
    for (auto& effect : m_effects)
    {
        effect->drawEffects(window);
    }
    float baseY = height - 200.f;

    // options
    m_attackText.setPosition({ 200.f, baseY });
    m_magicText.setPosition({ 200.f, baseY +50.f });
    m_itemText.setPosition({ 200.f, baseY +100.f });

    m_attackText.setFillColor(m_selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
    m_magicText.setFillColor(m_selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
    m_itemText.setFillColor(m_selectedOption == 2 ? sf::Color::Yellow : sf::Color::White);
    
    m_hpText.setPosition({ 1700.f, baseY });
    m_mpText.setPosition({ 1700.f, baseY + 50.f });

    for (auto& dmg : m_damageTexts)
    {
        window.draw(dmg.damageText);
    }
    window.draw(m_attackText);
    window.draw(m_magicText);
    window.draw(m_itemText);

    window.draw(m_hpText);
    window.draw(m_mpText);

    if (m_inMagicMenu) {
        sf::RectangleShape magicBox;

        float boxWidth = 300.f;
        float boxHeight = 240.f;

        magicBox.setSize({ boxWidth, boxHeight });
        magicBox.setFillColor(sf::Color(0, 0, 100)); 
        magicBox.setOutlineColor(sf::Color::White);
        magicBox.setOutlineThickness(5.f);

        magicBox.setPosition({ 470.f, baseY -60 });

        window.draw(magicBox);

		sf::Text healText = m_attackText;
		healText.setString("Cure");

        sf::Text fireText = m_attackText;
        fireText.setString("Fireball");

        sf::Text thunderText = m_attackText;
        thunderText.setString("Thunder");

		sf::Text iceText = m_attackText;
		iceText.setString("Ice");

		healText.setPosition({ 500.f, baseY-50 });
        fireText.setPosition({ 500.f, baseY });
        thunderText.setPosition({ 500.f, baseY+50 });
		iceText.setPosition({ 500.f, baseY + 100 });

        healText.setFillColor(
            m_selectedMagic == MagicType::Heal ? sf::Color::Yellow : sf::Color::White
		);
        fireText.setFillColor(
            m_selectedMagic == MagicType::Fireball ? sf::Color::Yellow : sf::Color::White
        );

        thunderText.setFillColor(
            m_selectedMagic == MagicType::Thunder ? sf::Color::Yellow : sf::Color::White
        );
		iceText.setFillColor(
			m_selectedMagic == MagicType::Ice ? sf::Color::Yellow : sf::Color::White
		);
		window.draw(healText);
        window.draw(fireText);
        window.draw(thunderText);
		window.draw(iceText);
    }
    if (m_inItemMenu)
    {
        sf::Text healthText = m_attackText;
        sf::Text mpText = m_attackText;
        sf::Text powerText = m_attackText;
		//displays the amount of potions the player has from the overworld
        healthText.setString("Potion x" + std::to_string(m_hudRef->getHealthPotionCount()));
        mpText.setString("Ether x" + std::to_string(m_hudRef->getSpeedPotionCount()));
        powerText.setString("Power x" + std::to_string(m_hudRef->getPowerPotionCount()));

        healthText.setPosition({ 500.f, baseY });
        mpText.setPosition({ 500.f, baseY + 50 });
        powerText.setPosition({ 500.f, baseY + 100 });

        healthText.setFillColor(m_selectedItem == 0 ? sf::Color::Yellow : sf::Color::White);
        mpText.setFillColor(m_selectedItem == 1 ? sf::Color::Yellow : sf::Color::White);
        powerText.setFillColor(m_selectedItem == 2 ? sf::Color::Yellow : sf::Color::White);

        window.draw(healthText);
        window.draw(mpText);
        window.draw(powerText);
    }
    if (m_showSpellBanner)
    {
        
        m_spellBannerBox.setSize({ width, 120.f });
        m_spellBannerBox.setPosition({ 0.f, 0.f });

        window.draw(m_spellBannerBox);

       

        m_spellBannerText.setPosition({
            width * 0.5f,
            60.f
            });

        window.draw(m_spellBannerText);

    }
    if (m_fadingOut)
    {
        sf::RectangleShape fadeRect;
        fadeRect.setSize(screenSize);
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(m_fadeAlpha)));

        window.draw(fadeRect);
    }
}

void FinalLevel::startDialogue()
{
    m_dialogueState = DialogueState::ACTIVE;
    m_currentLine = 0;

    m_visibleDialogueText = "";
    m_charIndex = 0;
    m_typeTimer = 0.f;
    m_lineFinished = false;
}

bool FinalLevel::isDialogueFinished() const
{
	return m_dialogueState == DialogueState::FINISHED;
}

bool FinalLevel::hasPlayerLost() const
{
    return m_turnState == TurnState::LOSE && m_loseHandled;
}

bool FinalLevel::hasPlayerWon() const
{
    return m_turnState == TurnState::WIN
        && m_winHandled
        && m_winTimer >= m_winDelay;
}

void FinalLevel::dialogue()
{
    m_currentLine = 0;

    m_dialogueLines = {
        "So... you finally made it.",
        "I was wondering how long you'd survive.",
        "In fact you were never meant to reach this point.",
        "Throughout all of this your friends didnt really try to help you did they?",
		"They just wanted to see you suffer.",
		"But you know what? I admire your persistence.",
		"Most people would have given up by now.",
		"Maybe you deserve to win after all.",
		"But before you can truly be free, you have to get through me.",
		"Let's see how long you can last against me!"
    };
}

void FinalLevel::trigger()
{
}

void FinalLevel::setPlayer(Player* player)
{
	m_playerRef = player;
}

void FinalLevel::setHUD(HUD* hud)
{
    m_hudRef = hud;
}

void FinalLevel::setBoss(NPC* boss)
{
    m_bossRef = boss;
}

void FinalLevel::spawnDamageText(int amount, sf::Vector2f position, sf::Color color)
{
    DamageText dmg{ TBFont };

    dmg.damageText.setFont(TBFont);

   
    if (amount > 0)
        dmg.damageText.setString("+" + std::to_string(amount));
    else
        dmg.damageText.setString(std::to_string(amount));

    dmg.damageText.setCharacterSize(50);
    dmg.damageText.setFillColor(color);

    dmg.damageText.setPosition(position);

    dmg.velocity = { 0.f, -50.f };
    dmg.lifetime = 1.0f;

    m_damageTexts.push_back(dmg);
}

void FinalLevel::setDialogueTexture(const sf::Texture& texture)
{
    m_dialogueSprite.emplace(texture);
}

void FinalLevel::showSpellBanner(const std::string& text)
{
    m_spellBannerText.setString(text);
	m_showSpellBanner = true;   

    auto bounds = m_spellBannerText.getLocalBounds();

    m_spellBannerText.setOrigin({
        bounds.position.x + bounds.size.x * 0.5f,
        bounds.position.y + bounds.size.y * 0.5f
        });
}
