
#include "Game.h"

#include <iostream>



auto desktopMode = sf::VideoMode::getDesktopMode(); 
Game::Game() :
	m_window{ desktopMode,"SFML Game 3.0",sf::State::Fullscreen },
	m_DELETEexitGame{ false }, m_camera(VIRTUAL_WIDTH,VIRTUAL_HEIGHT) //when true game will exit
{
	m_camera.onResize(m_window.getSize());
	m_window.setVerticalSyncEnabled(true);
	m_client.setOnMessage([this](const std::string& user,const std::string& action, const std::string& effect)
		{
			//std::string user = "User123"; //just for testing should if all going right be set from the PWA to display the username of the user sending the action

			if (action == "hinder" && effect == "spawn_enemy")
			{
				spawnEnemy = true;
				m_hud.pushChatMessage(user, "sent an enemy!", sf::Color(255, 80, 80));
			}
			else if (action == "hinder" && effect == "steal_power")
			{
				weakenPlayer = true;
				m_hud.pushChatMessage(user, "has weakened you!", sf::Color(255, 80, 80));
			}
			else if (action == "hinder" && effect == "drop_trap")
			{
				m_hud.pushChatMessage(user, "has blocked your path!", sf::Color(80, 255, 80));
			}
			else if (action == "hinder" && effect == "slow_player")
			{
				m_hud.pushChatMessage(user, "Has slowed your movements!", sf::Color(80, 255, 80));
			}
			else if (action == "help" && effect == "heal_player")
			{
				healPlayer = true;
				m_hud.pushChatMessage(user, "healed you!", sf::Color(80, 255, 80));
			}
		});
	
	if (USE_LOCAL_WS) {
		m_client.connect("localhost", "8080", false); //localhost testing
	}
	else {
		m_client.connect(
			"can-i-hinder-ws-fkc5d6hwgdg0f0fp.germanywestcentral-01.azurewebsites.net",
			"443",
			true //Azure tls 
		);
	}

	m_mapRenderer.load("ASSETS/LEVELS/Map2.tmx");
	std::cout << "player posX: " << m_player.getPosition().x << "player posY: " << m_player.getPosition().y;
	std::cout << "Doors loaded: " << m_mapRenderer.getDoors().size() << "\n";
	if (!m_mainMenuTexture.loadFromFile("ASSETS/LEVELS/PNG/MainMenu.png"))
	{
		std::cout << "Failed to load Main Menu PNG\n";
	}
	else
	{
		m_mainMenuSprite.setTexture(m_mainMenuTexture, true); // reset texture rect
		m_mainMenuSprite.setPosition({ 0.f, 0.f });

		float scaleX = VIRTUAL_WIDTH / m_mainMenuTexture.getSize().x;
		float scaleY = VIRTUAL_HEIGHT / m_mainMenuTexture.getSize().y;

		m_mainMenuSprite.setScale({ scaleX, scaleY });
	}
	if (!m_MagicalWorldFont.openFromFile("ASSETS/FONTS/MagicalWorld.ttf"))
	{
		std::cout << "Failed to load MagicalWorld font\n";
	}
	
	if (!m_buttonTexture.loadFromFile("ASSETS/LEVELS/PNG/Button.png"))
	{
		std::cout << "Failed to load Button.png\n";
	}
	setupMenuView();
	setupMainMenuButtons();
	setupMainMenuTitle();
	if (!m_hud.load())
	{
		std::cout << "Failed to load HUD\n";
	}
	else
	{
		m_hud.layout({
	static_cast<unsigned>(VIRTUAL_WIDTH),
	static_cast<unsigned>(VIRTUAL_HEIGHT)
			}); // makes sure the hud elements are in the right place when loaded.
	}
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	;

}


Game::~Game()
{
	m_client.close();

}



void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
		if (spawnEnemy)
		{
			std::cout << "An enemy has spawned to hinder you!\n";
			spawnEnemy = false;

			sf::Vector2f enemySize = { 48.f, 48.f }; //match eney size
			sf::Vector2f spawnPos = m_mapRenderer.getFloorSpawn(enemySize, m_player.getPosition(), 200.f); //200 is a nice sweet spot

			EnemyType type =
				(std::rand() % 2 == 0)
				? EnemyType::Skeleton
				: EnemyType::Goblin;

			spawnNPC(spawnPos, type);
		}
		if (healPlayer)
		{
			std::cout << "Healed by viewer!\n";
			healPlayer = false;

			m_testHealth += 0.25f;
			if (m_testHealth > 1.f)
				m_testHealth = 1.f;
		}
		if (weakenPlayer) {
			std::cout << "power reduced by viewer!\n";
			weakenPlayer = false;
			m_playerDamageMultiplier = 0.2f; // 80% damage reduction from the player to enemies for 6 seconds
			m_stealPowerDuration = 6.0f;
			m_stealPowerClock.restart();
			m_stealPowerActive = true;
		}

	}
}

void Game::processEvents()
{
	
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) // close window message 
		{
			m_DELETEexitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (const auto* resize = newEvent->getIf<sf::Event::Resized>())
		{
			m_camera.onResize({ resize->size.x, resize->size.y });
			m_hud.layout({
	                     static_cast<unsigned>(VIRTUAL_WIDTH),
	                     static_cast<unsigned>(VIRTUAL_HEIGHT)
		    }); //resizing to any screen (for mostly the 4k ones as my laptop is 1920x1080
		}
		if (m_currentMenuState == menuState::MAIN_MENU)
		{
			m_inputHandler.handleEvent(*newEvent, m_window, m_menuView);

			if (m_inputHandler.menuMouseMoved())
			{
				const sf::Vector2f mousePos = m_inputHandler.menuMousePosition();

				for (auto& button : m_menuButtons)
				{
					const bool isHovering = button.bounds().contains(mousePos);

					if (isHovering != button.hovered)
					{
						button.hovered = isHovering;

						if (isHovering)
						{
							button.sprite.setScale({ 0.65f, 0.65f });
							button.text.setFillColor(sf::Color::Yellow);
						}
						else
						{
							button.sprite.setScale({ 0.6f, 0.6f });
							button.text.setFillColor(sf::Color::White);
						}
					}
				}
			}
			if (m_inputHandler.menuMouseClickReleased())
			{
				const sf::Vector2f mousePos = m_inputHandler.menuMousePosition();

				for (auto& button : m_menuButtons)
				{
					if (button.bounds().contains(mousePos))
					{
						if (button.onClick)
							button.onClick();
						break;
					}
				}
			}
		}
		
	}
}



void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		if (m_currentMenuState == menuState::GAMEPLAY) {
			m_DELETEexitGame = true;
		}	
		else if (m_currentMenuState == menuState::PAUSE) {

		}
		else if (m_currentMenuState == menuState::MAIN_MENU) {
			m_DELETEexitGame = true;
		}
		else
			m_currentMenuState = menuState::MAIN_MENU;

		return;
	}
	if (m_currentMenuState == menuState::MAIN_MENU)
	{
		if (newKeypress->code == sf::Keyboard::Key::Enter)
		{
			m_currentMenuState = menuState::GAMEPLAY;
		}
		return;
	}
	//FUTURE EMILY REMEMBER TO REMOVE THESE TEST KEYS LATER!!!

	if (sf::Keyboard::Key::Numpad1 == newKeypress->code || sf::Keyboard::Key::Num1 == newKeypress->code) //just for testing so I dont have to set up the PWA aswell will be removed later
	{
		spawnEnemy = true;
	}
	if (sf::Keyboard::Key::Numpad2 == newKeypress->code || sf::Keyboard::Key::Num2 == newKeypress->code) //just for testing to see if the health is being updated will be removed later
	{
		m_testHealth -= 0.1f;
		if (m_testHealth < 0.f) 
			m_testHealth = 0.f;

		
	}
	if (sf::Keyboard::Key::Numpad3 == newKeypress->code || sf::Keyboard::Key::Num3 == newKeypress->code) //testing healing player will be removed later
	{
		m_testHealth += 0.1f;
		if (m_testHealth > 1.f) 
			m_testHealth = 1.f;
	}
	if (sf::Keyboard::Key::Numpad4 == newKeypress->code || sf::Keyboard::Key::Num4 == newKeypress->code)
	{
		m_hud.pushChatMessage(
			"TestUser",
			"made you weaker!",
			sf::Color(255, 80, 80),     
			sf::Color::White,           
			6.0f                        
		);
	}
	if (sf::Keyboard::Key::Numpad5 == newKeypress->code || sf::Keyboard::Key::Num5 == newKeypress->code)
	{
		m_hud.pushChatMessage(
			"Didde57",
			"healed you!",
			sf::Color(80, 255, 80),
			sf::Color::White,
			6.0f
		);
	}
	if (sf::Keyboard::Key::Numpad6 == newKeypress->code || sf::Keyboard::Key::Num6 == newKeypress->code)
	{
		m_items.emplace_back(
			ItemEffect{ ItemType::healthPotion, 0.25f, 0.f, 1 },
			sf::Vector2f(1500.f, 1500.f)
		);
	}

}





void Game::update(sf::Time t_deltaTime)
{
		if (m_DELETEexitGame)
		{
			m_window.close();
			return;
		}
		m_inputHandler.update();
		//handle menu state changes sound only right now
		if (m_currentMenuState != m_prevState)
		{
			m_audio.stopBackgroundMusic();

			switch (m_currentMenuState)
			{
			case menuState::MAIN_MENU:

				m_audio.playMenuBackgroundMusic("ASSETS/AUDIO/BACKGROUND MUSIC/Main Menu music.ogg");


				break;

			case menuState::GAMEPLAY:
				m_audio.stopMenuBackgroundMusic("ASSETS/AUDIO/BACKGROUND MUSIC/Main Menu music.ogg");
				m_audio.playInGameBackgroundMusic("ASSETS/AUDIO/BACKGROUND MUSIC/Main game music.ogg");
				break;

			case menuState::GAME_OVER:
			case menuState::PAUSE:
				
			case menuState::SETTINGS:
			default:
				break;
			}

			m_prevState = m_currentMenuState;
		}
		switch (m_currentMenuState)
		{
		case menuState::MAIN_MENU: {
			if (m_inputHandler.menuUpPressed())
				moveMenuSelection(-1);

			if (m_inputHandler.menuDownPressed())
				moveMenuSelection(+1);

			if (m_inputHandler.menuSelectPressed())
				activateSelectedButton();

			float menuTime = m_menuClock.getElapsedTime().asSeconds();
			float glow = 200.f + std::sin(menuTime * 4.f) * 55.f;
			float scale = 1.f + std::sin(menuTime * 2.f) * 0.05f;

			m_mainMenuTitleText.setScale({ scale, scale });
			m_mainMenuTitleHighlight.setScale({ scale, scale });
			m_mainMenuTitleShadow.setScale({ scale, scale });
			m_mainMenuTitleText.setFillColor(sf::Color(255, glow, 0));

			break;
		}

		case menuState::SETTINGS:
			
			return;

		case menuState::PAUSE:
			
			return;

		case menuState::GAME_OVER:
		
			return;

		case menuState::GAMEPLAY:
			break; 
		}
		if (m_DELETEexitGame)
		{
			m_window.close();
			return;
		}
		// steal power timer
		if (m_stealPowerActive)
		{
			if (m_stealPowerClock.getElapsedTime().asSeconds() >= m_stealPowerDuration)
			{
				m_stealPowerActive = false;
				m_playerDamageMultiplier = 1.0f;
			}
		}
		
		static bool attackWasHeld = false;
		const bool spaceHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
		const bool spaceJustPressed = spaceHeld && !attackWasHeld;
		attackWasHeld = spaceHeld;

		const bool controllerJustPressed = m_inputHandler.controllerAttackJustPressed();

		const bool attackJustPressed = spaceJustPressed || controllerJustPressed;
		
		m_player.update(t_deltaTime.asSeconds());

		//tracks direction of player movement
		sf::Vector2f direction = m_player.getMovement(); 
		
		float speed = 300.f; 
		sf::Vector2f movement = direction * speed * t_deltaTime.asSeconds();
		bool blocked = false;
		//for collision with walls 
		if (movement != sf::Vector2f(0.f, 0.f))
		{
			//tracks movement of player
			sf::FloatRect nextBounds = m_player.getBounds();
			nextBounds.position += movement;

			for (const auto& doorRef : m_mapRenderer.getDoors())
			{
				if (Entity::rectsIntersect(nextBounds, doorRef.rect))
				{
					if (m_keyCount >= doorRef.requiredKeys)
					{
						std::cout << "Door spawn read: " << doorRef.spawn.x << ", " << doorRef.spawn.y << "\n";
						m_mapRenderer.load(doorRef.nextMap);

						
						m_player.setPosition(doorRef.spawn);
						m_camera.follow(m_player.getPosition());
						return;
					}
					else
					{
						//door acts locked 
						blocked = true;
					}
					
					break;
				}
			}
			if (!blocked)
			{
				//this is for tiles 
				const auto& walls = m_mapRenderer.getCollisionRects();
				for (const auto& wall : walls)
				{
					if (Entity::rectsIntersect(nextBounds, wall))
					{
						blocked = true;
						break;
					}
				}
			}
			//only move if there is no collision
			if (!blocked)
			{
				m_player.movement(movement);
			}
			
		}
		//walking sound effect for player
		
		//treat very small movement as zero
		const float epsilon = 0.1f;
		//bool if player is moving a noticable amount and not blocked
		const bool shouldFootstep =
			(std::abs(movement.x) > epsilon || std::abs(movement.y) > epsilon) && !blocked;

		static bool footstepsOn = false;

		if (shouldFootstep && !footstepsOn)
		{
			m_audio.startWalkingSound();
			footstepsOn = true;
		}
		else if (!shouldFootstep && footstepsOn)
		{
			m_audio.stopWalkingSound();
			footstepsOn = false;
		}
		
		static sf::Vector2f lastPlayerPos = m_player.getPosition();
		sf::Vector2f playerPos = m_player.getPosition();
		sf::Vector2f playerVel = (playerPos - lastPlayerPos) / t_deltaTime.asSeconds();
		lastPlayerPos = playerPos;
		if (!m_player.isDead()&& attackJustPressed)
		{
			const sf::FloatRect atk = m_player.getAttackBounds();
			m_audio.startswordSlashSound();

			const float baseDamage = 0.25f;
			const float finalDamage = baseDamage * m_playerDamageMultiplier;
			for (auto& npc : m_npcs)
			{
				if (npc.isDead()) 
					continue;
				if (Entity::rectsIntersect(atk, npc.getBounds()))
					npc.takeDamage(finalDamage);
			}
		}

		bool anyNpcMoving = false;
		const float npcEpsilon = 0.1f;

		for (auto& npc : m_npcs)
		{
			const float dt = t_deltaTime.asSeconds();

			if (npc.isDead())
			{
				npc.setAttacking(false);
				npc.setVelocity({ 0.f, 0.f });
				npc.update(dt);    
				if (!npc.hasDroppedLoot())
				{
					npc.markDroppedLoot();
					m_items.emplace_back(ItemEffect{ ItemType::healthPotion, 0.25f }, npc.getPosition());
				}
				continue;           
			}

			sf::Vector2f npcPos = npc.getPosition();
			sf::Vector2f toPlayer = playerPos - npcPos;

			float distance = MathUtils::vectorLength(toPlayer);

			const float attackRange = npc.getAttackRange();
			sf::Vector2f delta{ 0.f, 0.f };

			if (distance <= attackRange)
			{
				npc.setVelocity({ 0.f, 0.f });
				npc.setAttacking(true);
				npc.facing(playerPos);

				if (npc.attackTimer(dt))
				{
					m_testHealth -= 0.1f;
					if (m_testHealth < 0.f) {
						m_testHealth = 0.f;
						m_player.dead();
					}

					m_player.takeDamage(0.4f);
				}
			}
			else
			{
				npc.setAttacking(false);

				//this was causing a "sticky" situation with the NPC's lol 
				//double movement was applied and duplicate collision Npcs still not fully navigating collisions 
				//but much better 
				sf::Vector2f sep = npc.computeSeparation(m_npcs, 10.f);
				const auto& walls = m_mapRenderer.getCollisionRects();

				sf::Vector2f intended = npc.computeAIMovement(playerPos, playerVel, dt, sep, walls);

				sf::Vector2f before = npc.getPosition();
				npc.moveWithCollision(intended, walls);
				sf::Vector2f after = npc.getPosition();

				delta = after - before; //track how much npc moved this frame
				
			}
			//bool if npc is moving a noticable amout this frame
			if (std::abs(delta.x) > npcEpsilon || std::abs(delta.y) > npcEpsilon)
				anyNpcMoving = true;

			npc.update(dt);
		}
		static bool npcStepsOn = false;

		if (anyNpcMoving && !npcStepsOn)
		{
			for (auto& npc : m_npcs) {
				if (npc.getType() == EnemyType::Goblin) {
					m_audio.startWalkingSound();
					npcStepsOn = true;
				}
				else if (npc.getType() == EnemyType::Skeleton) {
					m_audio.startNpcWalkingSound();
					npcStepsOn = true;
				}
			}
		}
		else if (!anyNpcMoving && npcStepsOn)
		{
			m_audio.stopNpcWalkingSound();
			npcStepsOn = false;
		}

		for (auto& item : m_items)
		{
			if (item.isCollected())
				continue;

			
			if (Entity::rectsIntersect(m_player.getBounds(), item.getBounds()))
			{
				item.collect();

				const ItemEffect& effect = item.getEffect();

				if (effect.type == ItemType::healthPotion)
				{
					m_testHealth += effect.amount;
					if (m_testHealth > 1.f)
						m_testHealth = 1.f;
				}
			}
		}

		for (auto& item : m_items)
		{
			item.update(t_deltaTime.asSeconds());
		}
		m_hud.update(m_testHealth,t_deltaTime.asSeconds());
		//Camera follows player
		m_camera.follow(m_player.getPosition());

}


void Game::render()
{
	
	m_window.clear(sf::Color::Black);
	if (m_currentMenuState == menuState::MAIN_MENU)
	{
		sf::View uiView;
		uiView.setSize({ VIRTUAL_WIDTH, VIRTUAL_HEIGHT });
		uiView.setCenter({ VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.5f });
		sf::FloatRect viewport;
		viewport.position = { 0.f, 0.f };
		viewport.size = { 1.f, 1.f };

		uiView.setViewport(viewport);
		m_window.setView(uiView);

		m_window.draw(m_mainMenuSprite);
		
		m_window.draw(m_mainMenuTitleHighlight);
		m_window.draw(m_mainMenuTitleText);
		for (auto& b : m_menuButtons)
		{
			m_window.draw(b.sprite);
			m_window.draw(b.text);
		}
		m_window.display();
		return;
	}
	m_camera.applyCam(m_window);
	m_mapRenderer.drawLayered(m_window, sf::RenderStates::Default, false);
	for (auto& npc : m_npcs) {
		npc.draw(m_window);
	}
	m_player.draw(m_window);
	for (auto& item : m_items)
	{
		item.draw(m_window);
	}
	
	m_mapRenderer.drawLayered(m_window, sf::RenderStates::Default, true);
	
	//DEBUG: Drawing Collision Rects
	
	//for (const auto& r : m_mapRenderer.getCollisionRects())
	//{
	//	sf::RectangleShape box;
	//	box.setPosition(r.position);
	//	box.setSize(r.size);
	//	box.setFillColor({ 255,0,0,100 });
	//	m_window.draw(box);
	//}

	sf::View uiView;
	uiView.setSize({ m_camera.getVirtualSize()});
	uiView.setCenter({ m_camera.getVirtualSize() * 0.5f});

	uiView.setViewport(m_camera.getViewport());

	m_window.setView(uiView);
	m_hud.draw(m_window);

	m_window.display();
}

void Game::spawnNPC(sf::Vector2f position, EnemyType type)
{
	auto tex = getEnemyTexture(type);
	if (!tex) return;

	//create and add NPC to vector and gives it the pursue behavuior alos sets its position
	m_npcs.emplace_back(tex);
	m_npcs.back().setAIMode(AIBehaviour::Mode::Pursue);
	m_npcs.back().setPosition(position.x, position.y);
	m_npcs.back().setType(type);
}

void Game::setupMenuView()
{
	//sets up the view for the main menu to make usre it scales properly on different screen sizes and maintains a consistent layout
	m_menuView.setSize({ VIRTUAL_WIDTH, VIRTUAL_HEIGHT });
	m_menuView.setCenter({ VIRTUAL_WIDTH * 0.5f, VIRTUAL_HEIGHT * 0.5f });
	m_menuView.setViewport({ {0.f, 0.f}, {1.f, 1.f} });
}

void Game::setupMainMenuButtons()
{
	m_menuButtons.clear();

	const float startY = 500.f;
	const float gap = 200.f;
	//creates the main menu buttons and sets their on click functions to change the menu state or exit the game
	createMenuButton("PLAY", startY, [this]()
		{
			m_currentMenuState = menuState::GAMEPLAY;
		});

	createMenuButton("SETTINGS", startY + gap, [this]()
		{
			m_currentMenuState = menuState::SETTINGS;
		});

	createMenuButton("EXIT", startY + gap * 2.f, [this]()
		{
			m_DELETEexitGame = true;
		});
}

void Game::createMenuButton(const std::string& label, float y, std::function<void()> onClick)
{
	m_menuButtons.emplace_back(m_buttonTexture, m_MagicalWorldFont);
	Button& button = m_menuButtons.back();

	// button sprite size / position
	button.sprite.setScale({ 0.6f, 0.6f });

	const auto spriteLocal = button.sprite.getLocalBounds();
	button.sprite.setOrigin({
		spriteLocal.position.x + spriteLocal.size.x * 0.5f,
		spriteLocal.position.y + spriteLocal.size.y * 0.5f
		});

	button.sprite.setPosition({ VIRTUAL_WIDTH * 0.5f, y });

	// text style and setup
	button.text.setString(label);
	button.text.setCharacterSize(72);
	button.text.setFillColor(sf::Color::White);


	// center text on sprite
	const auto textBounds = button.text.getLocalBounds();
	button.text.setOrigin({
		textBounds.position.x + textBounds.size.x * 0.5f,
		textBounds.position.y + textBounds.size.y * 0.5f
		});
	//postion text on sprite center
	button.text.setPosition(button.sprite.getPosition());

	button.onClick = std::move(onClick);
}

void Game::updateMenuHighlight()
{

	//updates the visual state of the menu buttons based on which one is currently selected
	for (int i = 0; i < (int)m_menuButtons.size(); ++i)
	{
		const bool selected = (i == m_selectedButton);

		m_menuButtons[i].hovered = selected;
		m_menuButtons[i].sprite.setScale(selected ? sf::Vector2f{ 0.65f, 0.65f } : sf::Vector2f{ 0.6f, 0.6f });
		m_menuButtons[i].text.setFillColor(selected ? sf::Color::Yellow : sf::Color::White);
	}
}

void Game::moveMenuSelection(int direction)
{
	if (m_menuButtons.empty()) 
		return;

	m_selectedButton += direction;
	//wrap around selection
	if (m_selectedButton < 0) 
		m_selectedButton = (int)m_menuButtons.size() - 1;
	if (m_selectedButton >= (int)m_menuButtons.size())
		m_selectedButton = 0;

	updateMenuHighlight();
}

void Game::activateSelectedButton()
{
	if (m_menuButtons.empty()) 
		return;

	auto& button = m_menuButtons[m_selectedButton];
	if (button.onClick) button.onClick();
}

void Game::setupMainMenuTitle()
{
	const sf::String title = "Can I Hinder?";
	const int size = 120;

	m_mainMenuTitleText.setFont(m_MagicalWorldFont);
	m_mainMenuTitleText.setString(title);
	m_mainMenuTitleText.setCharacterSize(size);
	m_mainMenuTitleText.setFillColor(sf::Color(205, 198, 178));   // a warm off white color for the title
	m_mainMenuTitleText.setOutlineColor(sf::Color(15, 10, 5)); // a dark brown outline for contrast
	m_mainMenuTitleText.setOutlineThickness(3.f);

	// center the text origin
	auto bounds = m_mainMenuTitleText.getLocalBounds();
	sf::Vector2f origin{
		bounds.position.x + bounds.size.x * 0.5f,
		bounds.position.y + bounds.size.y * 0.5f
	};

	m_mainMenuTitleText.setOrigin(origin);
	//position the title near the top center of the screen
	m_mainMenuTitleText.setPosition({ VIRTUAL_WIDTH * 0.5f, 180.f });

	m_mainMenuTitleHighlight = m_mainMenuTitleText;
	m_mainMenuTitleHighlight.setFillColor(sf::Color(255, 250, 235, 120));
	m_mainMenuTitleHighlight.setOutlineColor(sf::Color(255, 255, 255, 80));
	m_mainMenuTitleHighlight.setOutlineThickness(2.f);
	m_mainMenuTitleHighlight.move({ -2.f, -2.f }); // slight offset for a subtle glow effect
}


std::shared_ptr<sf::Texture> Game::getEnemyTexture(EnemyType type)
{
	//uses a cache to avoid reloading same texture multiple times to save memory
	auto textureIt = m_enemyTextures.find(type);
	if (textureIt != m_enemyTextures.end())
		return textureIt->second;

	auto texture = std::make_shared<sf::Texture>();
	const char* filePath = "";

	switch (type)
	{
	case EnemyType::Skeleton: 
		filePath = "ASSETS/IMAGES/Skeleton.png";
		break;
	case EnemyType::Goblin:   
		filePath = "ASSETS/IMAGES/Goblin.png";   
		break;
	}

	if (!texture->loadFromFile(filePath))
	{
		std::cout << "Failed to load enemy texture: " << filePath << "\n";
		return nullptr;
	}

	m_enemyTextures[type] = texture;
	return texture;
}





