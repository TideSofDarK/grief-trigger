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

//Similar to DialoguePanel but simpler
class Logger
{
private:
	std::string		actualString;
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
	void setString(std::string str);
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

typedef enum { ATTACKED, NOT_ATTACKED } ENEMY_STATE;

class Enemy
{
private:
	ENEMY_STATE state;

	Monster				&monster;
	sf::Sprite			sprite;
	Bar					hpBar;
	unsigned int		counter;
	bool				fading;
	sf::Clock clock;
	bool animation;

public:
	Enemy(sf::Vector2f pos, const sf::Texture &texture, Monster &monster);
	void draw(sf::RenderTarget &tg, bool selected = false);
	void drawUI(sf::RenderTarget &tg);
	void update(sf::Time time);
	sf::Vector2f getPosition() {return sprite.getPosition();};
	Monster &getMonster() {return monster;};
	void setState(ENEMY_STATE newState) {state = newState;};
	void playAnimation();
	void stopAnimation() {animation = false;};
};

class Battle
{
private:
	//Current state
	typedef enum { PLAYER, AI } BATTLE_STATE;
	BATTLE_STATE				state;
	unsigned int turnNumber;
	unsigned int currentAttacking;

	sf::Clock clock;

	//Font
	sf::Font					font;
	
	//Text shader
	sf::Shader textShader;

	//Damage effect
	std::vector<Damage>			damageEffects;

	//Enemies on screen
	std::vector<Enemy> enemies;

	//Enemy squad
	Squad						squad;

	//Number of selected enemy
	unsigned int				selected;

	//Resources loader
	pugi::xml_document			doc;

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

	//Tweener
	CDBTweener oTweener;

	//Battle logger
	Logger log;

public:
	Battle();
	void start(Squad &squad);
	void draw(sf::RenderTarget &tg);
	void drawUI(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
	void init(std::string fileName = "assets/resources.xml");
	void damagePlayer(Monster &monster);
	void nextStep();
};

#endif