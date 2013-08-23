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

	sf::SoundBuffer hitBuffer;
	sf::Sound hitSound;

	sf::SoundBuffer critBuffer;
	sf::Sound critSound;

	sf::SoundBuffer transitionBuffer;
	sf::Sound transitionSound;

	sf::SoundBuffer successBuffer;
	sf::Sound successSound;

	sf::SoundBuffer chestBuffer;
	sf::Sound chestSound;

	sf::SoundBuffer step1Buffer;
	sf::Sound step1Sound;
	sf::SoundBuffer step2Buffer;
	sf::Sound step2Sound;
	sf::SoundBuffer step3Buffer;
	sf::Sound step3Sound;
	sf::SoundBuffer step4Buffer;
	sf::Sound step4Sound;

	sf::SoundBuffer noiseBuffer;
	sf::Sound		noiseSound;

	sf::SoundBuffer pressBuffer;
	sf::Sound		pressSound;

	bool lastStep;

public:
	void playCritSound();
	void playHitSound();
	void playChestSound();
	void playDoorSound();
	void playClickSound();
	void playEnterSound();
	void playSelectSound();
	void playHurtSound();
	void playDieSound();
	void playWinSound();
	void playFailSound();
	void playTransitionSound();	
	void playNoiseSound();	
	void playPressSound();	
	void playSuccessSound();	

	void playStep1Sound();	
	void playStep2Sound();	
	void playStep3Sound();	
	void playStep4Sound();	
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
		gothicFont.loadFromFile("assets/fonts/gothic.ttf");
	};
	DFont( const DFont& );
	DFont& operator =( const DFont& );

	sf::Font font;
	sf::Font gothicFont;

public:
	sf::Font &getFont(){return font;};
	sf::Font &getGothicFont(){return gothicFont;};
};

class MusicManager
{
public:
	static MusicManager& instance()
	{
		static MusicManager theSingleInstance;
		return theSingleInstance;
	}

	std::string next;
	bool transition;

	void playMusic( const std::string& filename );
	void playMusicFast( const std::string& filename );
	void update(sf::Time time)
	{
		if (music.getStatus() != sf::Music::Playing)
		{
			music.openFromFile(next);
			music.play();
			transition = false;
		}
		if (music.getStatus() == sf::Music::Playing && music.getVolume() > 1 && transition)
		{
			music.setVolume(music.getVolume() - 1);
		}
		if (music.getStatus() == sf::Music::Playing && music.getVolume() <= 1 && transition)
		{
			music.openFromFile(next);
			music.play();
			
			transition = false;
		}	
		if (!transition && music.getVolume() < 70)
		{
			music.setVolume(music.getVolume() + 1);
		}
	};

private:
	MusicManager(){};
	MusicManager( const MusicManager& );
	MusicManager& operator =( const MusicManager& );

	sf::Music music;
};

#endif