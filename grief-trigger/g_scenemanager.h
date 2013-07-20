#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include <MapLoader.h>

#include "d_dialogueinfo.h"
#include "g_sceneobject.h"
#include "g_effectmanager.h"
#include "i_dialoguepanel.h"
#include "g_playerobject.h"

class Scene
{
private:
	//Player object
	PlayerObject po;

	//Pointer to map loader
	tmx::MapLoader *map;

	//Shaders
	EffectManager ef;

	//XML Parser
	DialogueInfo di;

	//Dialogue panel
	DialoguePanel dp;

	//Render all to final texture
	sf::RenderTexture finalTexture;

	//Pointers to views
	sf::View *camera;
	sf::View *unscalable;

public:
	Scene();
	void init(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml);
	void update(sf::Time &time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
};

class SceneManager
{
private:
	//Current scene pointer
	Scene current;
public:
	SceneManager(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml);
	void draw(sf::RenderTarget &rt);
	void update(sf::Time &time);
	void input(sf::Event &event);
};

