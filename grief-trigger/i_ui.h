#ifndef UI_INCLUDE
#define UI_INCLUDE

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <os_dbtweener.h>
#include <os_animatedsprite.hpp>

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

class Shop
{
private:
	class ShopItem
	{
	private:
		sf::Sprite		sprite;
		sf::Text		text;
		sf::Text cost;
		unsigned int c;
		std::wstring n;

	public:
		ShopItem(std::wstring name);
		void init();
		void update(bool selected)
		{
			if (selected)
			{
				text.setColor(sf::Color::Black);
				cost.setColor(sf::Color::Black);
			}
			else
			{
				text.setColor(sf::Color::White);
				cost.setColor(sf::Color::White);
			}
		};
		void draw(sf::RenderTarget &tg)
		{
			text.setPosition(sprite.getPosition().x + (CHARACTER_SIZE * 3) - 2, sprite.getPosition().y + 10);
			cost.setPosition(text.getPosition());
			cost.move(-200,0);
			tg.draw(sprite);
			tg.draw(text);
			tg.draw(cost);
		};
		void setPosition(sf::Vector2f pos) {sprite.setPosition(pos);};
		sf::Vector2f getPosition() {return sprite.getPosition();};
		void move(sf::Vector2f m) {sprite.move(m);};
		sf::Sprite &getSprite(){return sprite;};
		void setColor(sf::Color c){sprite.setColor(c);};
		int getCost(){return c;};
	};

	sf::RectangleShape	horizontalPointer;
	sf::RectangleShape	verticalPointer;

	std::vector<ShopItem>	items;

	sf::RectangleShape blackRect;

	sf::Text		money;

	unsigned int selection;

	bool working;
	bool loaded;

public:
	Shop();
	void close();
	void show();
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
	bool isWorking(){return working;};
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
	void setTransparent(bool t){};
	sf::FloatRect &getBounds(){return portrait.getLocalBounds();};
};

class Days
{
private:
	sf::Text	dayText;
	sf::Sprite	back;
public:
	Days();
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
	void setCenter(float x, float y)
	{
		view.setCenter(sf::Vector2f(x, y));
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