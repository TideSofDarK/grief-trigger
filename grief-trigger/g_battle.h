#ifndef BATTLE_INCLUDE
#define BATTLE_INCLUDE

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include <pugixml/pugiconfig.hpp>

#include "g_monsters.h"

class Bar
{
private:
public:
	Bar();
};

class Battle
{
private:
	typedef enum { PLAYER, AI } BATTLE_STATE;

	BATTLE_STATE state;

	Squad squad;

	unsigned int selected;

	pugi::xml_document doc;

	std::vector<sf::Sprite> monsters;

public:
	Battle();
	void start(Squad &squad);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
	void loadResources(std::string fileName = "assets/resources.xml");
};

#endif