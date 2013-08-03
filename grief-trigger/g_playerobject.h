#ifndef PLAYEROBJECT_INCLUDE
#define PLAYEROBJECT_INCLUDE

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "os_dbtweener.h"

#include "MapLoader.h"

#include "os_animation.hpp"
#include "os_animatedsprite.hpp"

#include "i_dialoguepanel.h"
#include "h_config.h"
#include "d_resourcesmanager.h"
#include "d_objects.h"
#include "g_monsters.h"

class PlayerObject
{
private:
	//Player animations
	Animation			walkingAnimation[4];

	//Animated player sprite itself
	AnimatedSprite		animatedSprite;

	//Moving direction
	int					direction;

	//Is walking
	bool				walking;

	//Next coord
	int					nextspot; 

	//Start camera variables
	sf::Vector2f		camStart;

	//Temp variables for smooth moving
	int					nx, ny;

	//Camera tweener
	CDBTweener			oTweener;
	float				ncx, ncy;

	//Player object on map
	tmx::MapObject		*object;

	//Pointer to sqauds
	std::vector<Squad>	*squads;

public:
	PlayerObject();
	void init(sf::Uint32 x, sf::Uint32 y, sf::Vector2f cameraStart, tmx::MapObject &playerObject, std::vector<Squad> &squadsList);
	void update(sf::Time &time, sf::View &camera);
	void draw(sf::RenderTarget &tg);
	void move();
	void play();
	AnimatedSprite& getSprite();
	bool step(int dir);
	void input(sf::Event &event);
	bool isWalking(){return walking;};
};

#endif