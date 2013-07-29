#ifndef MONSTERS_INCLUDE
#define MONSTERS_INCLUDE

#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "MapLoader.h"
#include "os_animatedsprite.hpp"

#include "d_basicstats.h"
#include "d_parser.h"
#include "h_config.h"
#include "d_resourcesmanager.h"

class Monster : BasicStats
{
private:
	//Name
	std::string name;
	
	typedef enum { ALIVE, DIED } MONSTER_STATE;

	MONSTER_STATE state;

public:
	Monster(std::string name);
	void receiveDamage(unsigned int damage) 
	{ 
		//got fucked with it
		int finally = hp - damage;
		std::cout << std::to_string(finally) << std::endl;
		if (finally <= 0)
		{
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
	void setStrength(unsigned int newStrength) {strength = newStrength;};
	void setAgility(unsigned int newAgility) {agility = newAgility;};
	void setIntelligence(unsigned int newIntelligence) {intelligence = newIntelligence;};
	void setHP(unsigned int newHP) {hp = newHP;};
	std::string getName() {return name;};
};

class Squad
{
private:
	//Player animations
	Animation				walkingAnimation[4];

	//Animated player sprite itself
	AnimatedSprite			animatedSprite;

	//List of monsters
	std::vector<Monster>	monsters;

	//Moving direction
	int						direction;

	//Is walking
	bool					walking;

	//Next coord
	int						nextspot; 

	//Temp variables for smooth moving
	int						nx, ny;

	//Random movement counter
	unsigned int			counter;
	unsigned int			maxCounter;

	//Bounds of movement
	sf::FloatRect			bounds;
	sf::RectangleShape		sh;

	//Pointer to player object
	tmx::MapObject			*hero;

	//Pointer to itself on map 
	tmx::MapObject			*object;

public:
	Squad();
	void init(std::string name, sf::Vector2f pos, tmx::MapObject& onMap);
	std::vector<Monster> &getMonsters() {return monsters;};
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time, std::vector<tmx::MapObject> &objects);
	bool isWalking() {return walking;};
	void step(int dir, std::vector<tmx::MapObject> &objects);
	tmx::MapObject& getOnMap(){return *object;};
};

#endif