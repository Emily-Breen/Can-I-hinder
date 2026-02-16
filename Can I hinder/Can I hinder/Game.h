
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
#include <cstdlib> 
#include <ctime>   
#include <unordered_map>
#include <vector>
#include <functional>
#include "Map.h"
#include "NPC.h"
#include<iostream>
#include "Camera.h"
#include "HUD.h"
#include "Items.h"
#include "Audio.h"
#include "Input Handler.h"

//enum for menu states
enum class menuState
{
	MAIN_MENU,
	SETTINGS,
	GAMEPLAY,
	PAUSE,
	GAME_OVER
};
//Button struct for menu buttons
struct Button
{
	sf::Sprite sprite;
	sf::Text text;
	std::function<void()> onClick;
	bool hovered{ false };
	// constructor to initialize sprite and text with texture and font
	Button(const sf::Texture& tex, const sf::Font& font)
		: sprite(tex), text(font)
	{
	}
	// function to get the global bounds of the button for hover detection
	sf::FloatRect bounds() const { return sprite.getGlobalBounds(); }
};

//Consts
const sf::Color ULTRAMARINE{ 5, 55,242,255 }; // const colour
constexpr float VIRTUAL_WIDTH = 1920.f;
constexpr float VIRTUAL_HEIGHT = 1080.f;
constexpr bool USE_LOCAL_WS = true; //set this to true to do local testing ws and false for wss azure
class Game
{
public:
	Game();
	~Game();
	void run();

private:
	//core game loop
	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void update(sf::Time t_deltaTime);
	void render();

	//Enemy system
	void spawnNPC(sf::Vector2f position, EnemyType type);

	std::vector<NPC> m_npcs;
	std::unordered_map<EnemyType, std::shared_ptr<sf::Texture>> m_enemyTextures; // enemy textures using unordered map to avoid reloading same texture
	std::shared_ptr<sf::Texture> getEnemyTexture(EnemyType type);

	//Menu system
	void setupMenuView();
	void setupMainMenuButtons();
	void createMenuButton(const std::string& label, float y, std::function<void()> onClick);
	void updateMenuHighlight();
	void moveMenuSelection(int direction);
	void activateSelectedButton();
	void setupMainMenuTitle();

	std::vector<Button> m_menuButtons;
	sf::Texture m_mainMenuTexture;
	sf::Sprite  m_mainMenuSprite{ m_mainMenuTexture };
	sf::Texture m_buttonTexture;
	sf::View m_menuView;
	int m_selectedButton = 0;

	//Items
	std::vector<Items> m_items;
	int m_keyCount = 3; // for testing level progression

	//Player & gameplay
	Player m_player; // player object
	InputHandler m_inputHandler;
	float m_testHealth = 1.0f;
	float m_playerDamageMultiplier = 1.0f;
	float m_stealPowerDuration = 0.0f;

	//Networking events
	WebsocketClient m_client; // websocket client
	bool spawnEnemy{ false };
	bool healPlayer{ false };
	bool slowPlayer{ false };
	bool weakenPlayer{ false };
	bool m_stealPowerActive{ false };

	//Timers and state flags
	sf::Clock m_stealPowerClock;
	sf::Clock m_menuClock;
	bool m_DELETEexitGame; // control exiting game
	bool isSpawnNPC{ false };

	//Windowing and rendering
	sf::RenderWindow m_window; // main SFML window
	menuState m_currentMenuState{ menuState::MAIN_MENU };
	menuState m_prevState{ menuState::GAMEPLAY };

	//UI and Hud
	sf::Font m_MagicalWorldFont;// font used by message
	sf::Text m_mainMenuTitleText{m_MagicalWorldFont};
	sf::Text m_mainMenuTitleShadow{ m_MagicalWorldFont };
	sf::Text m_mainMenuTitleHighlight{ m_MagicalWorldFont };
	HUD m_hud;
	
	//Map and Camera
	Camera m_camera;
	MapRenderer m_mapRenderer;

	//Audio
	Audio m_audio;
	

	
	
	
	
	
	


};

#pragma warning( pop ) 
#endif // !GAME_HPP

