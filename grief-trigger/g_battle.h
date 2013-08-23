#ifndef BATTLE_INCLUDE
#define BATTLE_INCLUDE

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

#include <THOR/Particles.hpp>

#include <pugixml/pugixml.hpp>

#include "os_dbtweener.h"

#include "g_monsters.h"
#include "d_gamedata.h"
#include "i_battleui.h"

typedef enum { ATTACKED, NOT_ATTACKED } ENEMY_STATE;

class Enemy
{
private:
	ENEMY_STATE				state;

	//Ref to monster
	Monster					&monster;

	//Sprite
	sf::Sprite				sprite;

	//Hp bar
	Bar						hpBar;

	//Animation
	unsigned int			counter;
	bool					fading;
	sf::Clock				clock;
	bool					animation;

	//Movement bounds
	sf::FloatRect			bounds;

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
	bool isAnimation() {return animation;};
	bool isDied(){return monster.isDied();};
	const sf::Texture *getTexture(){return sprite.getTexture();};
};

class Battle
{
private:
	//Current state
	typedef enum { PLAYER, AI, SPELL, QTE, ENDED, HEROSELECT, ITEM_MENU } BATTLE_STATE;
	BATTLE_STATE				state;
	unsigned int				turnNumber;
	int							currentAttacking;

	sf::Clock clock;

	//Damage effect
	std::deque<Damage>			damageEffects;

	//Enemies on screen
	std::deque<Enemy>			enemies;

	//Number of selected enemy
	unsigned int				selected;

	//Resources loader
	pugi::xml_document			doc;

	//Top panel's back
	sf::Sprite					battleBox;

	//Particles
	Effects						effects;

	ItemMenu					itemMenu;

	int							itemSelected;

	bool						lastBoss;
	
	//Faces
	//Nyashki
	sf::Sprite					emberSprite;
	sf::Sprite					thunderSprite;
	sf::Sprite					playerSprite;

	//Spizhenniy background
	static const int fireType = 0;
	static const int spaceType = 1;
	static const int sbackType = 2;
	static const int lightType = 3;
	static const int creoType = 4;
	static const int shardsType = 5;
	static const int hellType = 6;
	static const int mechType = 7;
	unsigned int				currentBackground;
	sf::Shader					fire;
	sf::Shader					space;
	sf::Shader					sback;
	sf::Shader					light;
	sf::Shader					creo;
	sf::Shader					hell;
	sf::Shader					mech;
	sf::Shader					shards;
	unsigned int				seconds;
	sf::Texture					background;
	sf::Texture					background2;
	sf::Texture					background3;

	unsigned int				selectedHero;

	//HP Bars
	Bar							emberHPBar;
	Bar							thunderHPBar;
	Bar							playerHPBar;

	//Mana Bars
	Bar							emberManaBar;
	Bar							thunderManaBar;
	Bar							playerManaBar;

	//Tweener
	CDBTweener					oTweener;

	//Battle logger
	Logger						log;

	//Appearing menu
	Menu menu;

	//Pointer to hero
	sf::Sprite					pointer;

	//Shadow effect
	sf::RectangleShape			effectRect;

	//Hit effect
	HitEffect					he;

	//Battle result
	typedef struct BattleResult
	{
		unsigned int playerXP;
		unsigned int emberXP;
		unsigned int thunderXP;
	};
	BattleResult				res;

	//Spell menu
	SpellMenu					spellMenu;

	//Spell QTE system
	SpellQTE					spellQTE;

	sf::Sprite					advantage;
	float nax;
	bool						aEffect;
	sf::Clock					t;
	bool						check;

public:
	Battle();
	void loadResources();
	void start(std::vector<Monster> &ms, bool p);
	void draw(sf::RenderTarget &tg);
	void drawUI(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
	void damagePlayer(Monster &monster);
	void damageMonster();
	void nextAIStep();
	void nextPlayerStep();
	void damageSpell();
	void clean();
	void selectHero();
	void setW(){state = AI;};
};

#endif