#ifndef RESOURCESMANAGER_INCLUDE
#define RESOURCESMANAGER_INCLUDE

#include <memory>
#include <mutex>
#include <string>
#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "h_config.h"

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

	sf::SoundBuffer failBuffer;
	sf::Sound failSound;

	sf::SoundBuffer transitionBuffer;
	sf::Sound transitionSound;

public:
	void playDoorSound();
	void playClickSound();
	void playEnterSound();
	void playSelectSound();
	void playHurtSound();
	void playDieSound();
	void playWinSound();
	void playFailSound();
	void playTransitionSound();	
};

class DFont
{
public:
	static DFont& instance()
	{
		static DFont theSingleInstance;
		return theSingleInstance;
	}

private:
	DFont()
	{
		font.loadFromFile(fontPath);
	};
	DFont( const SoundManager& );
	DFont& operator =( const SoundManager& );

	sf::Font font;

public:
	sf::Font &getFont(){return font;};
};

#endif