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

class MoneyEffect : Object
{
private:
	//Portal sprite
	AnimatedSprite	*sprite;
	Animation		*anim;

	sf::Vector2f sPos;

	bool visible;

public:
	MoneyEffect(sf::Vector2f pos);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
};

class ShopCrystal : Object
{
private:
	//Portal sprite
	AnimatedSprite	*sprite;
	Animation		*anim;

	//Pointer to object on map
	tmx::MapObject	*object;

	bool falling;

	sf::Vector2f sPos;

public:
	ShopCrystal();
	void init(sf::Vector2f pos, tmx::MapObject &object);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	tmx::MapObject& getOnMap(){return *object;};
};

class Portal : Object
{
private:
	//Portal sprite
	AnimatedSprite	*sprite;
	Animation		*anim;

	//Pointer to object on map
	tmx::MapObject	*object;

	//State
	bool			state;

	bool falling;

	sf::Vector2f sPos;

public:
	Portal(bool type);
	void init(sf::Vector2f pos, tmx::MapObject &object);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	tmx::MapObject& getOnMap(){return *object;};
	bool getType(){return state;};
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

class Chest : Object
{
private:
	//Chest sprite
	AnimatedSprite	*sprite;
	Animation		*anim;
	Animation		*idle;

	//Is opened
	bool			opened;

	//Pointer to object on map
	tmx::MapObject	*object;

public:
	Chest();
	void init(sf::Vector2f pos, tmx::MapObject &object);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
	void open();
	bool isOpened();
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

class Column : Object
{
private:
	sf::Sprite spriteUpper;
	sf::Sprite sprite;
	//Pointer to object on map
	tmx::MapObject	*object;
public:
	Column();
	void init(sf::Vector2f pos, tmx::MapObject &object);
	void draw(sf::RenderTarget &tg);
	void drawUpper(sf::RenderTarget &tg);
	void update(sf::Time time);
	tmx::MapObject& getOnMap();
};

#endif