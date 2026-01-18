
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "WebSocketClient.h"
#include "Player.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <unordered_map>
#include "Map.h"
#include "NPC.h"
#include<iostream>
#include "Camera.h"
#include "HUD.h"



const sf::Color ULTRAMARINE{ 5, 55,242,255 }; // const colour


class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();
	void spawnNPC(sf::Vector2f position);
	bool rectsIntersect(const sf::FloatRect& a, const sf::FloatRect& b);
	
	Player m_player; // player object
	std::vector<NPC> m_npcs;
	std::shared_ptr<sf::Texture> m_enemyTexture;
	WebsocketClient m_client; // websocket client
	bool spawnEnemy{ false };
	bool healPlayer{ false };
	Camera m_camera;
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_jerseyFont;// font used by message
	bool m_DELETEexitGame; // control exiting game
	bool isSpawnNPC{ false };

	//TESTING
	float m_testHealth = 1.0f;

	MapRenderer m_mapRenderer;
	HUD m_hud;
	float m_playerHP = 1.f;
	tmx::Map m_map;


};

#pragma warning( pop ) 
#endif // !GAME_HPP

