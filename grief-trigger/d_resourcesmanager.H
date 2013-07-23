#ifndef RESOURCESMANAGER_INCLUDE
#define RESOURCESMANAGER_INCLUDE

#include <string>
#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourcesManager
{
public:
	ResourcesManager() {};

private:
	ResourcesManager( const ResourcesManager& );
	ResourcesManager& operator =( const ResourcesManager& );

public:
	const sf::Texture&	getTexture( const std::string& filename );
	void deleteTexture( const sf::Texture& image );
	void deleteTexture( const std::string& filename );

private:
	std::map< std::string, sf::Texture > textures;
};

#endif