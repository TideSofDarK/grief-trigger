#ifndef RESOURCESMANAGER_INCLUDE
#define RESOURCESMANAGER_INCLUDE

#include <memory>
#include <mutex>
#include <string>
#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class TextureManager
{
public:
	static TextureManager& instance()
	{
		static TextureManager theSingleInstance;
		return theSingleInstance;
	}

private:
	TextureManager() {};
	TextureManager( const TextureManager& );
	TextureManager& operator =( const TextureManager& );

public:
	const sf::Texture&	getTexture( const std::string& filename );
	void deleteTexture( const sf::Texture& image );
	void deleteTexture( const std::string& filename );

private:
	std::map< std::string, sf::Texture > textures;
};

class SoundManager
{
public:
	static SoundManager& instance()
	{
		static SoundManager theSingleInstance;
		return theSingleInstance;
	}

private:
	SoundManager();
	SoundManager( const SoundManager& );
	SoundManager& operator =( const SoundManager& );

	sf::SoundBuffer doorBuffer;
	sf::Sound doorSound;

	sf::SoundBuffer enterBuffer;
	sf::Sound enterSound;

	sf::SoundBuffer clickBuffer;
	sf::Sound clickSound;

	sf::SoundBuffer selectBuffer;
	sf::Sound selectSound;

	sf::SoundBuffer hurtBuffer;
	sf::Sound hurtSound;

	sf::SoundBuffer dieBuffer;
	sf::Sound dieSound;

	sf::SoundBuffer winBuffer;
	sf::Sound winSound;

public:
	void playDoorSound();
	void playClickSound();
	void playEnterSound();
	void playSelectSound();
	void playHurtSound();
	void playDieSound();
	void playWinSound();
};

#endif