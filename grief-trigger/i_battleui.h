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

	class Item
	{
	private:
		sf::Sprite		sprite;
		sf::Text		text;
		Spell			spell;

	public:
		Item(Spell &newSpell)
		{
			text.setFont(DFont::instance().getFont());
			spell = newSpell;
			sprite.setTexture(TextureManager::instance().getTexture("assets/" + spell.getFileName() + ".png"));

			std::wstring string =  L", очков маны: " + std::to_wstring(spell.getMana());
			std::wstring string4 = spell.getName() + string;
			
			text.setString(string4);
			text.setColor(sf::Color::White);
		};
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
	};

	SPELLMENU_STATE		state;

	std::vector<Item>	spells;

	unsigned int		selection;
	unsigned int		selected;

	sf::Font			font;

	sf::RectangleShape	horizontalPointer;
	sf::RectangleShape	verticalPointer;

	CDBTweener			oTweener;
	float				tempSelectedX;

public:
	SpellMenu()
	{
		state = IDLE;
		font.loadFromFile(fontPath);
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
	typedef enum STATE { IDLE, WORKING, TRANSITION };
	typedef enum QTE_STATE { GOING, ENDED, SLEEP };

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

	static const unsigned int	startX = WIDTH / 3;
	static const unsigned int	startY = HEIGHT / 3;
	static const unsigned int	offset = 10;

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

public:
	SpellQTE();
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	void clean();
	void start(Spell &sp);
	bool isDamaged() {return damaged;};
	bool isEnded(){return (state == IDLE ? true : false);};
	void next(std::deque<Block>::iterator &it);
};

#endif