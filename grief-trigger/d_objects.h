#ifndef OBJECTS_INCLUDE
#define OBJECTS_INCLUDE

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <MapLoader.h>

class Object
{
public:
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
};

class Door : Object
{
private:
	//Door sprite
	sf::Sprite		sprite;

	//Is opened
	bool			opened;

	//Pointer to object on map
	tmx::MapObject	*object;

public:
	Door();
	void init(sf::Vector2f pos, tmx::MapObject &object);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
	void open();
	bool isOpened();
	tmx::MapObject& getOnMap();
};

#endif