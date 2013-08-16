#ifndef UI_INCLUDE
#define UI_INCLUDE

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <os_dbtweener.h>

#include "d_resourcesmanager.h"

typedef enum STATS_STATE{FALLING, DIS, IDLE};

class StatsWindowListener: public CDBTweener::IListener
{
public:
	bool		&gomap;
	STATS_STATE	&state;
	StatsWindowListener(bool &m, STATS_STATE &newState) : gomap(m), state(newState)
	{
	}
	void onTweenFinished(CDBTweener::CTween *pTween)
	{
		gomap = true;
		state = IDLE;
	};
};

class StatsWindowListener2: public CDBTweener::IListener
{
public:
	STATS_STATE	&state;
	StatsWindowListener2(STATS_STATE &newState) : state(newState)
	{
	}
	void onTweenFinished(CDBTweener::CTween *pTween)
	{
		state = IDLE;
	};
};

class StatsWindow
{
private:
	STATS_STATE				state;
	sf::Sprite				back;
	sf::Sprite				speech;
	sf::Text				feeling;
	sf::String				actualString;
	unsigned int			character;
	sf::Clock				clock;
	sf::Sprite				heroStats[3];
	sf::Sprite				heroBars[2];
	sf::Text				stats[5];
	sf::Text				name;
	float					statsPos[5];
	int						current;
	CDBTweener				oTweener;
	StatsWindowListener		listener;
	StatsWindowListener2	listener2;
	float					newY;
	bool					goMap;

public:
	StatsWindow();
	void show();
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	void close();
	void next();
	void setText(std::wstring wstr);;
	std::wstring getFeeling();
};

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
	void setString(std::wstring newStr){text.setString(newStr);};
};

class XPBar
{
private:
	sf::Sprite portrait;

	sf::Sprite xpbar;

	sf::Text level;

	sf::String str;

public:
	XPBar();
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void setTransparent(bool t){if (t){portrait.setColor(sf::Color(255,255,255,125));}else {portrait.setColor(sf::Color(255,255,255,255));}};
	sf::FloatRect &getBounds(){return portrait.getLocalBounds();};
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
	sf::View &getView()
	{
		return view;
	}
};

#endif