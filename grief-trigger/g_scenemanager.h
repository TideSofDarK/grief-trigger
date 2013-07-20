#pragma once

#include <vector>

#include <SFML//Graphics.hpp>

class Scene
{
public:
	Scene(void);
};

class SceneManager
{
private:
	std::vector<Scene*> scenes;
public:
	SceneManager(void);
	void draw(sf::RenderTarget &rt);
	void update(sf::Time &time);
};

