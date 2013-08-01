#include "g_scenemanager.h"

#include "h_config.h"

void Scene::loadResources()
{
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
			po.init(object.GetPosition().x, object.GetPosition().y, camera.getCenter(), object, doors, squads);
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
	}

	days.setTexture(TextureManager::instance().getTexture("assets/day.png"));
	days.setPosition(WIDTH - days.getTextureRect().width, 0);

	sm.init(sf::Vector2f(WIDTH, HEIGHT));

	DialoguePanel::instance().loadResources();
	DialoguePanel::instance().init();

	pm.init("assets/smoke.png");

	battle.init();

	state = MAP;
}

void Scene::init(std::string name, tmx::MapLoader &ml)
{
	//Constructor
	ml.Load(name);
	map = &ml;

	font.loadFromFile("assets/fonts/default.TTF");
	loadingText = sf::Text("Loading", font, 15);
	loadingText.setPosition(HALF_WIDTH - (loadingText.getGlobalBounds().width / 2), HALF_HEIGHT - (loadingText.getGlobalBounds().height / 2));

	//Start loading
	state = LOADING;
	std::thread loadingThread(&Scene::loadResources, this);
	loadingThread.join();
	
	//Create render texture
	finalTexture.create(WIDTH, HEIGHT);
}

void Scene::endBattle()
{
	for (auto i = squads.begin(); i != squads.end();)
	{
		Squad &squad = *i;
		if (squadToDelete.getOnMap().GetPosition() == squad.getOnMap().GetPosition())
		{
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

void Scene::update(sf::Time time)
{
	if (state != LOADING)
	{
		if (state == MAP)
		{
			//pm.update(time);
			for (auto i = squads.begin(); i != squads.end(); ++i)
			{
				i->update(time, map->GetLayers().back().objects);
			}	
			DialoguePanel::instance().update();			
			po.move(map->GetLayers().back().objects);
			po.update(time, camera);
		}

		//Always update shaders
		if (sm.isWorking()) sm.update();
	}
	else
	{
		if (counter == 1) loadingText.setString("Loading");
		else if (counter % 10 == 0) loadingText.setString("Loading.");
		else if (counter % 15 == 0) loadingText.setString("Loading..");
		else if (counter % 20 == 0) loadingText.setString("Loading...");
		else if (counter >= 20) counter = 0;
		counter++;
	}

	if (state == BATTLE) battle.update(time);

	if (state == TRANSITION)
	{
		if (!sm.isWorking())
		{
			state = BATTLE;
		}
	}
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

			//Unscalable
			finalTexture.setView(unscalable);

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