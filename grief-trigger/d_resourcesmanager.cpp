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

	chestBuffer.loadFromFile("assets/chest.wav");
	chestSound.setBuffer(chestBuffer);

	step1Buffer.loadFromFile("assets/step1.wav");
	step1Sound.setBuffer(step1Buffer);

	step2Buffer.loadFromFile("assets/step2.wav");
	step2Sound.setBuffer(step2Buffer);

	step3Buffer.loadFromFile("assets/step3.wav");
	step3Sound.setBuffer(step3Buffer);

	step4Buffer.loadFromFile("assets/step4.wav");
	step4Sound.setBuffer(step4Buffer);

	noiseBuffer.loadFromFile("assets/noise.wav");
	noiseSound.setBuffer(noiseBuffer);

	pressBuffer.loadFromFile("assets/press.wav");
	pressSound.setBuffer(pressBuffer);

	successBuffer.loadFromFile("assets/success.wav");
	successSound.setBuffer(successBuffer);

	hitBuffer.loadFromFile("assets/shit.wav");
	hitSound.setBuffer(hitBuffer);

	critBuffer.loadFromFile("assets/crit.wav");
	critSound.setBuffer(critBuffer);
}

void SoundManager::playCritSound()
{
	critSound.play();
}

void SoundManager::playHitSound()
{
	hitSound.play();
}

void SoundManager::playPressSound()
{
	pressSound.play();
}

void SoundManager::playSuccessSound()
{
	successSound.play();
}

void SoundManager::playNoiseSound()
{
	noiseSound.play();
}

void SoundManager::playChestSound()
{
	chestSound.play();
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

void SoundManager::playStep2Sound()
{
	step2Sound.play(); 
}

void SoundManager::playStep1Sound()
{
	if (step1Sound.getStatus() != sf::Sound::Status::Playing && step2Sound.getStatus() != sf::Sound::Status::Playing) 
	{
		if (!lastStep) 
		{
			step1Sound.play();
			lastStep = true;
		}
		else
		{
			playStep2Sound();
			lastStep = false;
		}
	}
}
void SoundManager::playStep3Sound()
{
	if (step3Sound.getStatus() != sf::Sound::Status::Playing) step3Sound.play();
}

void SoundManager::playStep4Sound()
{
	if (step4Sound.getStatus() != sf::Sound::Status::Playing) step4Sound.play();
}

void MusicManager::playMusic( const std::string& filename )
{
	if (filename != "")
	{
		next = "assets/" + filename + ".ogg";
		music.setLoop(true);
		transition = true;
	}
}

void MusicManager::playMusicFast( const std::string& filename )
{
	if (filename != "")
	{
		transition = false;
		sf::Time t = music.getPlayingOffset();
		music.openFromFile("assets/" + filename + ".ogg");
		music.play();
		music.setPlayingOffset(t);
		music.setVolume(70u);
		music.setLoop(true);
	}
}