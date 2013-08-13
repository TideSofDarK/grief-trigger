#ifndef OBJECTS_INCLUDE
#define OBJECTS_INCLUDE

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "os_animatedsprite.hpp"

#include <MapLoader.h>

class Object
{
public:
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
};

class NPC : Object
{
private:
	//NPC sprite
	sf::Sprite		sprite;

	//Pointer to object on map
	tmx::MapObject	*object;

public:
	NPC();
	void init(sf::Vector2f pos, tmx::MapObject &object);
	void draw(sf::RenderTarget &tg);
	tmx::MapObject& getOnMap();
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

//Simillar to hit effect
class Swing
{
private:
	bool			working;
	bool			played;
	Animation		animation;
	AnimatedSprite	sprite;

public:
	Swing();
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void init(sf::Vector2f pos);
	bool isWorking(){return working;};
};

#endif