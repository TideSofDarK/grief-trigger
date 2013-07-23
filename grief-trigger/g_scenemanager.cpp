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
			camera->setCenter(object.GetPosition().x + CHARACTER_SIZE, object.GetPosition().y + (CHARACTER_SIZE / 2));

			//Init hero object
			po.init(object.GetPosition().x, object.GetPosition().y, camera->getCenter());
		}
		else if (object.GetName() == "squad")
		{
			Squad newSquad;
			newSquad.init(object.GetPropertyString("monsters"), object.GetPosition());
			squads.push_back(newSquad);
		}
	}

	sm.init(sf::Vector2f(WIDTH, HEIGHT));

	di.init("text.xml");

	dp.loadResources();
	dp.init(di);

	pm.init("assets/smoke.png");

	loaded = true;
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
}

void Scene::update(sf::Time time)
{
	if (loaded)
	{
		pm.update(time);
		for (auto i = squads.begin(); i != squads.end(); ++i)
		{
			i->update(time, map->GetLayers().back().objects);
		}
		sm.update();
		dp.update();
		po.move(map->GetLayers().back().objects, dp);
		po.update(time, *camera);
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
}

void Scene::draw(sf::RenderTarget &tg)
{
	if (loaded)
	{
		finalTexture.clear(sf::Color(24u, 19u, 27u));

		//Particle effect under game map
		pm.drawUnder(finalTexture);

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

		//Unscalable
		finalTexture.setView(*unscalable);
		//Particles
		pm.draw(finalTexture);
		//Dialogue UI
		dp.draw(finalTexture);

		finalTexture.display();

		//Draw through shader manager
		sm.draw(finalTexture, tg);
	}
	else
	{
		tg.draw(loadingText);
	}
}

void Scene::input(sf::Event &event)
{
	if (loaded)
	{
		dp.input(event);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) sm.setCurrentEffect("distortion", sf::seconds(1));
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) sm.setCurrentEffect("rgb", sf::seconds(1));
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) finalTexture.getTexture().copyToImage().saveToFile("screenshot.png");
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) init("test.tmx", camera, unscalable, *map);
	}
}

SceneManager::SceneManager(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml)
{
	current.init(name, cam, uns, ml);
}

void SceneManager::setScene(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml)
{
	current.init(name, cam, uns, ml);
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