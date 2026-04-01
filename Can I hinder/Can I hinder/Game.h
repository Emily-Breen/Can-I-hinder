
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
#include "Effects.h"
#include "FinalLevel.h"

//enum for menu states
enum class menuState
{
	MAIN_MENU,
	SETTINGS,
	GAMEPLAY,
	BOSS_BATTLE,
	PAUSE,
	GAME_OVER
};
//struct for spawn delay to hold position and type of enemy to spawn after a delay
struct SpawnDelay {
	sf::Vector2f position;
	EnemyType type;

};
struct GameStats {
	int enemiesDefeated;
	int helpsReceived;
	int hindersReceived;
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
constexpr bool USE_LOCAL_WS = false; //set this to true to do local testing ws and false for wss azure
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
	void resetGame();

	//Enemy system
	void spawnNPC(sf::Vector2f position, EnemyType type);

	std::vector<NPC> m_npcs;
	std::unordered_map<EnemyType, std::shared_ptr<sf::Texture>> m_enemyTextures; // enemy textures using unordered map to avoid reloading same texture
	std::shared_ptr<sf::Texture> getEnemyTexture(EnemyType type);

	//Menu system
	void setupMenuView();
	void setupMainMenuButtons();
	void setupGameOverButtons();
	void createMenuButton(const std::string& label, float y, std::function<void()> onClick);
	void updateMenuHighlight();
	void moveMenuSelection(int direction);
	void activateSelectedButton();
	void setupMainMenuTitle();
	void setupPauseMenu();
	void setUpSessionCode(const std::string& code);

	std::vector<Button> m_menuButtons;
	sf::Texture m_mainMenuTexture;
	sf::Sprite  m_mainMenuSprite{ m_mainMenuTexture };
	sf::Texture m_gameOverTexture;
	sf::Sprite  m_gameOverSprite{ m_gameOverTexture };
	sf::Texture m_buttonTexture;
	sf::View m_menuView;
	int m_selectedButton = 0;

	//Items
	std::vector<Items> m_items;
	int m_keyCount = 3; // for testing level progression

	//Player & gameplay
	Player m_player; // player object
	InputHandler m_inputHandler;
	FinalLevel m_finalLevel;
	GameStats m_stats;
	std::vector<std::unique_ptr<Effects>> m_effects;
	float m_playerHealth = 1.0f;
	float m_playerDamageMultiplier = 1.0f;
	float m_stealPowerDuration = 0.0f;
	float m_powerMultiplier = 1.0f;
	float m_powerBuffDuration = 0.0f;
	const float m_baseMoveSpeed = 300.f;
	const float m_baseDamage = 0.25f;
	float m_speedMultiplier = 1.0f;
	float m_speedBuffDuration = 0.0f;
	float m_slowMultiplier = 1.0f;
	float m_slowDuration = 0.0f;
	sf::RectangleShape m_pauseOverlay;
	std::shared_ptr<sf::Texture> m_poofTexture;
	std::vector<SpawnDelay> m_spawnDelay;
	

	//Networking events
	WebsocketClient m_client; // websocket client
	bool spawnEnemy{ false };
	bool spawnBrute{ false };
	bool healPlayer{ false };
	bool slowPlayer{ false };
	bool weakenPlayer{ false };
	bool m_stealPowerActive{ false };
	bool speedUpPlayer{ false };
	bool powerBoostPlayer{ false };
	bool  m_speedBuffActive = false;
	bool  m_powerBuffActive = false;
	bool m_slowActive = false;
	std::string session;

	//Timers and state flags
	sf::Clock m_stealPowerClock;
	sf::Clock m_menuClock;
	sf::Clock m_speedBuffClock;
	sf::Clock m_powerBuffClock;
	sf::Clock m_slowClock;
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
	sf::Text m_sessionCodeText{ m_MagicalWorldFont };
	sf::Text gameOverText{ m_MagicalWorldFont };
	sf::Text m_pauseText{ m_MagicalWorldFont };
	std::string m_sessionCode;
	HUD m_hud;
	
	//Map and Camera
	Camera m_camera;
	MapRenderer m_mapRenderer;

	//Audio
	Audio m_audio;
	
	
	
	
	
	
	


};

#pragma warning( pop ) 
#endif // !GAME_HPP

