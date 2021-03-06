#ifndef SCENEMANAGER_INCLUDE
#define SCENEMANAGER_INCLUDE

#include <thread>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

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
#include "i_ui.h"
#include "h_config.h"

#define SCENE_TYPE_MAP "map"
#define SCENE_TYPE_CUTSCENE "cutscene"

typedef enum { MAP, BATTLE, TRANSITION, PAUSED, LOADING, RESULT, STATS } SCENE_STATE;

static bool resourcesLoaded = false;

//I tried to implement something more cleaner
//But fuck you
class Scene
{
private:
	//State
	SCENE_STATE			state;

	//Stats window
	StatsWindow			swindow;

	//Scrolling
	MapScrollingListener oListener;	

	//Loading thread
	sf::Thread			thread;

	//Shaders
	ShaderManager		sm;

	//Battle screen
	Battle				battle;

	std::vector<MoneyEffect> me;

	//Enemy squads
	std::vector<Squad>	squads;

	std::deque<Portal>	portals;

	//The Doors
	std::vector<Door>	doors;

	//The Doors
	std::vector<NPC>	npcs;

	std::vector<Chest>	chests;

	std::vector<ShopCrystal>	shops;

	std::vector<Column>	columns;

	bool				started;

	//Player object
	PlayerObject		po;

	//Particles manager
	ParticlesManager	pm;

	//Render all to final texture
	sf::RenderTexture	finalTexture;

	//Views
	sf::View			unscalable;
	Camera camera;

	//Loading text
	unsigned int		counter;
	sf::Text			loadingText;

	Shop shop;

	//Transition timer
	sf::Clock			transitionClock;

	//Squad to delete
	Squad				squadToDelete;
	unsigned int		toDelete;

	//Day counter
	Days				days;

	//List of tips
	std::vector<Tip>	tips;

	//Portrait and xp bar
	XPBar				xpbar;

	//YOBA
	sf::Sprite			vingette;

	//Camera tweener
	CDBTweener			oTweener;
	float				ncx, ncy;
	sf::Vector2f		camStart;
	bool				moving;

	//Scene type
	std::string			type;

	//Attack effect
	//NEED MORE BOOLEANS
	Swing				swing;
	bool				afterSwing;
	bool				check;

	//Result screen
	Result				resScreen;
	bool				startResults;

	//Galaxy back
	sf::Texture			backTexture;
	sf::Shader			galaxyBack;

	bool				lastSpeaked;

	//Loading resources
	void				loadResources();

public:
	Scene() : thread(&Scene::loadResources, this), oListener(moving)
	{
		//Create render texture
		finalTexture.create(WIDTH, HEIGHT);
		unscalable = sf::View(sf::FloatRect(0, 0, WIDTH, HEIGHT));
	};
	void init(std::string newType, std::string name);
	void startBattle(Squad &squad, bool p);
	void startBattle(Monster &m, bool p);
	void endBattle();
	void endBattleB();
	void setMapState(){state = MAP;};
	void removeTip(std::string type);
	void setPaused(bool p) {state = (p == true ? PAUSED : MAP);};
	void update(sf::Time time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
	void setCurrentEffect(std::string string, sf::Time time);
	std::vector<tmx::MapObject> &getObjects();
	PlayerObject &getPlayerObject();
	std::deque<Squad> &getSquadList();
	Camera &getCamera(){return camera;};
	std::vector<Door> &getDoors(){return doors;};
	sf::Vector2f getPlayerPositionOnMap(){return po.getOnMap().GetPosition();};
	sf::FloatRect getPlayerBoundOnMap(){return po.getSprite().getGlobalBounds();};
	void setBattleState(){battle.setW();state = BATTLE;};
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
		tShader.loadFromFile("assets/transition.frag", sf::Shader::Fragment);
		tShader2.loadFromFile("assets/inversion.frag", sf::Shader::Fragment);
		counter = 0;
	};

	SceneManager( const SceneManager& );
	SceneManager& operator =( const SceneManager& );

	//Current scene pointer
	Scene		current;

	//Transition
	bool				transition;
	bool				loaded;
	sf::Shader			tShader;
	sf::Shader			tShader2;
	sf::RenderTexture	t1;
	sf::RenderTexture	t2;
	sf::Clock			timer;
	unsigned int		counter;
	SceneInfo lastSi;

public:
	void draw(sf::RenderTarget &rt);
	void update(sf::Time time);
	void input(sf::Event &event);
	void setScene(SceneInfo si);
	Scene &getScene()
	{
		return current;
	};
	void initBattle(Squad &squad, bool p);
	void initBattle(Monster &b, bool p);
	void endBattle();
	void endBattleB(){current.endBattleB();};
	void startTransition(SceneInfo si);
};

#endif
