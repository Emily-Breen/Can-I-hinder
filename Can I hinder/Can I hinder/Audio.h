#pragma once
#include "SFML/Audio.hpp"

class Audio
{
public:
	void playSoundEffect(const char* soundFilePath);
	void playInGameBackgroundMusic(const char* musicFilePath);
	void playMenuBackgroundMusic(const char* musicFilePath);
	void stopBackgroundMusic();

private:
	sf::SoundBuffer soundBuffer;
	sf::Sound soundEffect;
	sf::Music backgroundMusic;

};

