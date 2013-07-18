#pragma once

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

class AppearingText
{
private:
	sf::Clock clock;
	sf::Font font;
	sf::Text text;

	std::string actualString;

	unsigned int character;
	bool ended;

public:
	AppearingText(std::string message);
	void reset(std::string message);
	void update();
	void stop();
	void draw(sf::RenderTarget &rt);
};

