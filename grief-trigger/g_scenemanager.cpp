#include "g_scenemanager.h"

#include "h_config.h"
#include "os_x360controller.hpp"

/*
* BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  
* EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  
* BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  
* EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  BEEDLOKODE  EDOKOLDEEB  
*/

void Scene::loadResources()
{
	vingette.setTexture(TextureManager::instance().getTexture("assets/vingette.png"));

	//Init some managers/etc...

	battle.loadResources();

	sm.init(sf::Vector2f(WIDTH, HEIGHT));

	DialoguePanel::instance().loadResources();
	DialoguePanel::instance().init();

	pm.init("assets/smoke.png");

	galaxyBack.loadFromFile("assets/galaxy.frag", sf::Shader::Fragment);

	state = MAP;

	resScreen.loadResources();

	backTexture.create(WIDTH,HEIGHT);

	resourcesLoaded = true;

	if (Level::instance().getDay() == 6 && Level::instance().getScene() == 7)
	{
		//Monster &mo = Monster("boss1");
		//SceneManager::instance().initBattle( mo ,false);
		state = BATTLE;
	}
	if (Level::instance().getDay() == 6 && Level::instance().getScene() == 8)
	{
		//Monster &mo = Monster("boss2");
		//SceneManager::instance().initBattle( mo ,false);
		state = BATTLE;
	}
}

void Scene::init(std::string newType, std::string name)
{
	std::cout << Parser::instance().getSceneInfo(Level::instance().getDay(), Level::instance().getScene()).type + "\n";

	camera.setPosition(0,0);

	squads.clear();
	doors.clear();
	chests.clear();
	shops.clear();
	portals.clear();
	columns.clear();
	npcs.clear();

	if (Parser::instance().getMap(Level::instance().getDay(), Level::instance().getScene()) != "")
	{
		Level::instance().load(Parser::instance().getMap(Level::instance().getDay(), Level::instance().getScene()));
	}
	else
	{
		Level::instance().load("empty.tmx");
	}

	finalTexture.clear();
	finalTexture.create(WIDTH, HEIGHT);

	DialoguePanel::instance().stop();
	DialoguePanel::instance().clean();
	DialoguePanel::instance().hide();
	
	type = Parser::instance().getSceneInfo(Level::instance().getDay(), Level::instance().getScene()).type;

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

	if (Parser::instance().getMap(Level::instance().getDay(), Level::instance().getScene()) != "" && type == SCENE_TYPE_MAP)
	{
		//Parse map
		for(auto i = Level::instance().getML()->GetLayers().back().objects.begin(); i != Level::instance().getML()->GetLayers().back().objects.end(); i++)
		{
			tmx::MapObject &object = *i;
			if (object.GetName() == "hero")
			{
				//Set center of camera to player coords
				camera.setCenter(object.GetPosition().x, object.GetPosition().y);
				ncx = camera.getPosition().x;
				ncy = camera.getPosition().y;

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
			else if (object.GetName() == "shop")
			{
				//Init shop
				ShopCrystal crystal;
				crystal.init(object.GetPosition(), object);
				shops.push_back(crystal);
			}
			else if (object.GetName() == "door")
			{
				//Init door
				Door door;
				door.init(object.GetPosition(), object);
				doors.push_back(door);
			}	
			else if (object.GetName() == "column")
			{
				//Init column
				Column col;
				col.init(object.GetPosition(), object);
				columns.push_back(col);
			}
			else if (object.GetName() == "chest")
			{
				//Init column
				Chest chest;
				chest.init(object.GetPosition(), object);
				chests.push_back(chest);
			}
			else if (object.GetName() == "portalstart")
			{
				//Init portal
				Portal p(true);
				p.init(object.GetPosition(), object);
				portals.push_back(p);
			}
			else if (object.GetName() == "portalend")
			{
				//Init portal
				Portal p(false);
				p.init(object.GetPosition(), object);
				portals.push_back(p);
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
	else if (Parser::instance().getMap(Level::instance().getDay(), Level::instance().getScene()) != "")
	{
		for(auto i = Level::instance().getML()->GetLayers().back().objects.begin(); i != Level::instance().getML()->GetLayers().back().objects.end(); i++)
		{
			tmx::MapObject &object = *i;
			if (object.GetName() == "hero")
			{
				camera.setCenter(object.GetPosition().x, object.GetPosition().y);
			}
			else if (object.GetName() == "portalstart")
			{
				//Init portal
				Portal p(true);
				p.init(object.GetPosition(), object);
				portals.push_back(p);
			}
			else if (object.GetName() == "portalend")
			{
				//Init portal
				Portal p(false);
				p.init(object.GetPosition(), object);
				portals.push_back(p);
			}
		}
	}

	MusicManager::instance().playMusic(Parser::instance().getMusic(Level::instance().getDay(), Level::instance().getScene()));

	started = false;
	startResults = true;

	if (Level::instance().getDay() == 6 && Level::instance().getScene() == 7)
	{
		Monster &mo = Monster("boss1");
		SceneManager::instance().initBattle( mo ,false);
		state = BATTLE;
	}
	if (Level::instance().getDay() == 6 && Level::instance().getScene() == 8)
	{
		Monster &mo = Monster("boss2");
		SceneManager::instance().initBattle( mo ,false);
		state = BATTLE;
	}
}

void Scene::endBattleB()
{
	battle.clean();

	MusicManager::instance().playMusic(Parser::instance().getMusic(Level::instance().getDay(), Level::instance().getScene()));

	state = MAP;
}

void Scene::endBattle()
{
	battle.clean();

	squads[toDelete].getOnMap().SetName("null");

	squads.clear();

	if (Parser::instance().getMap(Level::instance().getDay(), Level::instance().getScene()) != "" && type == SCENE_TYPE_MAP)
	{
		//Parse map
		for(auto i = Level::instance().getML()->GetLayers().back().objects.begin(); i != Level::instance().getML()->GetLayers().back().objects.end(); i++)
		{
			tmx::MapObject &object = *i;
			if (object.GetName() == "squad")
			{
				//Init squad
				Squad newSquad;
				newSquad.init(object.GetPropertyString("monsters"), object.GetPosition(), object);
				squads.push_back(newSquad);
			}
		}
	}

	MusicManager::instance().playMusic(Parser::instance().getMusic(Level::instance().getDay(), Level::instance().getScene()));

	if (GameData::instance().getPlayer().getLevelOffset() != 0)
	{
		state = RESULT;
		resScreen.reset();
		resScreen.show();
		startResults = true;
	}
	else
	{
		state = MAP;
	}
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
	shop.update(time);
	days.update(time);
	DialoguePanel::instance().update(); 
	if (type == SCENE_TYPE_MAP)
	{
		if (state != LOADING && state != RESULT)
		{
			if (state == BATTLE)
			{
				battle.update(time);			
			}
			if (state == STATS)
			{
				swindow.update(time);
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
					setCurrentEffect("battle", sf::seconds(1.0f));
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
				if (po.getSprite().getLocalBounds().intersects(xpbar.getBounds()))
				{
					xpbar.setTransparent(true);
				}
				else
				{
					xpbar.setTransparent(false);
				}

				for (auto it = squads.begin(); it != squads.end(); it++)
				{
					if (it->getOnMap().GetName() != "null")
					{
						it->update(time);	
					}									
				}

				/************************************************************************/
				/* Scrolling															*/
				/************************************************************************/
				if (!moving)
				{				
					//Update objects only if camera is not moving
					if (!shop.isWorking() && type != SCENE_TYPE_CUTSCENE ) po.update(time);
					for (auto i = chests.begin(); i != chests.end(); ++i)
					{
						i->update(time);	
					}
					for (auto i = portals.begin(); i != portals.end(); ++i)
					{
						i->update(time);				
					}
					for (auto i = shops.begin(); i != shops.end(); ++i)
					{
						i->update(time);				
					}
					for (auto i = me.begin(); i != me.end(); ++i)
					{
						i->update(time);			
					}
					//Update objects only if camera is not moving

					if (po.getSprite().getPosition().y > camera.getPosition().y + HALF_HEIGHT - (CHARACTER_SIZE*2) )
					{
						ncy = camera.getPosition().y;
						CDBTweener::CTween *pTween = new CDBTweener::CTween();
						pTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 1.5f);
						pTween->addValue(&ncy, ncy + (HALF_HEIGHT / 2));
						pTween->setUserData("Eagle");
						oTweener.addTween(pTween);
						oTweener.addListener(&oListener);
						moving = true;
					}
					if(po.getSprite().getPosition().y < camera.getPosition().y + (CHARACTER_SIZE*2) )
					{
						ncy = camera.getPosition().y;
						CDBTweener::CTween *pTween = new CDBTweener::CTween();
						pTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 1.5f);
						pTween->addValue(&ncy, ncy - (HALF_HEIGHT / 2));
						pTween->setUserData("Eagle");
						oTweener.addTween(pTween);
						oTweener.addListener(&oListener);
						moving = true;
					}
					if (po.getSprite().getPosition().x >= camera.getPosition().x + HALF_WIDTH - (CHARACTER_SIZE*2) )
					{
						ncx = camera.getPosition().x;
						CDBTweener::CTween *pTween = new CDBTweener::CTween();
						pTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 1.5f);
						pTween->addValue(&ncx, ncx + (HALF_WIDTH / 2));
						pTween->setUserData("Eagle");
						oTweener.addTween(pTween);
						oTweener.addListener(&oListener);
						moving = true;
					}
					if (po.getSprite().getPosition().x < camera.getPosition().x + (CHARACTER_SIZE*2) )
					{
						ncx = camera.getPosition().x;
						CDBTweener::CTween *pTween = new CDBTweener::CTween();
						pTween->setEquation(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 1.5f);
						pTween->addValue(&ncx, ncx - (HALF_WIDTH / 2));
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
					//Check Enemies
					for (auto it = squads.begin(); it != squads.end(); ++it)
					{
						Squad &squad = *it;
						if (squad.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE+1, CHARACTER_SIZE, CHARACTER_SIZE * 2))
							|| squad.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x - CHARACTER_SIZE + 1, po.getSprite().getPosition().y, CHARACTER_SIZE * 2, CHARACTER_SIZE)))
						{
							if ((po.getSprite().getPosition().y > squad.getOnMap().GetPosition().y && po.getDirection() == DIR_UP)
								|| (po.getSprite().getPosition().y < squad.getOnMap().GetPosition().y && po.getDirection() == DIR_DOWN)
								|| (po.getSprite().getPosition().x < squad.getOnMap().GetPosition().x && po.getDirection() == DIR_RIGHT)
								|| (po.getSprite().getPosition().x > squad.getOnMap().GetPosition().x && po.getDirection() == DIR_LEFT))
							{
								//Some fuking magic with...
								if (tips.size() == 0) tips.push_back(Tip(L"Атаковать!", _X, "enemy"));
								if ((sf::Keyboard::isKeyPressed(sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A))) )
								{
									SceneManager::instance().initBattle(squad, true);

									removeTip("enemy");
								}	
								break;
							}						
						}	
						else removeTip("enemy");
					}

					if (!shop.isWorking())
					{
						for (auto it = shops.begin(); it != shops.end(); ++it)
						{
							ShopCrystal &crystal = *it;
							if (crystal.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE+1, CHARACTER_SIZE, CHARACTER_SIZE * 2))
								|| crystal.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x - CHARACTER_SIZE + 1, po.getSprite().getPosition().y, CHARACTER_SIZE * 2, CHARACTER_SIZE)))
							{
								if ((po.getSprite().getPosition().y > crystal.getOnMap().GetPosition().y && po.getDirection() == DIR_UP)
									|| (po.getSprite().getPosition().y < crystal.getOnMap().GetPosition().y && po.getDirection() == DIR_DOWN)
									|| (po.getSprite().getPosition().x < crystal.getOnMap().GetPosition().x && po.getDirection() == DIR_RIGHT)
									|| (po.getSprite().getPosition().x > crystal.getOnMap().GetPosition().x && po.getDirection() == DIR_LEFT))
								{
									//Some fuking magic with...
									if (tips.size() == 0) tips.push_back(Tip(L"Магазин", _X, "enemy"));
									if ((sf::Keyboard::isKeyPressed(sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A))) )
									{
										shop.show();
										removeTip("shop");
									}	
									break;
								}						
							}	
							else removeTip("shop");
						}
					}					

					//Check portals
					for (auto it = portals.begin(); it != portals.end(); ++it)
					{
						Portal &portal = *it;
						if (portal.getType() == false)
						{
							if (portal.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE+1, CHARACTER_SIZE, CHARACTER_SIZE * 2))
								|| portal.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x - CHARACTER_SIZE + 1, po.getSprite().getPosition().y, CHARACTER_SIZE * 2, CHARACTER_SIZE)))
							{
								if ((po.getSprite().getPosition().y > portal.getOnMap().GetPosition().y && po.getDirection() == DIR_UP)
									|| (po.getSprite().getPosition().y < portal.getOnMap().GetPosition().y && po.getDirection() == DIR_DOWN)
									|| (po.getSprite().getPosition().x < portal.getOnMap().GetPosition().x && po.getDirection() == DIR_RIGHT)
									|| (po.getSprite().getPosition().x > portal.getOnMap().GetPosition().x && po.getDirection() == DIR_LEFT))
								{
									//Some fuking magic with...
									if (tips.size() == 0) tips.push_back(Tip(L"Войти в портал", _X, "portal"));
									if ((sf::Keyboard::isKeyPressed(sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A))) )
									{
										Level::instance().nextScene();
										std::cout << "DEHb + " + std::to_string(Level::instance().getDay()) + ", CILEHA + " + std::to_string(Level::instance().getScene()) + "\n";
										SceneManager::instance().startTransition(Parser::instance().getSceneInfo(Level::instance().getDay(), Level::instance().getScene()));

										state = PAUSED;

										removeTip("portal");
									}	
									break;
								}						
							}	
							else removeTip("portal");
						}					
					}

					//Check doors
					for (auto it = doors.begin(); it != doors.end(); ++it)
					{
						Door &door = *it;
						if ((!door.isOpened() && door.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE * 2)))
							|| (!door.isOpened() && door.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y + CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE * 2))))
						{
							//Some fuking magic with the doors
							if (tips.size() == 0) tips.push_back(Tip(L"Открыть дверь", _X, "door"));
							if ((sf::Keyboard::isKeyPressed(sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A))) )
							{
								door.open();

								removeTip("door");
							}	
							break;
						}		
						else removeTip("door");
					}

					//Check chests
					for (auto it = chests.begin(); it != chests.end(); ++it)
					{
						Chest &chest = *it;
						if ((!chest.isOpened() && chest.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE * 2)))
							|| (!chest.isOpened() && chest.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y + CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE * 2))))
						{
							//Some fuking magic with...
							if (tips.size() == 0) tips.push_back(Tip(L"Открыть сундук", _X, "chest"));
							if ((sf::Keyboard::isKeyPressed(sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A))) )
							{
								chest.open();

								me.push_back(MoneyEffect(chest.getOnMap().GetPosition()));

								removeTip("chest");
							}	
							break;

						}	
						else removeTip("chest");
					}

					if (!lastSpeaked)
					{
						//Check NPCs
						for (auto it = npcs.begin(); it != npcs.end(); ++it)
						{
							NPC &npc = *it;
							if (npc.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x, po.getSprite().getPosition().y - CHARACTER_SIZE+1, CHARACTER_SIZE, CHARACTER_SIZE * 2))
								|| npc.getOnMap().GetAABB().intersects(sf::FloatRect(po.getSprite().getPosition().x - CHARACTER_SIZE + 1, po.getSprite().getPosition().y, CHARACTER_SIZE * 2, CHARACTER_SIZE)))
							{
								//...
								if (tips.size() == 0) tips.push_back(Tip(L"Общение", _X, "npc"));
								if ((sf::Keyboard::isKeyPressed(sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A)))  && !DialoguePanel::instance().isHided())
								{
									DialoguePanel::instance().openDialogue(npc.getOnMap().GetName(), "day" + std::to_string(Level::instance().getDay()) + "/scene" + std::to_string(Level::instance().getScene()));
									lastSpeaked = true;

									removeTip("npc");
								}	
								break;
							}		
							else removeTip("npc");
						}
					}				
				}	
			}
		}
	}
	else if (type == SCENE_TYPE_CUTSCENE)
	{
		for (auto i = portals.begin(); i != portals.end(); ++i)
		{
			i->update(time);				
		}

		//Start speech if cutscene
		if (type == SCENE_TYPE_CUTSCENE && !started && state != LOADING)
		{
			DialoguePanel::instance().stop();
			DialoguePanel::instance().clean();
			//DialoguePanel::instance().hide();
			DialoguePanel::instance().openDialogue("cutscene", "day" + std::to_string(Level::instance().getDay()) + "/scene" + std::to_string(Level::instance().getScene()));
			started = true;
			//DialoguePanel::instance().openDialogue("cutscene", "day1/scene1");
		}
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
	else
	{
		counter++;
	}

	if (startResults)
	{
		startResults = false;
	}

	//Always update shaders
	sm.update();
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
				//finalTexture.setView(camera.getView());

				sf::Sprite spr;
				spr.setTexture(backTexture);
				galaxyBack.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
				galaxyBack.setParameter("seconds", counter);
				finalTexture.draw(spr, &galaxyBack);

				finalTexture.setView(camera.getView());

				//Map
				Level::instance().getML()->Draw(finalTexture);

				//Player object
				for (auto i = doors.begin(); i != doors.end(); ++i)
				{
					if (!i->isOpened())
					{
						i->draw(finalTexture);
					}					
				}
				
				for (auto i = columns.begin(); i != columns.end(); ++i)
				{
					i->draw(finalTexture);				
				}
				for (auto i = chests.begin(); i != chests.end(); ++i)
				{
					i->draw(finalTexture);			
				}
				for (auto i = portals.begin(); i != portals.end(); ++i)
				{
					if (i->getOnMap().GetPosition().y <= po.getSprite().getPosition().y)i->draw(finalTexture);							
				}
				for (auto i = shops.begin(); i != shops.end(); ++i)
				{
					if (i->getOnMap().GetPosition().y <= po.getSprite().getPosition().y)i->draw(finalTexture);							
				}
				if (Level::instance().getDay() == 6)
				{
					if (Level::instance().getScene() == 8 || Level::instance().getScene() == 7)
					{
						
					}
					else
					{
						po.draw(finalTexture); //poor white guy
					}
				}
				else
				{
					po.draw(finalTexture); //poor white guy
				}
				for (auto i = shops.begin(); i != shops.end(); ++i)
				{
					if (i->getOnMap().GetPosition().y > po.getSprite().getPosition().y)i->draw(finalTexture);							
				}
				for (auto i = portals.begin(); i != portals.end(); ++i)
				{
					if (i->getOnMap().GetPosition().y > po.getSprite().getPosition().y)i->draw(finalTexture);							
				}

				for (auto i = doors.begin(); i != doors.end(); ++i)
				{
					if (i->isOpened())
					{
						i->draw(finalTexture);
					}					
				}
				for (auto i = columns.begin(); i != columns.end(); ++i)
				{
					i->drawUpper(finalTexture);				
				}

				for (auto i = npcs.begin(); i != npcs.end(); ++i)
				{
					i->draw(finalTexture);
				}

				//Enemy squads
				for (auto i = squads.begin(); i != squads.end(); ++i)
				{
					i->draw(finalTexture);
				}

				for (auto i = me.begin(); i != me.end(); ++i)
				{
					i->draw(finalTexture);			
				}

				swing.draw(finalTexture);

				//Unscalable				
				finalTexture.setView(unscalable);

				//Tips, only if hero is not moving
				if (!po.isWalking())
				{
					for (auto i = tips.begin(); i != tips.end(); ++i)
					{
						i->draw(finalTexture, sf::Vector2f(990, 680));
					}
				}

				//Dialogue UI
				DialoguePanel::instance().draw(finalTexture);	

				finalTexture.draw(vingette);	
				xpbar.draw(finalTexture);

				days.draw(finalTexture);

				if (state == RESULT)
				{
					resScreen.draw(finalTexture);
				}
				

				if (state == STATS)
				{
					swindow.draw(finalTexture);
				}

				shop.draw(finalTexture);
			}
			else if (!swing.isWorking() && state == BATTLE) //Draw battle
			{
				//Scaled
				finalTexture.setView(sf::View(sf::FloatRect(0,0,HALF_WIDTH, HALF_HEIGHT)));
				battle.draw(finalTexture);

				//Unscalable
				finalTexture.setView(unscalable);
				battle.drawUI(finalTexture);
			}

			finalTexture.display();

			sm.draw(finalTexture, tg);
		}
		else
		{
			sm.setCurrentEffect("inversion", sf::seconds(1));
			
			finalTexture.clear(sf::Color(24u, 19u, 27u));
			finalTexture.draw(loadingText);
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
			Level::instance().getML()->Draw(finalTexture);

			for (auto i = portals.begin(); i != portals.end(); ++i)
			{
				i->draw(finalTexture);				
			}

			//Unscalable
			finalTexture.setView(unscalable);

			//Dialogue UI
			DialoguePanel::instance().draw(finalTexture);		

			finalTexture.draw(vingette);

			days.draw(finalTexture);

			finalTexture.display();

			sm.draw(finalTexture, tg);
		}
		else
		{
			sm.setCurrentEffect("inversion", sf::seconds(1));

			finalTexture.clear(sf::Color(24u, 19u, 27u));
			finalTexture.draw(loadingText);
			finalTexture.display();
			sm.draw(finalTexture, tg);
		}
	}
}

std::vector<tmx::MapObject> &Scene::getObjects()
{
	return Level::instance().getML()->GetLayers().back().objects;
}

void Scene::startBattle(Monster &m, bool p)
{
	MusicManager::instance().playMusic("maintheme");
	state = BATTLE;

	std::vector<Monster> *ms = new std::vector<Monster>;
	ms->push_back(m);

	battle.start(*ms, p);

	afterSwing = false;
}

void Scene::startBattle(Squad &squad, bool p)
{
	MusicManager::instance().playMusic("maintheme");
	toDelete = std::find(squads.begin(), squads.end(), squad) - squads.begin();
	swing.init(squad.getOnMap().GetPosition());
	state = TRANSITION;

	battle.start(squad.getMonsters(), p);

	afterSwing = false;
}

void Scene::input(sf::Event &event)
{	
	DialoguePanel::instance().input(event);
	
	if (state == STATS && !shop.isWorking())swindow.input(event);
	if (type == SCENE_TYPE_MAP)
	{
		if (state != LOADING)
		{
			if (state != BATTLE)
			{		
				if (!shop.isWorking())
				{		
					if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) || (xb::Joystick::isButtonPressed(0, xb::Joystick::B)  && event.type == sf::Event::JoystickButtonPressed)  && state == MAP && !shop.isWorking())
					{
						swindow.show();
						state = STATS;
					}	

					if (state != PAUSED)
					{
						if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V) state = PAUSED;
					}
					else
					{
						if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V) state = MAP;
					}
				}
				if (shop.isWorking())shop.input(event);

				if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) && shop.isWorking())shop.close();	
			}
			else if (state == BATTLE)
			{
				battle.input(event);
			}
			if (state == RESULT && !startResults)
			{
				resScreen.input(event);
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

void SceneManager::setScene(SceneInfo si)
{
	current.init(si.type, si.map);

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
		//tShader.setParameter("texture2", t2.getTexture());			// hotya eto uzhe blizko
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

		current.init(lastSi.type, lastSi.map);
	}

	MusicManager::instance().update(time);
}

void SceneManager::input(sf::Event &event)
{
	if (loaded && !transition) current.input(event);
}

void SceneManager::initBattle(Squad &squad, bool p)
{
	current.startBattle(squad, p);
}

void SceneManager::initBattle(Monster &b, bool p)
{
	current.startBattle(b, p);
}

void SceneManager::endBattle()
{
	current.endBattle();
}

void SceneManager::startTransition(SceneInfo si)
{
	t1.create(WIDTH, HEIGHT);
	t2.create(WIDTH, HEIGHT);
	t1.clear();
	t2.clear();

	lastSi = si;

	current.draw(t1);

	SoundManager::instance().playTransitionSound();

	timer.restart();

	loaded = false;
	transition = true;
}
