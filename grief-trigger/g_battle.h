#ifndef BATTLE_INCLUDE
#define BATTLE_INCLUDE

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

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
	typedef enum { PLAYER, AI, SPELL, QTE, ENDED } BATTLE_STATE;
	BATTLE_STATE				state;
	unsigned int turnNumber;
	unsigned int currentAttacking;

	sf::Clock clock;

	//Damage effect
	std::vector<Damage>			damageEffects;

	//Enemies on screen
	std::vector<Enemy>			enemies;

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
	static const int fireType = 0;
	static const int spaceType = 1;
	unsigned int				currentBackground;
	sf::Shader					fire;
	sf::Shader					space;
	unsigned int				seconds;
	sf::Texture					background;
	sf::Texture					background2;
	sf::Texture					background3;

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

	//Hit effect list
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

public:
	Battle();
	void loadResources();
	void start(Squad &squad);
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
};

#endif