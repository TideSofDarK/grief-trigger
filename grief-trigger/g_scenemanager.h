#ifndef SCENEMANAGER_INCLUDE
#define SCENEMANAGER_INCLUDE

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include <MapLoader.h>

#include "d_dialogueinfo.h"
#include "g_shadermanager.h"
#include "i_dialoguepanel.h"
#include "g_playerobject.h"
#include "g_particlesmanager.h"

class Scene
{
private:
	//Player object
	PlayerObject po;

	//Pointer to map loader
	tmx::MapLoader *map;

	//Shaders
	ShaderManager ef;

	//XML Parser
	DialogueInfo di;

	//Dialogue panel
	DialoguePanel dp;

	//Particles manager
	ParticlesManager pm;

	//Render all to final texture
	sf::RenderTexture finalTexture;

	//Pointers to views
	sf::View *camera;
	sf::View *unscalable;

public:
	Scene();
	void init(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml);
	void update(sf::Time time);
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
	void update(sf::Time time);
	void input(sf::Event &event);
	void setScene(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml);
};

#endif
