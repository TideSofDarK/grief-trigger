#ifndef BATTLEUI_INCLUDE
#define BATTLEUI_INCLUDE

#include <list>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Thor/Resources.hpp>
#include <Thor/Graphics.hpp>

#include <pugixml/pugixml.hpp>

#include "os_dbtweener.h"

#include "h_config.h"
#include "d_resourcesmanager.h"

#define SHAPE_WIDTH 180.0
#define BAR_WIDTH 170.0

#define SHAPE_HEIGHT 18.0
#define BAR_HEIGHT 8.0

#define OFFSET 5.0

#define ANIMATIONTIME 1.0

#define NOT_SELECTED -1

//Similar to DialoguePanel but simpler
class Logger
{
private:
	std::wstring		actualString;
	unsigned int	character;
	sf::Text		text;
	sf::Font		font;
	bool			ended;
	bool			read;
	sf::Clock		clock;

public:
	Logger();
	void init(sf::Vector2f pos);
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	void stop();
	void setString(std::wstring str);
	bool isEnded() {return ended;};
	bool isRead() {return read;};
};

class Damage
{
private:
	sf::Text	text;

	sf::Font	font;

	float		ny;
	float		na;
	float		dirY;

	//Is active
	bool active;

public:
	Damage(sf::Vector2f pos, std::string str, sf::Font &font);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	bool isActive() {return active;};
	void changeActive() {active = !active;};
};

class Bar
{
private:
	unsigned int *variable;
	unsigned int maxVariable;

	sf::RectangleShape shape;
	sf::RectangleShape bar;

	bool vertical;

public:
	Bar();
	void init(bool vert, sf::Vector2f pos, unsigned int &var, sf::Color color = sf::Color(145, 199, 105));
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time, sf::Vector2f np = sf::Vector2f());
};

class Menu
{
private:
	static const unsigned int	appearingSpeed = 2;
	bool						working;
	bool						initialized;
	std::vector<sf::Text>		items;
	unsigned int				selection;
	sf::RectangleShape			selector;
	float						nx;
	float						counter;
	sf::Sprite					back;
	sf::Font					font;
	int							selected;

public:
	Menu(void);
	void init(std::vector<std::wstring> newItems);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
	void appear(sf::Vector2f pos);
	void disappear();
	bool isWorking() {return working;};
	bool isInitialized() {return initialized;};
	void select();
	unsigned int getSelected(){return selected;};
};

#endif