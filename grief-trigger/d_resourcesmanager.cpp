#include "d_resourcesmanager.h"

const sf::Texture& ResourcesManager::getTexture( const std::string& filename )
{
	// Check, whether the image already exists
	for( std::map<std::string, sf::Texture>::const_iterator it = textures.begin();
		it != textures.end(); 
		++it)
	{
		if( filename == it->first )
		{
			std::cout << "DEBUG_MESSAGE: using existing image.\n";
			return it->second;
		}
	}

	// The image doesn't exists. Create it and save it.
	sf::Texture image;
	if( image.loadFromFile( filename ) )
	{
		textures[filename] = image;
		std::cout << "DEBUG_MESSAGE: loading image.\n";
		return textures[filename];
	}

	std::cout << "GAME_ERROR: Image was not found. It is filled with an empty image.\n";
	textures[filename] = image;
	return textures[filename];
}

void ResourcesManager::deleteTexture( const sf::Texture& image )
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

void ResourcesManager::deleteTexture( const std::string& filename )
{
	std::map<std::string, sf::Texture>::const_iterator it = textures.find( filename );
	if( it != textures.end() )
		textures.erase( it );
}