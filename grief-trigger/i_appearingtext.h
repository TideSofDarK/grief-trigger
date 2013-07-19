#pragma once

#include <list>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "d_dialogueinfo.h"
#include "RoundedRectangleShape.hpp"

class AppearingText
{
protected:
	static const unsigned int fontSize = 40;

	DialogueInfo *di;

	sf::RectangleShape pointer;

	sf::Clock clock;
	sf::Font font;
	sf::Text text;
	std::vector<sf::Text> answers;

	sf::Sprite background;
	sf::Texture backgroundTexture;

	sf::Sprite art;
	sf::Texture artTexture;

	std::string actualString;

	std::string lastName;
	std::string lastSituation;

	unsigned int character;
	int selected;
	bool ended;
	bool visible;
	bool isAnswering;

public:
	AppearingText(DialogueInfo &dinfo);
	void reset(std::string name, std::string situation);
	void update();
	void input(sf::Event &event);
	void stop();
	void draw(sf::RenderTarget &rt);
	void hide();
	void next();
	bool showAnswers();
	void applyAnswer(unsigned int number);
	bool isHided();
	bool isEnded();
};

