
#include "Game.h"

#include <iostream>



auto desktopMode = sf::VideoMode::getDesktopMode(); 
Game::Game() :
	m_window{ desktopMode,"SFML Game 3.0",sf::State::Fullscreen },
	m_DELETEexitGame{ false }, m_camera(static_cast<float>(sf::VideoMode::getDesktopMode().size.x), static_cast<float>(sf::VideoMode::getDesktopMode().size.y)
	) //when true game will exit
{

	m_client.setOnMessage([this](const std::string& action, const std::string& effect)
		{
			std::string user = "User123"; //just for testing should if all going right be set from the PWA to display the username of the user sending the action

			if (action == "hinder" && effect == "spawn_enemy")
			{
				spawnEnemy = true;
				m_hud.pushChatMessage(user, "sent an enemy!", sf::Color(255, 80, 80));
			}
			else if (action == "help" && effect == "heal_player")
			{
				healPlayer = true;
				m_hud.pushChatMessage(user, "healed you!", sf::Color(80, 255, 80));
			}
		});

	m_client.connect("localhost", "8080");





	m_mapRenderer.load("ASSETS/LEVELS/Map.tmx");

	std::cout << "player posX: " << m_player.getPosition().x << "player posY: " << m_player.getPosition().y;
	if (m_mapRenderer.getMapPath() == "ASSETS/LEVELS/Map4.tmx")
	{
		m_player.setPosition({ 450.f, 1900.f });
		return;
	}
	std::cout << "Doors loaded: " << m_mapRenderer.getDoors().size() << "\n";
		

	if (!m_hud.load())
	{
		std::cout << "Failed to load HUD\n";
	}
	else
	{
		m_hud.layout(m_window.getSize()); // makes sure the hud elements are in the right place when loaded.
	}
	std::srand(static_cast<unsigned>(std::time(nullptr)));
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

			sf::Vector2f spawnPos =
				m_player.getPosition() + sf::Vector2f(200.f, 0.f);

			EnemyType type =
				(std::rand() % 2 == 0)
				? EnemyType::Skeleton
				: EnemyType::Goblin;

			spawnNPC(spawnPos, type);
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
	}
}



void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_DELETEexitGame = true; 
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


void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_DELETEexitGame = true; 
	}
}


void Game::update(sf::Time t_deltaTime)
{
	
		checkKeyboardState();
		if (m_DELETEexitGame)
		{
			m_window.close();
			return;
		}
		//handle menu state changes sound only right now
		if (m_currentMenuState != m_prevState)
		{
			m_audio.stopBackgroundMusic();

			switch (m_currentMenuState)
			{
			case menuState::MAIN_MENU:
				
				m_audio.playMenuBackgroundMusic("ASSETS/AUDIO/BACKGROUND MUSIC/Main menu music.ogg");


				break;

			case menuState::GAMEPLAY:
				m_audio.playInGameBackgroundMusic("ASSETS/AUDIO/BACKGROUND MUSIC/Main game music.ogg");
				break;

			case menuState::GAME_OVER:
				
				break;

			case menuState::PAUSE:
				
				break;

			case menuState::SETTINGS:


				break;
			}

			m_prevState = m_currentMenuState;
		}

		static bool attackWasHeld = false;
		const bool attackHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space); 
		const bool attackJustPressed = attackHeld && !attackWasHeld;
		attackWasHeld = attackHeld;
		
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

			for (const auto& door : m_mapRenderer.getDoors())
			{
				if (Entity::rectsIntersect(nextBounds, door.rect))
				{
					if (m_keyCount >= door.requiredKeys)
					{
						std::cout << "Door spawn read: " << door.spawn.x << ", " << door.spawn.y << "\n";
						m_mapRenderer.load(door.nextMap);

						
						m_player.setPosition(door.spawn);
						//m_player.setPosition({ 1590.f, 4621.f });
						m_camera.follow(m_player.getPosition());
						m_camera.applyCam(m_window);
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

			for (auto& npc : m_npcs)
			{
				if (npc.isDead()) 
					continue;
				if (Entity::rectsIntersect(atk, npc.getBounds()))
					npc.takeDamage(0.25f);
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
	m_camera.applyCam(m_window);
	m_window.clear(sf::Color::Black);
	m_mapRenderer.drawLayered(m_window, sf::RenderStates::Default, false);
	
	m_player.draw(m_window);
	for (auto& item : m_items)
	{
		item.draw(m_window);
	}
	for (auto& npc : m_npcs) {
		npc.draw(m_window);
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

	m_window.setView(m_window.getDefaultView());
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





