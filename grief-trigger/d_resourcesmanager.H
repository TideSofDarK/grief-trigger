#ifndef RESOURCESMANAGER_INCLUDE
#define RESOURCESMANAGER_INCLUDE

#include <memory>
#include <mutex>
#include <string>
#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourcesManager
{
public:
	static ResourcesManager& instance()
	{
		static ResourcesManager theSingleInstance;
		return theSingleInstance;
	}

private:
	ResourcesManager() {};
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