#include "d_resourcesmanager.h"

const sf::Texture& TextureManager::getTexture( const std::string& filename )
{
	// Check, whether the image already exists
	for( std::map<std::string, sf::Texture>::const_iterator it = textures.begin();
		it != textures.end(); 
		++it)
	{
		if( filename == it->first )
		{
			return it->second;
		}
	}

	// The image doesn't exists. Create it and save it.
	sf::Texture image;
	if( image.loadFromFile( filename ) )
	{
		textures[filename] = image;
		return textures[filename];
	}

	textures[filename] = image;
	return textures[filename];
}

void TextureManager::deleteTexture( const sf::Texture& image )
{
	for( std::map<std::string, sf::Texture>::const_iterator it = textures.begin();
		it != textures.end(); 
		++it)
	{
		// compare the adresses
		if( &image == &it->second )
		{
			textures.erase( it );
			return;
		}
	}
}

void TextureManager::deleteTexture( const std::string& filename )
{
	std::map<std::string, sf::Texture>::const_iterator it = textures.find( filename );
	if( it != textures.end() )
		textures.erase( it );
}

SoundManager::SoundManager()
{
	doorBuffer.loadFromFile("assets/door.wav");
	doorSound.setBuffer(doorBuffer);

	clickBuffer.loadFromFile("assets/typewriter.wav");
	clickSound.setBuffer(clickBuffer);

	enterBuffer.loadFromFile("assets/enter.wav");
	enterSound.setBuffer(enterBuffer);

	selectBuffer.loadFromFile("assets/select.wav");
	selectSound.setBuffer(enterBuffer);

	hurtBuffer.loadFromFile("assets/hurt.wav");
	hurtSound.setBuffer(hurtBuffer);

	dieBuffer.loadFromFile("assets/die.wav");
	dieSound.setBuffer(dieBuffer);

	winBuffer.loadFromFile("assets/win.wav");
	winSound.setBuffer(winBuffer);

	failBuffer.loadFromFile("assets/fail.wav");
	failSound.setBuffer(failBuffer);

	transitionBuffer.loadFromFile("assets/trans.wav");
	transitionSound.setBuffer(transitionBuffer);
}

void SoundManager::playDoorSound()
{
	doorSound.play();
}

void SoundManager::playClickSound()
{
	clickSound.play();
}

void SoundManager::playEnterSound()
{
	enterSound.play();
}

void SoundManager::playSelectSound()
{
	selectSound.play();
}

void SoundManager::playHurtSound()
{
	hurtSound.play();
}

void SoundManager::playDieSound()
{
	dieSound.play();
}

void SoundManager::playWinSound()
{
	winSound.play();
}

void SoundManager::playFailSound()
{
	failSound.play();
}

void SoundManager::playTransitionSound()
{
	transitionSound.play();
}