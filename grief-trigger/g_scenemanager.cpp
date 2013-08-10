#include "g_scenemanager.h"

#include "h_config.h"

/*
* BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  
* EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  
* BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  
* EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  
*/

void Scene::loadResources()
{
	//vingette.setTexture(TextureManager::instance().getTexture("assets/vingette.png"));

	//Init some managers/etc...

	battle.loadResources();

	days.setTexture(TextureManager::instance().getTexture("assets/day.png"));
	days.setPosition(WIDTH - days.getTextureRect().width, 0);

	sm.init(sf::Vector2f(WIDTH, HEIGHT));

	DialoguePanel::instance().loadResources();
	DialoguePanel::instance().init();

	pm.init("assets/smoke.png");

	state = MAP;

	//Start speech if cutscene
	if (type == SCENE_TYPE_CUTSCENE)
	{
		DialoguePanel::instance().openDialogue("cutscene", "day" + std::to_string(Level::instance().getDay()) + "/scene" + std::to_string(Level::instance().getScene()));
	}

	resourcesLoaded = true;
}

void Scene::init(std::string newType, std::string name, tmx::MapLoader &ml)
{
	//Constructor
	if (name != "") 
	{
		ml.Load(name);
	}
	map = &ml;
	type = newType;

	loadingText = sf::Text("Loading", DFont::instance().getFont(), 15);
	loadingText.setPosition(HALF_WIDTH - (loadingText.getGlobalBounds().width / 2), HALF_HEIGHT - (loadingText.getGlobalBounds().height / 2));

	if (!resourcesLoaded)
	{
		//Start loading
		state = LOADING;
		thread.launch();
	}
	else
	{
		state = MAP;
	}

	if (type == SCENE_TYPE_MAP)
	{
		//Parse map
		for(auto i = map->GetLayers().back().objects.begin(); i != map->GetLayers().back().objects.end(); i++)
		{
			tmx::MapObject &object = *i;
			if (object.GetName() == "hero")
			{
				//Set center of camera to player coords
				camera.setPosition(0, 0);
				ncx = camera.getPosition().x;
				ncy = camera.getPosition().y;
				//camera.setCenter(0 + (HALF_WIDTH / 2), 0 + (HALF_HEIGHT / 2));

				//Init hero object
				po.init(object.GetPosition(), object);
			}
			else if (object.GetName() == "squad")
			{
				//Init squad
				Squad newSquad;
				newSquad.init(object.GetPropertyString("monsters"), object.GetPosition(), object);
				squads.push_back(newSquad);
			}
			else if (object.GetName() == "door")
			{
				//Init door
				Door door;
				door.init(object.GetPosition(), object);
				doors.push_back(door);
			}	
			else if (object.GetName() != "")
			{
				//Add NPC
				NPC npc;
				npc.init(object.GetPosition(), object);
				npcs.push_back(npc);
			}
		}
	}

	//Create render texture
	finalTexture.create(WIDTH, HEIGHT);

	MusicManager::instance().playMusic(Parser::instance().getMusic(Level::instance().getDay(), Level::instance().getScene()));
}

void Scene::endBattle()
{
	MusicManager::instance().playMusic(Parser::instance().getMusic(Level::instance().getDay(), Level::instance().getScene()));
	squads[toDelete].getOnMap().SetName("null");
	squads.erase(squads.begin()+toDelete);
	//battle.clean();

	state = MAP;
}

void Scene::removeTip(std::string type)
{
	for (auto i = tips.begin(); i != tips.end();)
	{
		Tip &tip = *i;
		if (tip.getType() == type)
		{
			i = tips.erase(i);
			break;
		}
		else
		{
			i++;
		}
	}
}

void Scene::update(sf::Time time)
{
	if (type == SCENE_TYPE_MAP)
	{
		if (state != LOADING)
		{
			if (state == BATTLE)
			{
				battle.update(time);
			}
			else
			{
				DialoguePanel::instance().update(); 
			}
			if (state == TRANSITION)
			{
				swing.update(time);

				if (!swing.isWorking() && afterSwing == false && check == false)
				{				
					afterSwing = true;
				}			
				if (!sm.isWorking() && !swing.isWorking() && afterSwing == true && check == false)
				{				
					transitionClock.restart();
					setCurrentEffect("battle", sf::seconds(0.97));
					afterSwing = false;
					check = true;
				}		
				if (!sm.isWorking() && !swing.isWorking() && afterSwing == false)
				{				
					state = BATTLE;
					check = false;
				}	
			}
			if (state == MAP)
			{
				/************************************************************************/
				/* Scrolling															*/
				/************************************************************************/
				if (!moving)
				{				
					//Update objects only if camera is not moving
					po.update(time);
					for (auto i = squads.begin(); i != squads.end(); i++)
					{
						Squad &s = *i;
						s.update(time);					
					}
					//Update objects only if camera is not moving

					if (po.getSprite().getPosition().y > camera.getPosition().y + HALF_HEIGHT)
					{
						ncy = camera.getPosition().y;
						CDBTweener::CTween *pTween = new CDBTweener::CTween();
						pTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 1.0f);
						pTween->addValue(&ncy, ncy + HALF_HEIGHT);
						pTween->setUserData("Eagle");
						oTweener.addTween(pTween);
						oTweener.addListener(&oListener);
						moving = true;
					}
					if(po.getSprite().getPosition().y < camera.getPosition().y)
					{
						ncy = camera.getPosition().y;
						CDBTweener::CTween *pTween = new CDBTweener::CTween();
						pTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 1.0f);
						pTween->addValue(&ncy, ncy - HALF_HEIGHT);
						pTween->setUserData("Eagle");
						oTweener.addTween(pTween);
						oTweener.addListener(&oListener);
						moving = true;
					}
					if (po.getSprite().getPosition().x >= camera.getPosition().x + HALF_WIDTH)
					{
						ncx = camera.getPosition().x;
						CDBTweener::CTween *pTween = new CDBTweener::CTween();
						pTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 1.0f);
						pTween->addValue(&ncx, ncx + HALF_WIDTH);
						pTween->setUserData("Eagle");
						oTweener.addTween(pTween);
						oTweener.addListener(&oListener);
						moving = true;
					}
					if (po.getSprite().getPosition().x < camera.getPosition().x)
					{
						ncx = camera.getPosition().x;
						CDBTweener::CTween *pTween = new CDBTweener::CTween();
						pTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 1.0f);
						pTween->addValue(&ncx, ncx - HALF_WIDTH);
						pTween->setUserData("Eagle");
						oTweener.addTween(pTween);
						oTweener.addListener(&oListener);
						moving = true;
					}
				}
				oTweener.step((float)time.asMilliseconds() * 0.001f);
				camera.setPosition(ncx, ncy);
				/************************************************************************/
				/* Scrolling															*/
				/************************************************************************/

				xpbar.update(time);	

				//Do logic
				if (!po.isWalking())
				{
					//Check doors
					for (auto it = doors.begin(); it != doors.end(); ++it)
					{
						Door &door = *it;
						if (!door.isOpened() && door.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE * 2)))
						{
							//Some fuking magic with the doors
							if (tips.size() == 0) tips.push_back(Tip(L"Открыть дверь", _Z, "door"));
							if (sf::Keyboard::isKeyPressed(_Z))
							{
								door.open();

								removeTip("door");
							}	
							break;
						}		
						else removeTip("door");
					}

					//Check Enemies
					for (auto it = squads.begin(); it != squads.end(); ++it)
					{
						Squad &squad = *it;
						if (squad.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE+1, CHARACTER_SIZE, CHARACTER_SIZE * 2))
							|| squad.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x - CHARACTER_SIZE + 1, po.getSprite().getPosition().y, CHARACTER_SIZE * 2, CHARACTER_SIZE)))
						{
							//Some fuking magic with...
							if (tips.size() == 0) tips.push_back(Tip(L"Атаковать!", _X, "enemy"));
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
							{
								SceneManager::instance().initBattle(squad);

								removeTip("enemy");
							}	
							break;
						}		
						else removeTip("enemy");
					}

					//Check NPCs
					for (auto it = npcs.begin(); it != npcs.end(); ++it)
					{
						NPC &npc = *it;
						if (npc.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE+1, CHARACTER_SIZE, CHARACTER_SIZE * 2))
							|| npc.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x - CHARACTER_SIZE + 1, po.getSprite().getPosition().y, CHARACTER_SIZE * 2, CHARACTER_SIZE)))
						{
							//...
							if (tips.size() == 0) tips.push_back(Tip(L"Общение", _X, "npc"));
							if (sf::Keyboard::isKeyPressed(_X) && !DialoguePanel::instance().isHided())
							{
								DialoguePanel::instance().openDialogue(npc.getOnMap().GetName(), "day" + std::to_string(Level::instance().getDay()) + "/scene" + std::to_string(Level::instance().getScene()));

								removeTip("npc");
							}	
							break;
						}		
						else removeTip("npc");
					}
				}	
			}
		}
		//Always update shaders
		if (sm.isWorking()) sm.update();
	}
	else if (type == SCENE_TYPE_CUTSCENE)
	{
		DialoguePanel::instance().update();
	}

	if (state == LOADING) //loading screen
	{
		if (counter == 1) loadingText.setString("Loading");
		else if (counter % 10 == 0) loadingText.setString("Loading.");
		else if (counter % 15 == 0) loadingText.setString("Loading..");
		else if (counter % 20 == 0) loadingText.setString("Loading...");
		else if (counter >= 20) counter = 0;
		counter++;
	}
}


void Scene::draw(sf::RenderTarget &tg)
{
	if (type == SCENE_TYPE_MAP)
	{
		if (state != LOADING)
		{
			finalTexture.clear(sf::Color(24u, 19u, 27u));

			//Draw map and hero
			if (state != BATTLE)
			{
				//Game content
				finalTexture.setView(camera.getView());
				//Map
				map->Draw(finalTexture);
				//Enemy squads
				for (auto i = squads.begin(); i != squads.end(); ++i)
				{
					i->draw(finalTexture);
				}
				//Player object
				po.draw(finalTexture);
				for (auto i = doors.begin(); i != doors.end(); ++i)
				{
					i->draw(finalTexture);
				}
				for (auto i = npcs.begin(); i != npcs.end(); ++i)
				{
					i->draw(finalTexture);
				}
				swing.draw(finalTexture);

				//Unscalable
				finalTexture.draw(vingette);
				finalTexture.setView(unscalable);

				//Tips, only if hero is not moving
				if (!po.isWalking())
				{
					for (auto i = tips.begin(); i != tips.end(); ++i)
					{
						i->draw(finalTexture, sf::Vector2f(990, 680));
					}
				}

				xpbar.draw(finalTexture);

				//Dialogue UI
				DialoguePanel::instance().draw(finalTexture);
				finalTexture.draw(days);
			}
			else if (!swing.isWorking() && state == BATTLE) //Draw battle
			{
				//Scaled
				finalTexture.setView(camera.getView());
				battle.draw(finalTexture);

				//Unscalable
				finalTexture.setView(unscalable);
				battle.drawUI(finalTexture);
			}

			finalTexture.display();

			sm.draw(finalTexture, tg);
		}
	}
	else if (type == SCENE_TYPE_CUTSCENE)
	{
		if (state != LOADING)
		{
			finalTexture.clear(sf::Color(24u, 19u, 27u));

			finalTexture.setView(camera.getView());
			//Map
			map->Draw(finalTexture);
			//Unscalable
			finalTexture.setView(unscalable);

			//Dialogue UI
			DialoguePanel::instance().draw(finalTexture);
			finalTexture.draw(days);

			finalTexture.display();

			sm.draw(finalTexture, tg);
		}
		else
		{
			tg.draw(loadingText);
		}
	}
}

std::vector<tmx::MapObject> &Scene::getObjects()
{
	return map->GetLayers().back().objects;
}

void Scene::startBattle(Squad &squad)
{
	MusicManager::instance().playMusic("maintheme");
	toDelete = std::find(squads.begin(), squads.end(), squad) - squads.begin();
	swing.init(squad.getOnMap().GetPosition());
	state = TRANSITION;

	battle.start(squad);

	afterSwing = false;
}

void Scene::input(sf::Event &event)
{
	DialoguePanel::instance().input(event);
	if (type == SCENE_TYPE_MAP)
	{
		if (state != LOADING)
		{
			if (state != BATTLE)
			{		
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) setCurrentEffect("distortion", sf::seconds(1));
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) setCurrentEffect("acid", sf::seconds(1));
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) finalTexture.getTexture().copyToImage().saveToFile("screenshot.png");

				if (state != PAUSED)
				{
					if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V) state = PAUSED;
				}
				else
				{
					if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V) state = MAP;
				}
			}
			else if (state == BATTLE)
			{
				battle.input(event);
			}
		}
	}
	else if (type == SCENE_TYPE_CUTSCENE)
	{

	}
}

void Scene::setCurrentEffect(std::string string, sf::Time time)
{
	sm.setCurrentEffect(string, time);
}

void SceneManager::setScene(SceneInfo si, tmx::MapLoader &ml)
{
	current.init(si.type, si.map, ml);

	loaded = true;
	transition = false;
}

void SceneManager::draw(sf::RenderTarget &rt)
{
	if (loaded && !transition)
	{
		current.draw(rt);
	}	
	if (transition && !loaded)
	{
		current.update(sf::seconds(1.0f / 15.f));
		current.draw(t2);
		loaded = true;
	}
	if (transition)
	{
		tShader.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));	//
		tShader.setParameter("texture1", t1.getTexture());			// ne hvataet tolko peremennih na translite
		tShader.setParameter("texture2", t2.getTexture());			// hotya eto uzhe blizko
		tShader.setParameter("seconds", counter / 2);				// 
		sf::Texture t;												//
		t.create(WIDTH, HEIGHT);									// *\ _______________________________/*
		sf::Sprite spr;												//   | | | | | | | | | | | | | | | ||
		spr.setTexture(t);											//   || | | | | | | | | | | | | | | |
		sf::RenderTexture t3;										//   | | | | | | | | | | | | | | | ||
		t3.create(WIDTH, HEIGHT);									//   || | | | | | | | | | | | | | | |
		t3.draw(spr, &tShader);										//   | | | | | | | | | | | | | | | ||
		spr.setTexture(t1.getTexture());							//   || | | | | | | | | | | | | | | |
		t3.draw(spr, &tShader);										//   | | | | | | | | | | | | | | | ||
		tShader2.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));	//   || | | | | | | | | | | | | | | |
		tShader2.setParameter("texture", t3.getTexture());			//   | | | | | | | | | | | | | | | ||
		tShader2.setParameter("seconds", counter);					//   \______________________________/
		spr.setTexture(t3.getTexture());							//
		rt.draw(spr, &tShader2);									// nahuy ya eto sdelal?
	}
}

void SceneManager::update(sf::Time time)
{
	if (counter < 60) 
	{
		counter++;
	}
	else
	{
		counter = 0;
	}
	if (loaded && !transition)
	{
		current.update(time);
	}
	if (transition && timer.getElapsedTime() >= sf::seconds(1.8f))
	{
		transition = false;
		loaded = true;
	}

	MusicManager::instance().update(time);
}

void SceneManager::input(sf::Event &event)
{
	if (loaded && !transition) current.input(event);
}

void SceneManager::initBattle(Squad &squad)
{
	//current.setPaused(true);
	//current.setCurrentEffect("battle", sf::seconds(0.97));
	current.startBattle(squad);
}

void SceneManager::endBattle()
{
	current.endBattle();
}

void SceneManager::startTransition(SceneInfo si)
{
	t1.create(WIDTH, HEIGHT);
	t2.create(WIDTH, HEIGHT);

	current.draw(t1);

	current.init(si.type, si.map, *current.getMapLoader());
	SoundManager::instance().playTransitionSound();

	timer.restart();

	loaded = false;
	transition = true;
}
