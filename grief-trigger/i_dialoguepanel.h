#pragma once

#include <list>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <dbtweener.h>

#include "d_dialogueinfo.h"

class DialoguePanel
{
protected:
	//Font size
	static const unsigned int fontSize = 40;

	//Buffer for typewriter sound
	sf::SoundBuffer clickBuffer;
	sf::Sound click;

	sf::SoundBuffer enterBuffer;
	sf::Sound enter;

	//Pointer to dialogue data
	DialogueInfo *di;

	//Simple shape pointer
	sf::RectangleShape pointer;

	//Default clock
	sf::Clock clock;

	//Default font
	sf::Font font;

	//Current drawable text
	sf::Text text;

	//Answers list
	std::vector<sf::Text> answers;

	//Background panel
	sf::Sprite background;
	sf::Texture backgroundTexture;
	//Tween variable
	float nby;

	//Portrait
	sf::Sprite art;
	sf::Texture artTexture;

	//Current text
	std::string actualString;

	//Next string
	std::string nextString;

	//Last used name and situation
	std::string lastName;
	std::string lastSituation;

	//Number of letter
	unsigned int character;

	//Number of selected answer
	int selected;

	//Is string animation ended
	bool ended;

	//Is dialogue panel visible
	bool visible;

	//Is answering state
	bool isAnswering;

	//Tweener itself
	CDBTweener oTweener;

public:
	DialoguePanel();
	void init(DialogueInfo &dinfo);
	void openDialogue(std::string name, std::string situation);
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

