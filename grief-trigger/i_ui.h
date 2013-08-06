#ifndef UI_INCLUDE
#define UI_INCLUDE

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <os_dbtweener.h>

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
	void update(sf::Time time);
};

class MapScrollingListener: public CDBTweener::IListener
{
public:
	bool &moving;
	MapScrollingListener(bool &m) : moving(m)
	{
	}
	void onTweenFinished(CDBTweener::CTween *pTween)
	{
		moving = false;
	}
};

typedef struct Camera
{
	sf::View		view;
	Camera()
	{
		view = sf::View(sf::FloatRect(sf::FloatRect(0, 0, HALF_WIDTH, HALF_HEIGHT)));
	}
	void move(float x, float y)
	{
		view.move(x, y);
	}
	void setPosition(float x, float y)
	{
		view.setCenter(sf::Vector2f(x + (HALF_WIDTH/2), y + (HALF_HEIGHT / 2)));
	}
	sf::Vector2f getPosition()
	{
		return sf::Vector2f(view.getCenter().x - (HALF_WIDTH/2), view.getCenter().y - (HALF_HEIGHT / 2));
	}
	const sf::View &getView()
	{
		return view;
	}
};

#endif