#pragma once

#include <iostream>
#include <list>
#include <string>

#include <SFML/Graphics.hpp>

class EffectManager
{
private:
	//Effect shader
	sf::Shader shader;

	//Window size
	sf::Vector2f size;

	//Timer
	sf::Clock clock;
	sf::Time duration;

	//Seconds counter
	unsigned int counter;

	//Is working
	bool working;
public:
	EffectManager(sf::Vector2f resolution);
	void update();
	void draw(sf::RenderTexture &rt, sf::RenderTarget &target);
	sf::Shader& getCurrentEffect();
	void setCurrentEffect(std::string name, sf::Time time);
	bool isWorking() {return working;};
};