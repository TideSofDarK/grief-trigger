#ifndef SCENEMANAGER_INCLUDE
#define SCENEMANAGER_INCLUDE

#include <thread>
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

typedef enum { MAP, BATTLE, TRANSITION, PAUSED, LOADING } SCENE_STATE;

class Scene
{
private:
	//Battle screen
	Battle				battle;

	//State
	SCENE_STATE			state;

	//Enemy squads
	std::vector<Squad>	squads;

	//The Doors
	std::vector<Door>	doors;

	//Player object
	PlayerObject		po;

	//Pointer to map loader
	tmx::MapLoader		*map;

	//Shaders
	ShaderManager		sm;

	//Particles manager
	ParticlesManager	pm;

	//Render all to final texture
	sf::RenderTexture	finalTexture;

	//Views
	sf::View			camera;
	sf::View			unscalable;

	//Loading text
	sf::Font			font;
	unsigned int		counter;
	sf::Text			loadingText;

	//Transition timer
	sf::Clock			transitionClock;

	//Squad to delete
	Squad				squadToDelete;
	unsigned int		toDelete;
	
	//Day counter
	sf::Sprite			days;

	//Loading resources
	void loadResources();

public:
	Scene()
	{
		camera = sf::View(sf::FloatRect(sf::FloatRect(0, 0, WIDTH, HEIGHT)));
		unscalable = sf::View(sf::FloatRect(0, 0, WIDTH, HEIGHT));
		camera.zoom(1.f/2.f);
	};
	void init(std::string name, tmx::MapLoader &ml);
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	void startBattle(Squad &squad);
	void endBattle();
	void setPaused(bool p) 
	{
		state = (p == true ? PAUSED : MAP);
	};
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
		
	};

	SceneManager( const SceneManager& );
	SceneManager& operator =( const SceneManager& );

	//Current scene pointer
	Scene		current;

	sf::View	camera;
	sf::View	unscalable;

public:
	void draw(sf::RenderTarget &rt);
	void update(sf::Time time);
	void input(sf::Event &event);
	void setScene(std::string name, tmx::MapLoader &ml);
	void initBattle(Squad &squad);
	void endBattle();
};

#endif
