
#include "Game.h"

#include <iostream>




Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{800U, 600U}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{false} //when true game will exit
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

		}
		if (healPlayer) {
			std::cout << "You have been healed!" << std::endl;
			healPlayer = false;
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
	if (sf::Keyboard::Key::W == newKeypress->code)
	{
		m_player.movePlayer(sf::Vector2f(0.f, -5.f));
	}
	if (sf::Keyboard::Key::A == newKeypress->code)
	{
		m_player.movePlayer(sf::Vector2f(-5.f, 0.f));
	}
	if (sf::Keyboard::Key::S == newKeypress->code)
	{
		m_player.movePlayer(sf::Vector2f(0.f, 5.f));
	}
	if (sf::Keyboard::Key::D == newKeypress->code)
	{
		m_player.movePlayer(sf::Vector2f(5.f, 0.f));
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
	}
	m_player.playerUpdate(t_deltaTime.asSeconds());
}


void Game::render()
{
	m_window.clear(sf::Color::Black);
	m_player.draw(m_window);
	m_window.display();
}




