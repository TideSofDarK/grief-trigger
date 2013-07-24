#ifndef SCENEMANAGER_INCLUDE
#define SCENEMANAGER_INCLUDE

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include <MapLoader.h>

#include "g_shadermanager.h"
#include "i_dialoguepanel.h"
#include "g_playerobject.h"
#include "g_particlesmanager.h"
#include "d_resourcesmanager.h"
#include "g_monsters.h"
#include "d_parser.h"
#include "d_objects.h"

class Scene
{
private:
	//Is loaded
	bool loaded;

	//Enemy squads
	std::vector<Squad> squads;

	//The Doors
	std::vector<Door> doors;

	//Player object
	PlayerObject po;

	//Pointer to map loader
	tmx::MapLoader *map;

	//Shaders
	ShaderManager sm;

	//Dialogue panel
	DialoguePanel dp;

	//Particles manager
	ParticlesManager pm;

	//Render all to final texture
	sf::RenderTexture finalTexture;

	//Pointers to views
	sf::View *camera;
	sf::View *unscalable;

	//Loading thread
	sf::Thread loadingThread;

	//Loading resources
	void loadResources();

	//Loading text
	sf::Font font;
	unsigned int counter;
	sf::Text loadingText;

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
