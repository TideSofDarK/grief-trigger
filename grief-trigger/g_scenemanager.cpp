#include "g_scenemanager.h"

#include "h_config.h"

void Scene::loadResources()
{
	vingette.setTexture(TextureManager::instance().getTexture("assets/vingette.png"));

	//Init some managers/etc...

	//Parse map
	for(auto i = map->GetLayers().back().objects.begin(); i != map->GetLayers().back().objects.end(); i++)
	{
		tmx::MapObject &object = *i;
		if (object.GetName() == "hero")
		{
			//Set center of camera to player coords
			//camera->setCenter(object.GetPosition().x + CHARACTER_SIZE, object.GetPosition().y + (CHARACTER_SIZE / 2));
			camera.setCenter(0 + (HALF_WIDTH / 2), 0 + (HALF_HEIGHT / 2));

			//Init hero object
			po.init(object.GetPosition().x, object.GetPosition().y, object);
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

	days.setTexture(TextureManager::instance().getTexture("assets/day.png"));
	days.setPosition(WIDTH - days.getTextureRect().width, 0);

	sm.init(sf::Vector2f(WIDTH, HEIGHT));

	DialoguePanel::instance().loadResources();
	DialoguePanel::instance().init();

	pm.init("assets/smoke.png");

	state = MAP;
}

void Scene::init(std::string name, tmx::MapLoader &ml)
{
	//Constructor
	ml.Load(name);
	map = &ml;

	loadingText = sf::Text("Loading", DFont::instance().getFont(), 15);
	loadingText.setPosition(HALF_WIDTH - (loadingText.getGlobalBounds().width / 2), HALF_HEIGHT - (loadingText.getGlobalBounds().height / 2));

	//Start loading
	state = LOADING;
	std::thread loadingThread(&Scene::loadResources, this);
	loadingThread.join();

	//Create render texture
	finalTexture.create(WIDTH, HEIGHT);

	camStart = camera.getCenter();
}

void Scene::endBattle()
{
	for (auto i = squads.begin(); i != squads.end();)
	{
		Squad &squad = *i;
		if (squadToDelete.getOnMap().GetPosition() == squad.getOnMap().GetPosition())
		{
			squadToDelete.getOnMap().SetName("null");
			i = squads.erase(i);
		}
		else
		{
			i++;
		}
	}
	battle.clean();

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

void Scene::updateFixed(sf::Time time)
{
	if (state != LOADING)
	{
		DialoguePanel::instance().update(); 
		
		if (state == BATTLE)
		{
			battle.update(time);
		}

		if (state == TRANSITION)
		{
			if (!sm.isWorking())
			{
				state = BATTLE;
			}
		}
	}
	else //loading screen
	{
		if (counter == 1) loadingText.setString("Loading");
		else if (counter % 10 == 0) loadingText.setString("Loading.");
		else if (counter % 15 == 0) loadingText.setString("Loading..");
		else if (counter % 20 == 0) loadingText.setString("Loading...");
		else if (counter >= 20) counter = 0;
		counter++;
	}
}

void Scene::update(sf::Time time)
{
	
	if (state != LOADING)
	{
		if (state == MAP)
		{
			for (auto i = squads.begin(); i != squads.end(); ++i)
			{
				i->update(time);
			}	
			
			po.update(time);

			//Scrolling
			if (po.getSprite().getPosition().y > camera.getCenter().y + (HALF_HEIGHT / 2) - (CHARACTER_SIZE / 2))
			{
				oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_IN, 1.0f, &ncy, ncy + HALF_HEIGHT);
			}
			if (po.getSprite().getPosition().y < camera.getCenter().y - (HALF_HEIGHT / 2) - (CHARACTER_SIZE / 2))
			{
				oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_IN, 1.0f, &ncy, ncy - HALF_HEIGHT);
			}
			if (po.getSprite().getPosition().x > camera.getCenter().x + (HALF_WIDTH / 2) - (CHARACTER_SIZE / 2))
			{
				oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_IN, 1.0f, &ncx, ncx + HALF_WIDTH);
			}
			if (po.getSprite().getPosition().x < camera.getCenter().x - (HALF_WIDTH / 2) - (CHARACTER_SIZE / 2))
			{
				oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_IN, 1.0f, &ncx, ncx - HALF_WIDTH);
			}
			if ((int)ncy % HALF_HEIGHT != 0)
			{
				camera.setCenter(camera.getCenter().x, camStart.y + ncy);
			}
			if ((int)ncx % HALF_WIDTH != 0)
			{
				camera.setCenter(camStart.x + ncx, camera.getCenter().y);
			}
			oTweener.step(time.asSeconds());

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
							DialoguePanel::instance().openDialogue(npc.getOnMap().GetName(), "day1");

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

void Scene::draw(sf::RenderTarget &tg)
{
	if (state != LOADING)
	{
		finalTexture.clear(sf::Color(24u, 19u, 27u));

		//Draw map and hero
		if (state != BATTLE)
		{
			//Game content
			finalTexture.setView(camera);
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

			//Unscalable
			finalTexture.draw(vingette);
			finalTexture.setView(unscalable);

			//Tips, only if hero is not moving
			if (!po.isWalking())
			{
				for (auto i = tips.begin(); i != tips.end(); ++i)
				{
					i->draw(finalTexture, sf::Vector2f(970, 640));
				}
			}

			xpbar.draw(finalTexture);

			//Dialogue UI
			DialoguePanel::instance().draw(finalTexture);
			finalTexture.draw(days);
		}
		else //Draw battle
		{
			//Scaled
			finalTexture.setView(camera);
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
		tg.draw(loadingText);
	}
}

std::vector<tmx::MapObject> &Scene::getObjects()
{
	return map->GetLayers().back().objects;
}

void Scene::startBattle(Squad &squad)
{
	transitionClock.restart();
	state = TRANSITION;

	battle.start(squad);

	squadToDelete = squad;
}

void Scene::input(sf::Event &event)
{
	if (state != LOADING)
	{
		if (state != BATTLE)
		{
			DialoguePanel::instance().input(event);
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) setCurrentEffect("distortion", sf::seconds(1));
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) setCurrentEffect("battle", sf::seconds(1));
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

void Scene::setCurrentEffect(std::string string, sf::Time time)
{
	sm.setCurrentEffect(string, time);
}

void SceneManager::setScene(std::string name, tmx::MapLoader &ml)
{
	current.init(name, ml);
}

void SceneManager::draw(sf::RenderTarget &rt)
{
	current.draw(rt);
}

void SceneManager::update(sf::Time time)
{
	current.update(time);
}

void SceneManager::updateFixed(sf::Time time)
{
	current.updateFixed(time);
}

void SceneManager::input(sf::Event &event)
{
	current.input(event);
}

void SceneManager::initBattle(Squad &squad)
{
	current.setPaused(true);
	current.setCurrentEffect("battle", sf::seconds(0.97));
	current.startBattle(squad);
}

void SceneManager::endBattle()
{
	current.endBattle();
}