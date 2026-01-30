#pragma once
#include "SFML/Audio.hpp"
#include <iostream>

class Audio
{
public:
	Audio();
	void playSoundEffect(const char* soundFilePath);
	void startWalkingSound();
	void stopWalkingSound();
	void startNpcWalkingSound();
	void stopNpcWalkingSound();
	void startswordSlashSound();
	void stopswordSlashSound();
	void playInGameBackgroundMusic(const char* musicFilePath);
	void playMenuBackgroundMusic(const char* musicFilePath);
	void stopBackgroundMusic();

private:
	sf::SoundBuffer soundBuffer;
	sf::Sound soundEffect;

	sf::SoundBuffer walkingBuffer;    
	sf::Sound walkingSound;

	sf::SoundBuffer npcWalkingBuffer;
	sf::Sound npcWalkingSound;

	sf::SoundBuffer swordSlashBuffer;
	sf::Sound swordSlashSound;

	sf::Music backgroundMusic;
	sf::Music backgroundMenuMusic;

};

