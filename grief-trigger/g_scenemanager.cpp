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
			camera->setCenter(0 + (HALF_WIDTH / 2), 0 + (HALF_HEIGHT / 2));

			//Init hero object
			po.init(object.GetPosition().x, object.GetPosition().y, camera->getCenter(), object, doors, squads);
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

	sm.init(sf::Vector2f(WIDTH, HEIGHT));

	DialoguePanel::instance().loadResources();
	DialoguePanel::instance().init();

	pm.init("assets/smoke.png");

	battle.init();

	loaded = true;
	isBattle = false;
}

Scene::Scene() : loadingThread(&Scene::loadResources, this)
{
}

void Scene::init(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml)
{
	//Constructor
	ml.Load(name);
	map = &ml;
	camera = cam;
	unscalable = uns;

	font.loadFromFile("assets/fonts/default.TTF");
	loadingText = sf::Text("Loading", font, 15);
	loadingText.setPosition(HALF_WIDTH - (loadingText.getGlobalBounds().width / 2), HALF_HEIGHT - (loadingText.getGlobalBounds().height / 2));

	//Start loading
	loaded = false;
	loadingThread.launch();

	//Create render texture
	finalTexture.create(WIDTH, HEIGHT);

	paused = false;
	transition = false;
}

void Scene::update(sf::Time time)
{
	if (loaded)
	{
		if (!paused && !transition)
		{
			if (!isBattle)
			{
				//pm.update(time);
				for (auto i = squads.begin(); i != squads.end(); ++i)
				{
					i->update(time, map->GetLayers().back().objects);
				}	
				if (!DialoguePanel::instance().isHided()) DialoguePanel::instance().update();			
				po.move(map->GetLayers().back().objects);
				po.update(time, *camera);
			}
		}
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
	if (isBattle) battle.update(time);
}

void Scene::draw(sf::RenderTarget &tg)
{
	if (transition)
	{
		if (!sm.isWorking())
		{
			isBattle = true;
			transition = false;
		}
	}

	if (loaded)
	{
		finalTexture.clear(sf::Color(24u, 19u, 27u));

		//Draw map and hero
		if (!isBattle)
		{
			//Particle effect under game map
			//pm.drawUnder(finalTexture);

			//Game content
			finalTexture.setView(*camera);
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
			finalTexture.setView(*unscalable);
			//Particles
			//pm.draw(finalTexture);
			//Dialogue UI
			DialoguePanel::instance().draw(finalTexture);
		}
		else //Draw battle
		{
			//Scaled
			finalTexture.setView(*camera);
			battle.draw(finalTexture);

			//Unscalable
			finalTexture.setView(*unscalable);
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

void Scene::startBattle(Squad squad)
{
	transitionClock.restart();
	transition = true;

	battle.start(squad);
}

void Scene::input(sf::Event &event)
{
	if (loaded)
	{
		if (!isBattle)
		{
			DialoguePanel::instance().input(event);
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) setCurrentEffect("distortion", sf::seconds(1));
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) setCurrentEffect("battle", sf::seconds(1));
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) finalTexture.getTexture().copyToImage().saveToFile("screenshot.png");

			if (!paused)
			{
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V) paused = true;
			}
			else
			{
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V) paused = false;
			}
		}
		else if (isBattle)
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
	current.init(name, &camera, &unscalable, ml);
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
	current.setCurrentEffect("battle", sf::seconds(1));
	current.startBattle(squad);
}