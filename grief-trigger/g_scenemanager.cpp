#include "g_scenemanager.h"

#include "h_config.h"

Scene::Scene()
{
}

void Scene::init(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml)
{
	ml.Load(name);
	map = &ml;
	camera = cam;
	unscalable = uns;

	po.init(128, 128);

	ef.init(sf::Vector2f(WIDTH, HEIGHT));

	di.init("text.xml");

	dp.init(di);

	finalTexture.create(WIDTH, HEIGHT);
}

void Scene::update(sf::Time &time)
{
	ef.update();
	dp.update();
	po.move(map->GetLayers().back().objects, dp);
	po.update(time, *camera);
}

void Scene::draw(sf::RenderTarget &tg)
{
	finalTexture.clear();

	//Game content
	finalTexture.setView(*camera);
	map->Draw(finalTexture, false);
	po.draw(finalTexture);

	//Unscalable
	finalTexture.setView(*unscalable);
	dp.draw(finalTexture);

	finalTexture.display();

	ef.draw(finalTexture, tg);
}

void Scene::input(sf::Event &event)
{
	dp.input(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) ef.setCurrentEffect("distortion", sf::seconds(1));
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) ef.setCurrentEffect("shader", sf::seconds(1));
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) finalTexture.getTexture().copyToImage().saveToFile("screenshot.png");
}

SceneManager::SceneManager(std::string name, sf::View *cam, sf::View *uns, tmx::MapLoader &ml)
{
	current.init(name, cam, uns, ml);
}

void SceneManager::draw(sf::RenderTarget &rt)
{
	current.draw(rt);
}

void SceneManager::update(sf::Time &time)
{
	current.update(time);
}

void SceneManager::input(sf::Event &event)
{
	current.input(event);
}