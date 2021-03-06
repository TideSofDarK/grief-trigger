#ifndef MONSTERS_INCLUDE
#define MONSTERS_INCLUDE

#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include <Thor/Animation.hpp>

#include "os_animatedsprite.hpp"
#include "os_animation.hpp"

#include "MapLoader.h"

#include "d_basicstats.h"
#include "d_parser.h"
#include "h_config.h"
#include "d_resourcesmanager.h"
#include "i_battleui.h"

class Monster : BasicStats
{
private:
	//Name
	std::string name;
	
	typedef enum { ALIVE, DIED } MONSTER_STATE;

	MONSTER_STATE state;

	unsigned int maxHP;

public:
	Monster(std::string name);
	void receiveDamage(unsigned int damage) 
	{ 
		//got fucked with it
		int finally = hp - damage;
		std::cout << std::to_string(finally) << std::endl;
		if (finally <= 0)
		{
			SoundManager::instance().playDieSound();
			hp = 0;
			state = DIED;
			std::cout << "null" << std::endl;
		}
		else
		{
			hp -= damage;
			std::cout << "minus" << std::endl;
		}
	};
	bool isDied(){return (state == DIED ? true : false);};
	unsigned int &getStrength() {return strength;};
	unsigned int &getAgility() {return agility;};
	unsigned int &getIntelligence() {return intelligence;};
	unsigned int &getHP() {return hp;};
	unsigned int &getMaxHP(){return maxHP;};
	void setStrength(unsigned int newStrength) {strength = newStrength;};
	void setAgility(unsigned int newAgility) {agility = newAgility;};
	void setIntelligence(unsigned int newIntelligence) {intelligence = newIntelligence;};
	void setHP(unsigned int newHP) {hp = newHP;};
	std::string getName() {return name;};
};

class Squad
{
private:
	//Squad animations
	thor::FrameAnimation	animations[4];

	//Squad sprite itself
	thor::Animator<sf::Sprite, std::string> animator;
	sf::Sprite sprite;

	Animation				*animationA;
	AnimatedSprite			*spriteA;
	sf::Clock				aggroTimer;
	bool					aggr;

	//List of monsters
	std::vector<Monster>	monsters;

	//Moving direction
	int						direction;

	//Is walking
	bool					walking;

	//Next coord
	float					nextspot; 

	//Temp variables for smooth moving
	float					nx, ny;

	//Random movement counter
	unsigned int			counter;
	unsigned int			maxCounter;

	//Bounds of movement
	sf::FloatRect			bounds;
	sf::RectangleShape		sh;

	//Pointer to itself on map 
	tmx::MapObject			*object;

public:
	Squad();
	void init(std::string name, sf::Vector2f pos, tmx::MapObject& onMap);
	std::vector<Monster> &getMonsters() {return monsters;};
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	bool isWalking() {return walking;};
	void step(int dir);
	tmx::MapObject& getOnMap(){return *object;};
	//Some bydlocode
	//To delete squad after battle
	//Anyway different squads can't keep same bounds, �i�
	bool operator ==(const Squad &a)
	{
		return bounds==a.bounds;
	}
};

#endif