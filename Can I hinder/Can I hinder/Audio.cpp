#include "Audio.h"

Audio::Audio() : soundEffect(soundBuffer), walkingSound(walkingBuffer), npcWalkingSound(npcWalkingBuffer), swordSlashSound(swordSlashBuffer)
{
	if (!walkingBuffer.loadFromFile("ASSETS/AUDIO/SFX/walking effect.ogg"))
	{
		std::cout << "Failed to load footsteps sound\n";
	}

	walkingSound.setLooping(true);
	walkingSound.setVolume(35.f);

	if (!npcWalkingBuffer.loadFromFile("ASSETS/AUDIO/SFX/Skeleton.ogg")) {
		std::cout << "Failed to load NPC footsteps\n";
	}
		
	npcWalkingSound.setLooping(true);
	npcWalkingSound.setVolume(50.f);

	if (!swordSlashBuffer.loadFromFile("ASSETS/AUDIO/SFX/sword slash.ogg"))
	{
		std::cout << "Failed to load sword slash sound\n";
	}
	swordSlashSound.setLooping(false);
	swordSlashSound.setVolume(50.f);
}

void Audio::playSoundEffect(const char* soundFilePath)
{
	if (!soundBuffer.loadFromFile(soundFilePath))
	{
		std::cout << "Failed to load sound effect: " << soundFilePath << std::endl;
		return;
	}

	soundEffect.setBuffer(soundBuffer);
	soundEffect.play();
}

void Audio::startWalkingSound()
{
	if (walkingSound.getStatus() != sf::SoundSource::Status::Playing)
		walkingSound.play();
}

void Audio::stopWalkingSound()
{
	if (walkingSound.getStatus() == sf::SoundSource::Status::Playing)
		walkingSound.stop();
}

void Audio::startNpcWalkingSound()
{
	if (npcWalkingSound.getStatus() != sf::SoundSource::Status::Playing)
		npcWalkingSound.play();
}

void Audio::stopNpcWalkingSound()
{
	if (npcWalkingSound.getStatus() == sf::SoundSource::Status::Playing)
		npcWalkingSound.stop();
}

void Audio::startswordSlashSound()
{
	if (swordSlashSound.getStatus() != sf::SoundSource::Status::Playing)
		swordSlashSound.play();
}

void Audio::stopswordSlashSound()
{
	if (swordSlashSound.getStatus() != sf::SoundSource::Status::Playing)
		swordSlashSound.stop();
}

void Audio::playInGameBackgroundMusic(const char* musicFilePath)
{
	if (backgroundMusic.openFromFile(musicFilePath))
	{
		backgroundMusic.setLooping(true);
		backgroundMusic.setVolume(20.f); //set to 60% volume
		backgroundMusic.play();
	}
	else
	{
		std::cout << "Failed to load in-game music: " << musicFilePath << std::endl;
	}
}

void Audio::playMenuBackgroundMusic(const char* musicFilePath)
{
	if (backgroundMenuMusic.openFromFile(musicFilePath))
	{
		backgroundMenuMusic.setLooping(true);
		backgroundMenuMusic.setVolume(50.f); //set to 60% volume
		backgroundMenuMusic.play();
	}
	else
	{
		std::cout << "Failed to load menu music: " << musicFilePath << std::endl;
	}
}

void Audio::stopBackgroundMusic()
{
	backgroundMusic.stop();
}
