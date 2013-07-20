#pragma once

#include <SFML/Graphics.hpp>

class SceneObject
{
protected: 
	sf::Texture texture;
public:
	void update(sf::Time &time);
	void draw(sf::RenderTarget &tg);
	void input(sf::Event &event);
};

