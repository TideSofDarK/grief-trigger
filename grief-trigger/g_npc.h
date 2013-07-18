#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "g_sceneobject.h"

class NPC : SceneObject
{
private:
	sf::Sprite sprite;

public:
	NPC(std::string name, sf::Uint32 x, sf::Uint32 y);
	void update(sf::Time &time);
	void draw(sf::RenderTarget &tg);
};

