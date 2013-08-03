#ifndef UI_INCLUDE
#define UI_INCLUDE

#include <string>

#include <SFML/Graphics.hpp>

#include "d_resourcesmanager.h"

class Tip
{
private:
	sf::Sprite	sprite;

	sf::Text	text;

	unsigned int key;

	std::string type;

public:
	Tip(std::wstring string, unsigned int key, std::string newType);
	sf::Sprite &getSprite(){return sprite;};
	void draw(sf::RenderTarget &tg, sf::Vector2f pos);
	unsigned int &getKey(){return key;};
	std::string getType(){return type;};
};

class XPBar
{
private:
	sf::Sprite portrait;

	sf::Sprite xpbar;

public:
	XPBar();
	void draw(sf::RenderTarget &tg);
};

#endif