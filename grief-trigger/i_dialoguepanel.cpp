#include "i_dialoguepanel.h"

AppearingText::AppearingText(DialogueInfo &dinfo)
{
	di = &dinfo;

	font.loadFromFile("assets/fonts/default.ttf");

	//Set up tween for background panel
	nby = 0;

	backgroundTexture.loadFromFile("assets/dbox.png");
	background.setTexture(backgroundTexture);
	background.setPosition(0, nby);

	pointer = sf::RectangleShape();
	pointer.setFillColor(sf::Color::Blue);

	hide();
}

void AppearingText::stop()
{
	text.setString(actualString);
	ended = true;
	character = actualString.length();
}

std::string wordWrap( std::string str, size_t width = 50 ) {
	size_t curWidth = width;
	while( curWidth < str.length() ) {
		std::string::size_type spacePos = str.rfind( ' ', curWidth );
		if( spacePos == std::string::npos )
			spacePos = str.find( ' ', curWidth );
		if( spacePos != std::string::npos ) {
			str[ spacePos ] = '\n';
			curWidth = spacePos + width + 1;
		}
	}

	return str;
}

void AppearingText::reset(std::string name, std::string situation)
{
	selected = 0;
	isAnswering = false;
	lastName = name;
	lastSituation = situation + '/';

	if (ended == false && text.getString().toAnsiString().c_str() != actualString.c_str())
	{
		stop();
	}
	else
	{
		text = sf::Text("", font, fontSize);

		text.setPosition(320, 455);

		artTexture.loadFromFile("assets/" + lastName + "_art.png");
		art.setTexture(artTexture);

		actualString = di->getDialogue(name, lastSituation);

		//Remove all newlines
		std::string::size_type pos = 0; 
		while ( ( pos = actualString.find ("\n",pos) ) != std::string::npos )
		{
			actualString.erase ( pos, 2 );
		}

		//Wrap words
		actualString = wordWrap(actualString);

		//Reset variables
		character = 0;
		clock.restart();
		ended = false;
		visible = true;
	}
}

bool AppearingText::showAnswers()
{

	//Get answers
	if (di->getAnswers(lastName, lastSituation) != "")
	{
		hide();
		actualString = di->getAnswers(lastName, lastSituation);
	}
	else
	{
		return false;
	}

	//Remove first newline
	std::string::size_type pos = 0; 
	pos = actualString.find ("\n",pos);
	actualString.erase ( pos, 1 );

	//Count answers
	int s = std::count(actualString.begin(), actualString.end(), '\n');

	std::vector<std::string> strings;

	//Add answers
	pos = 0;
	std::string token;
	while ((pos = actualString.find('\n')) != std::string::npos) {
		strings.push_back(actualString.substr(0, pos));
		actualString.erase(0, pos + 1);
	}

	for (int i = 0; i < s; i++)
	{
		sf::Text ans(strings[i], font, fontSize);
		ans.setPosition(320, (455 + i * fontSize) + i * 4);
		answers.push_back(ans);
	}

	isAnswering = true;

	ended = true;
	visible = true;

	return true;
}

void AppearingText::hide()
{
	selected = 0;
	isAnswering = false;
	visible = false;
	ended = true;
	text.setString("");
	actualString = "";
	if (answers.size() != 0)
	{
		answers.clear();
	}
}

void AppearingText::update()
{
	if (visible)
	{
		sf::Uint32 elapsed = clock.getElapsedTime().asMilliseconds();
		
		//Update tween if animation is not ended
		if (nby != 0)
		{

			background.setPosition(0, nby);
		}
		else
		{
			if (ended == false)
			{
				if (elapsed > 30 && character < actualString.length())
				{
					clock.restart();
					character++;
					text.setString( sf::String(actualString.substr(0, character)));
				}
				else if (character >= actualString.length())
				{
					stop();
				}
			}
			if (isAnswering)
			{
				pointer.setPosition(answers[selected].getPosition().x, answers[selected].getPosition().y + fontSize / 6);
				pointer.setSize(sf::Vector2f(answers[selected].getGlobalBounds().width, fontSize));
			}
		}
	}
}

void AppearingText::input(sf::Event &event)
{
	if (visible)
	{
		if (isAnswering)
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down && selected < answers.size() - 1)
			{
				selected++;
			}
			else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up && selected - 1 >= 0)
			{
				selected--;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) 
			{
				applyAnswer(selected);
			}
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && ended == true) 
		{
			if (showAnswers() == false)
			{
				hide();
			}
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && ended != true)
		{
			stop();
		}
	}
}

void AppearingText::applyAnswer(unsigned int number)
{
	reset(lastName, lastSituation + "speech" + std::to_string(number + 1));
}

void AppearingText::draw(sf::RenderTarget &rt)
{
	if (visible)
	{
		rt.draw(background);
		rt.draw(art);

		if (isAnswering)
		{
			rt.draw(pointer);

			for (auto i = answers.begin(); i != answers.end(); i++)
			{
				rt.draw(*i);
			}
		}
		else
		{
			rt.draw(text);
		}
	}
}

bool AppearingText::isHided()
{
	return visible;
}

bool AppearingText::isEnded()
{
	return ended;
}