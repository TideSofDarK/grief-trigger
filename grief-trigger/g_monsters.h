#ifndef MONSTERS_INCLUDE
#define MONSTERS_INCLUDE

#include <vector>
#include <string>
#include <cmath>

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

public:
	Monster(std::string name);
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

public:
	Squad();
	void init(std::string name, sf::Vector2f pos);
	std::vector<Monster> getMonsters() {return monsters;};
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time, std::vector<tmx::MapObject> &objects);
	bool isWalking() {return walking;};
	void step(int dir, std::vector<tmx::MapObject> &objects);
};

#endif