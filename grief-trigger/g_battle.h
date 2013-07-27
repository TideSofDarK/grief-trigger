#ifndef BATTLE_INCLUDE
#define BATTLE_INCLUDE

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

#include <pugixml/pugiconfig.hpp>

#include "os_dbtweener.h"

#include "g_monsters.h"
#include "d_gamedata.h"

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

class Battle
{
private:
	//Current state
	typedef enum { PLAYER, AI } BATTLE_STATE;
	BATTLE_STATE				state;
	unsigned int turnNumber;

	//Font
	sf::Font					font;
	
	//Text shader
	sf::Shader textShader;

	//Damage effect
	std::vector<Damage>			damageEffects;

	//Enemy squad
	Squad						squad;

	//Number of selected enemy
	unsigned int				selected;

	//Resources loader
	pugi::xml_document			doc;

	//List of monsters on screen
	std::vector<sf::Sprite>		monsters;

	//Selection animation
	unsigned int				counter;
	bool						fading;

	//Top panel's back
	sf::Sprite					battleBox;
	
	//Faces
	//Nyashki
	sf::Sprite					emberSprite;
	sf::Sprite					thunderSprite;
	sf::Sprite					playerSprite;

	//Spizhenniy background
	sf::Shader					fire;
	unsigned int				seconds;
	sf::Texture					background;

	//HP Bars
	Bar emberHPBar;
	Bar thunderHPBar;
	Bar playerHPBar;

	//Mana Bars
	Bar emberManaBar;
	Bar thunderManaBar;
	Bar playerManaBar;

	//Enemy bars
	std::list<Bar> enemyBars;

	//Tweener
	CDBTweener oTweener;

public:
	Battle();
	void start(Squad &squad);
	void draw(sf::RenderTarget &tg);
	void drawUI(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
	void loadResources(std::string fileName = "assets/resources.xml");
};

#endif