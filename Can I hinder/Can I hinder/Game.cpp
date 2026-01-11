
#include "Game.h"

#include <iostream>




Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{1920, 1080U}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{false}, m_camera(1920,1080) //when true game will exit
{ 
	
	m_client.setOnMessage([this](const std::string& action, const std::string& effect) {
		if (action == "hinder" && effect == "spawn_enemy") {
			spawnEnemy = true;
		}
		else if (action == "help" && effect == "heal_player") {
			healPlayer = true;
		}
		});
	
	m_client.connect("localhost", "8080");



	//TESTING LOADING A MAP

	m_mapRenderer.load("ASSETS/LEVELS/Map.tmx");
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
		if (spawnEnemy) {
			std::cout << "An enemy has spawned to hinder you!" << std::endl;
			spawnEnemy = false;

			// Pick a spawn position - near player for now 
			sf::Vector2f spawnPos = m_player.getPosition() + sf::Vector2f(200.f, 0.f);
			spawnNPC(spawnPos);
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

		
		m_player.update(t_deltaTime.asSeconds());

		
		sf::Vector2f direction = m_player.getMovement(); 
		std::cout << "direction = (" << direction.x << ", " << direction.y << ")\n";
		
		float speed = 300.f; 
		sf::Vector2f movement = direction * speed * t_deltaTime.asSeconds();

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
				if (rectsIntersect(nextBounds, wall))
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

		// for now npcs are free to roam into the sunset :D - 11/01/26 Collisions added but NPC is dumb and likes licking the walls some Ai Behaviours added its a little janky atm :O
		static sf::Vector2f lastPlayerPos = m_player.getPosition();
		sf::Vector2f playerPos = m_player.getPosition();
		sf::Vector2f playerVel = (playerPos - lastPlayerPos) / t_deltaTime.asSeconds();
		lastPlayerPos = playerPos;

		for (auto& npc : m_npcs)
		{
			npc.update(t_deltaTime.asSeconds()); 

			sf::Vector2f delta = npc.computeAIMovement(playerPos, playerVel, t_deltaTime.asSeconds());
			if (delta == sf::Vector2f(0.f, 0.f))
				continue;

			sf::FloatRect next = npc.getBounds();
			next.position += delta;

			bool blocked = false;
			for (const auto& wall : m_mapRenderer.getCollisionRects())
			{
				if (rectsIntersect(next, wall))
				{
					blocked = true;
					break;
				}
			}

			if (!blocked)
			{
				npc.movement(delta);
			}
			else
			{
				// Stop sliding into a wall
				npc.setVelocity({ 0.f, 0.f });
			}
		}

		//Camera follows player
		m_camera.follow(m_player.getPosition());

}


void Game::render()
{
	m_camera.applyCam(m_window);
	m_window.clear(sf::Color::Black);
	m_mapRenderer.drawLayered(m_window, sf::RenderStates::Default, false);
	m_player.draw(m_window);
	for (auto& npc : m_npcs) {
		npc.draw(m_window);
	}
	m_mapRenderer.drawLayered(m_window, sf::RenderStates::Default, true);

	for (const auto& r : m_mapRenderer.getCollisionRects())
	{
		sf::RectangleShape box;
		box.setPosition(r.position);
		box.setSize(r.size);
		box.setFillColor({ 255,0,0,100 });
		m_window.draw(box);
	}
	m_window.display();
}

void Game::spawnNPC(sf::Vector2f position)
{
	if (!m_enemyTexture) {
		m_enemyTexture = std::make_shared<sf::Texture>();
		if (!m_enemyTexture->loadFromFile("ASSETS/IMAGES/Skeleton.png")) {
			std::cout << "Failed to load enemy texture\n";
			return;
		}
	}

	NPC newNPC(m_enemyTexture);
	//added AI Behaviour here just to test how things run for now 
	newNPC.setAIMode(AIBehaviour::Mode::Pursue);
	newNPC.setPosition(position.x,position.y);
	m_npcs.push_back(newNPC); 
}

bool Game::rectsIntersect(const sf::FloatRect& a, const sf::FloatRect& b)
{
	return !(
		a.position.x + a.size.x <= b.position.x ||
		a.position.x >= b.position.x + b.size.x ||
		a.position.y + a.size.y <= b.position.y ||
		a.position.y >= b.position.y + b.size.y
		);
}





