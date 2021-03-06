#ifndef DIALOGUEPANEL_INCLUDE
#define DIALOGUEPANEL_INCLUDE

#include <list>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Thor/Resources.hpp>
#include <Thor/Graphics.hpp>

#include <pugixml/pugixml.hpp>

#include "os_dbtweener.h"

#include "h_config.h"
#include "d_resourcesmanager.h"
#include "i_ui.h"

class DialoguePanel
{
public:
	static DialoguePanel& instance()
	{
		static DialoguePanel theSingleInstance;
		return theSingleInstance;
	}

private:
	//Font size
	static const unsigned int fontSize =		40;

	//resource.xml parsing
	pugi::xml_document doc;

	//Simple shape pointer
	sf::RectangleShape							pointer;

	Tip											tip;

	//Default clock
	sf::Clock									clock;

	//Current drawable text
	sf::Text									text;

	//Answers list
	std::vector<sf::Text>						answers;

	sf::Texture t;

	//Background panel
	sf::Sprite									background;
	//Tween variable
	float										nby;

	//Portrait
	sf::Sprite									art;
	//Tween variable
	float										nax;

	//Current text
	std::wstring								actualString;

	//Next string
	std::wstring								nextString;

	//Last used name and situation
	std::string									lastName;
	std::string									lastSituation;

	//Number of letter
	unsigned int								character;

	//Number of selected answer
	int											selected;

	//Is string animation ended
	bool										ended;

	//Is dialogue panel visible
	bool										visible;

	//Is answering state
	bool										isAnswering;

	//Tweener itself
	CDBTweener									oTweener;

	bool										nextS;

private:
	DialoguePanel() : tip(L"Продолжить", _X, "black") {};
	DialoguePanel( const DialoguePanel& );
	DialoguePanel& operator =( const DialoguePanel& );

public:
	void init();
	void clean()
	{
		lastName ="";
		lastSituation = "";
		sf::Texture t;
		art.setTexture(t);
	};
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
	bool loadResources(std::string fileName = "assets/resources.xml");
};

#endif