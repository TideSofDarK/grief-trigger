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
#include "g_battle.h"
#include "h_config.h"

class Scene
{
private:
	//Battle screen
	Battle battle;

	//Is battle
	bool isBattle;

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
	
	//Is paused
	bool paused;

public:
	Scene();
	void init(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml);
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	void startBattle(Squad squad);
	void setPaused(bool p) {paused = p;};
	void setCurrentEffect(std::string string, sf::Time time);
};

class SceneManager
{
public:
	static SceneManager& instance()
	{
		static SceneManager theSingleInstance;
		return theSingleInstance;
	}

private:
	SceneManager() {
		camera = sf::View(sf::FloatRect(sf::FloatRect(0, 0, WIDTH, HEIGHT)));
		unscalable = sf::View(sf::FloatRect(0, 0, WIDTH, HEIGHT));
		camera.zoom(1.f/2.f);
	};

	SceneManager( const SceneManager& );
	SceneManager& operator =( const SceneManager& );

	//Current scene pointer
	Scene current;

	sf::View camera;
	sf::View unscalable;

public:
	void draw(sf::RenderTarget &rt);
	void update(sf::Time time);
	void input(sf::Event &event);
	void setScene(std::string name, tmx::MapLoader &ml);
	void initBattle(Squad &squad);
};

#endif
