#include "i_appearingtext.h"

AppearingText::AppearingText(std::string message)
{
	font.loadFromFile("assets/fonts/default.ttf");

	reset(message);
}

void AppearingText::stop()
{
	text.setString(actualString);
	ended = true;
	character = actualString.length();
}

void AppearingText::reset(std::string message)
{
	if (ended == false && text.getString().toAnsiString().c_str() != actualString.c_str())
	{
		stop();
	}
	else
	{
		text = sf::Text("", font, 24);

		actualString = message;

		character = 0;

		clock.restart();

		ended = false;
	}
}

void AppearingText::update()
{
	if (clock.getElapsedTime().asMilliseconds() > 30 && character < actualString.length())
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

void AppearingText::draw(sf::RenderTarget &rt)
{
	rt.draw(text);
}