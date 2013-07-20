#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "MapLoader.h"

#include "Animation.hpp"
#include "AnimatedSprite.hpp"

#include "g_sceneobject.h"
#include "i_dialoguepanel.h"

static const sf::Uint32 DIR_IDLE = -1;
static const sf::Uint32 DIR_TOP = 0;
static const sf::Uint32 DIR_RIGHT = 1;
static const sf::Uint32 DIR_LEFT = 2;
static const sf::Uint32 DIR_DOWN = 3;

class PlayerObject : SceneObject
{
	Animation walkingAnimation[4];

private:
	AnimatedSprite animatedSprite;
	int direction;
	bool walking;
	int nextspot; 
	int nx, ny;

public:
	PlayerObject();
	void init(sf::Uint32 x, sf::Uint32 y);
	void update(sf::Time &time, sf::View &camera);
	void draw(sf::RenderTarget &tg);
	void move(std::vector<tmx::MapObject> &objects, DialoguePanel &at);
	void play();
	AnimatedSprite& getSprite();
};

