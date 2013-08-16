#ifndef BATTLEUI_INCLUDE
#define BATTLEUI_INCLUDE

#include <list>
#include <iostream>
#include <deque>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Thor/Resources.hpp>
#include <Thor/Graphics.hpp>

#include <pugixml/pugixml.hpp>

#include "os_dbtweener.h"
#include "os_animatedsprite.hpp"
#include "os_animation.hpp"

#include "h_config.h"
#include "d_resourcesmanager.h"
#include "d_magic.h"

#define SHAPE_WIDTH 180.0
#define BAR_WIDTH 170.0

#define SHAPE_HEIGHT 18.0
#define BAR_HEIGHT 8.0

#define OFFSET 5.0

#define ANIMATIONTIME 1.0

#define NOT_SELECTED -1

#define YELLOW sf::Color(249u,217u,35u,255u)
#define BLUE sf::Color(1u,250u,254u,255u)

#define QTE_FACTOR 7.f

float interpolateLinear(
	const float pointA,
	const float pointB,
	float factor
	);

//Similar to DialoguePanel but simpler
class Logger
{
private:
	std::wstring		actualString;
	unsigned int		character;
	sf::Text			text;
	bool				ended;
	bool				read;
	sf::Clock			clock;

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

	float		ny;
	float		na;
	float		dirY;

	//Is active
	bool		active;

public:
	Damage(sf::Vector2f pos, std::string str);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	bool isActive() {return active;};
	void changeActive() {active = !active;};
};

class Bar
{
private:
	unsigned int		*variable;
	unsigned int		maxVariable;

	sf::RectangleShape	shape;
	sf::RectangleShape	bar;

	bool				vertical;

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
	void clean() {selected = NOT_SELECTED;};
};

class SpellMenu
{
private:
	typedef enum SPELLMENU_STATE { IDLE, WORKING, TRANSITION };

	sf::Text		descr;

	class Item
	{
	private:
		sf::Sprite		sprite;
		sf::Text		text;
		Spell			spell;

	public:
		Item(Spell &newSpell);
		void update(bool selected)
		{
			if (selected)
			{
				text.setColor(sf::Color::Black);
			}
			else
			{
				text.setColor(sf::Color::White);
			}
		};
		void draw(sf::RenderTarget &tg)
		{
			text.setPosition(sprite.getPosition().x + (CHARACTER_SIZE * 3) - 2, sprite.getPosition().y + 10);
			tg.draw(sprite);
			tg.draw(text);
		};
		void setPosition(sf::Vector2f pos) {sprite.setPosition(pos);};
		sf::Vector2f getPosition() {return sprite.getPosition();};
		void move(sf::Vector2f m) {sprite.move(m);};
		Spell &getSpell(){return spell;};
		sf::Sprite &getSprite(){return sprite;};
		void setColor(sf::Color c){sprite.setColor(c);};
	};

	SPELLMENU_STATE		state;

	std::vector<Item>	spells;

	unsigned int		selection;
	unsigned int		selected;

	sf::RectangleShape	horizontalPointer;
	sf::RectangleShape	verticalPointer;

	CDBTweener			oTweener;
	float				tempSelectedX;

public:
	SpellMenu()
	{
		descr.setFont(DFont::instance().getFont());
		descr.setColor(sf::Color::White);
		state = IDLE;
		horizontalPointer = sf::RectangleShape(sf::Vector2f(WIDTH, CHARACTER_SIZE));
		verticalPointer = sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, HEIGHT));
	};
	void setSpells(std::vector<Spell> newSpells);
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	int getSelected(){return selected;};
	Spell &getSelectedSpell() {return spells[selected].getSpell();};
	bool isWorking()
	{
		return (state == WORKING ? true : false);
	};
	void close()
	{
		state = IDLE;
	};
	void select();
};

class SpellQTE
{
private:
	typedef enum STATE { PREPARE, IDLE, WORKING, TRANSITION };
	typedef enum QTE_STATE { GOING, ENDED, SLEEP, NEXT };

	typedef struct Block
	{
		void draw(sf::RenderTarget &tg)
		{
			tg.draw(sprite);
		};
		sf::Sprite sprite;
		int b;
		QTE_STATE state;
	};

	class EText
	{
	private:
		sf::Sprite		sprite;
		unsigned int	counter;
		bool			appearing;
		bool			working;

	public:
		EText(std::string type)
		{
			sprite.setTexture(TextureManager::instance().getTexture("assets/"+type+".png"));
			sprite.setColor(sf::Color(255,255,255,1));
			appearing = true;
			working = true;
			sprite.setPosition(HALF_WIDTH - (sprite.getLocalBounds().width / 2), HEIGHT/6);
			if (type != "prepare")
			{
				sprite.move(-15 + (rand() % (int)(30 + 1)), -40 + (rand() % (int)(80 + 1)));
				//sprite.scale((6 + (rand() % (int)(10 - 6 + 1))) / 10, (6 + (rand() % (int)(10 - 6 + 1))) / 10);
				sprite.move(100,0);
			}		
			else
			{
				sprite.move((sprite.getLocalBounds().width / 3),-45);
			}
		};
		void draw(sf::RenderTarget &tg)
		{
			if (working)
			{
				tg.draw(sprite);
			}	
		};
		void update(sf::Time time)
		{
			if (working)
			{
				if (appearing) 
				{
					sprite.setColor(sf::Color(255,255,255,interpolateLinear(sprite.getColor().a, 255, 0.1)));
					sprite.setPosition(interpolateLinear(sprite.getPosition().x, sprite.getPosition().x - 50, 0.1), sprite.getPosition().y);
				}
				else
				{
					sprite.setColor(sf::Color(255,255,255,interpolateLinear(sprite.getColor().a, 0, 0.1)));
					sprite.setPosition(interpolateLinear(sprite.getPosition().x, sprite.getPosition().x - 50, 0.1), sprite.getPosition().y);
				}

				if (sprite.getColor().a >= 246) //magic number, fuck interpolation
				{
					appearing = false;
				}
				if (sprite.getColor().a == 0)
				{
					working = false;
					//appearing = true;
				}
			}
		};
		bool isWorking(){return working;};
	};

	static const unsigned int	startX = WIDTH / 3;
	static const unsigned int	startY = HEIGHT / 3;
	static const unsigned int	offset = 10;
	static const bool			type = 1;

	std::vector<EText>			etexts;

	Spell						spell;

	STATE						state;

	std::deque<Block>			blocks;

	sf::Sprite					line;

	sf::RectangleShape			shape;

	sf::Sprite					cell;

	sf::Clock					timer;
	bool						waiting;

	sf::Clock					appearingTimer;

	bool						damaged;

	sf::Sprite					prepare[3];
	unsigned int				hero;

public:
	SpellQTE();
	void loadResources();
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	void clean();
	void start(Spell &sp, std::string name);
	bool isDamaged() {return damaged;};
	bool isEnded(){return (state == IDLE ? true : false);};
	void next(std::deque<Block>::iterator &it);
};

class HitEffect
{
private:
	bool			working;
	Animation		animation;
	AnimatedSprite	sprite;

public:
	HitEffect();
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void init(sf::Vector2f pos);
};

class Result
{
private:
	sf::Sprite		back;
	sf::Text		stats[5];
	int				current;

public:
	Result();
	void loadResources();
	void show();
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	void next();
	void reset(){current = 0;};
};

#endif