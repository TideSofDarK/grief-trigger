#ifndef PLAYEROBJECT_INCLUDE
#define PLAYEROBJECT_INCLUDE

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include <Thor/Animation.hpp>

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
	thor::FrameAnimation	animations[4];
	thor::FrameAnimation	idleAnimations[4];

	//Animated player sprite itself
	thor::Animator<sf::Sprite, std::string> animator;
	sf::Sprite sprite;

	//Moving direction
	int						direction;

	//Is walking
	bool					walking;

	//Next coord
	float					nextspot; 

	//Temp variables for smooth moving
	float					nx, ny;

	//Player object on map
	tmx::MapObject			*object;

public:
	PlayerObject();
	void init(sf::Vector2f pos, tmx::MapObject &playerObject);
	void update(sf::Time &time);
	void draw(sf::RenderTarget &tg);
	void play();
	sf::Sprite& getSprite();
	bool step(int dir);
	bool isWalking(){return walking;};
	tmx::MapObject &getOnMap(){return *object;};
	int getDirection(){return direction;};
};

#endif