
#include "Game.h"

#include <iostream>



auto desktopMode = sf::VideoMode::getDesktopMode(); 
Game::Game() :
	m_window{desktopMode,"SFML Game 3.0",sf::State::Fullscreen},
	m_DELETEexitGame{false}, m_camera(static_cast<float>(sf::VideoMode::getDesktopMode().size.x),static_cast<float>(sf::VideoMode::getDesktopMode().size.y)
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
		static bool attackWasHeld = false;
		const bool attackHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space); 
		const bool attackJustPressed = attackHeld && !attackWasHeld;
		attackWasHeld = attackHeld;
		
		m_player.update(t_deltaTime.asSeconds());

		//tracks direction of player movement
		sf::Vector2f direction = m_player.getMovement(); 
		
		float speed = 300.f; 
		sf::Vector2f movement = direction * speed * t_deltaTime.asSeconds();
		//for collision with walls 
		if (movement != sf::Vector2f(0.f, 0.f))
		{
			//tracks movement of player
			sf::FloatRect nextBounds = m_player.getBounds();
			nextBounds.position += movement;

			//this is for the tiles
			bool blocked = false;
			const auto& walls = m_mapRenderer.getCollisionRects();

			for (const auto& wall : walls)
			{
				if (Entity::rectsIntersect(nextBounds, wall))
				{
					blocked = true;
					break;
				}
			}

			//only move if there is no collision
			if (!blocked)
			{
				m_player.movement(movement);
			}
		}

		// for now npcs are free to roam into the sunset :D - 11/01/26 Collisions added but NPC is dumb and likes licking the walls some Ai Behaviours added its a little janky atm :O 18/01/2026 
		// NPCS behaviour is a bit smart but have to elaborate more as in flee if player strength increases etc
		static sf::Vector2f lastPlayerPos = m_player.getPosition();
		sf::Vector2f playerPos = m_player.getPosition();
		sf::Vector2f playerVel = (playerPos - lastPlayerPos) / t_deltaTime.asSeconds();
		lastPlayerPos = playerPos;

		if (!m_player.isDead()&& attackJustPressed)
		{
			const sf::FloatRect atk = m_player.getAttackBounds();
			

			for (auto& npc : m_npcs)
			{
				if (npc.isDead()) 
					continue;
				if (Entity::rectsIntersect(atk, npc.getBounds()))
					npc.takeDamage(0.25f);
			}
		}

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

			float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

			const float attackRange = npc.getAttackRange();

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

				auto SeparationVector = [&](NPC& self) -> sf::Vector2f
					{
						const float desiredSeparation = 100.f;
						const float desiredSeparationSq = desiredSeparation * desiredSeparation;

						sf::Vector2f seperationForce{ 0.f, 0.f };
						int neighbourCount = 0;

						const sf::Vector2f selfPos = self.getPosition();

						for (auto& neighbour : m_npcs)
						{
							if (&neighbour == &self) 
								continue;
							if (neighbour.isDead()) 
								continue;

							sf::Vector2f offset = selfPos - neighbour.getPosition();
							float offsetSq = offset.x * offset.x + offset.y * offset.y;

							if (offsetSq > 0.0001f && offsetSq < desiredSeparationSq)
							{
								seperationForce += offset; 
								++neighbourCount;
							}
						}

						if (neighbourCount > 0)
							seperationForce /= static_cast<float>(neighbourCount);

						return seperationForce;
				};
				sf::Vector2f sep = SeparationVector(npc);
				const auto& walls = m_mapRenderer.getCollisionRects();
				sf::Vector2f delta = npc.computeAIMovement(playerPos, playerVel, dt, sep,walls);
				npc.moveWithCollision(delta, walls);

				if (delta != sf::Vector2f(0.f, 0.f))
				{
					sf::FloatRect next = npc.getBounds();
					next.position += delta;

					bool blocked = false;
					for (const auto& wall : m_mapRenderer.getCollisionRects())
					{
						if (Entity::rectsIntersect(next, wall))
						{
							blocked = true;
							break;
						}
					}

					if (!blocked)
						npc.movement(delta);
					else
						npc.setVelocity({ 0.f, 0.f });
				}
			}

			npc.update(dt);
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

	NPC newNPC(tex);
	newNPC.setAIMode(AIBehaviour::Mode::Pursue);
	newNPC.setPosition(position.x, position.y);
	m_npcs.push_back(newNPC);
}


std::shared_ptr<sf::Texture> Game::getEnemyTexture(EnemyType type)
{
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





