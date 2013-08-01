#include "i_dialoguepanel.h"

#include "d_parser.h"

bool DialoguePanel::loadResources(std::string fileName)
{
	//Load resources list file
	pugi::xml_parse_result result = doc.load_file((fileName).c_str());

	std::string resourcesList = doc.child("hires").child_value("dialogues");

	//Remove first newline
	std::string::size_type pos = 0; 
	pos = resourcesList.find ("\n",pos);
	resourcesList.erase ( pos, 1 );

	//Count resources
	int s = std::count(resourcesList.begin(), resourcesList.end(), '\n');

	std::vector<std::string> resourcesNames;

	//Add resources
	pos = 0;
	std::string token;
	while ((pos = resourcesList.find('\n')) != std::string::npos) {
		resourcesNames.push_back(resourcesList.substr(0, pos));
		resourcesList.erase(0, pos + 1);
	}

	for (int i = 0; i < s; i++)
	{
		TextureManager::instance().getTexture("assets/" + resourcesNames[i]);
	}
	return true;
}

void DialoguePanel::init()
{
	font.loadFromFile("assets/fonts/default.TTF");

	nby = HEIGHT / 2;
	background.setTexture(TextureManager::instance().getTexture("assets/dbox.png"));
	background.setPosition(0, nby);

	pointer = sf::RectangleShape();
	pointer.setFillColor(sf::Color::Blue);

	hide();
}

void DialoguePanel::stop()
{
	text.setString(actualString);
	ended = true;
	character = actualString.length();
}

std::wstring wordWrap( std::wstring str, size_t width = 50 ) {
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

void DialoguePanel::openDialogue(std::string name, std::string situation)
{
	selected = 0;
	isAnswering = false;

	if (ended == false && text.getString().toWideString().c_str() != actualString.c_str())
	{
		stop();
	}
	else
	{
		text = sf::Text("", font, fontSize);

		text.setPosition(320, 455);
		text.setColor(sf::Color::Black);

		//Load new art image
		if (lastName != name) {
			lastName = name;
			art.setTexture(TextureManager::instance().getTexture("assets/" + lastName + "_art.png"));
		}
		if (lastSituation != situation) lastSituation = situation + '/';

		//If next string is
		if (nextString != L"") 
		{
			actualString = nextString;
			nextString = L"";
		}
		else 
		{
			actualString = Parser::instance().parseDialogue(name, lastSituation);
		}

		//Replace all newlines
		while ( actualString.find (L"\n") != std::string::npos )
		{
			actualString.replace( actualString.find (L"\n"), 1 ,L" ");
		}

		//Wrap words
		actualString = wordWrap(actualString);

		//Check height
		size_t n = std::count(actualString.begin(), actualString.end(), '\n');
		const int maxLines = 3;
		if (n > maxLines)
		{
			//Find first space after last line
			unsigned int a = actualString.find(' ', maxLines*50);
			a++;

			//Cut all to next string
			nextString = actualString.substr(a, actualString.length());
			actualString.erase(a, actualString.length());
		}

		//Reset tweener
		if (!visible)
		{
			nby = HALF_HEIGHT;
			oTweener.addTween(&CDBTweener::TWEQ_ELASTIC, CDBTweener::TWEA_INOUT, 1.0f, &nby, 0.0f);

			nax = -640 / 2;
			oTweener.addTween(&CDBTweener::TWEQ_ELASTIC, CDBTweener::TWEA_INOUT, 0.5f, &nax, 0.0f);

			background.setPosition(0, nby);
			art.setPosition(nax, 0);
		}

		//Reset variables
		character = 0;
		clock.restart();
		ended = false;
		visible = true;
	}
}

bool DialoguePanel::showAnswers()
{
	//Get answers
	if (Parser::instance().parseAnswers(lastName, lastSituation).size() != 0)
	{
		//Clear all text
		text.setString("");
		answers.clear();
	}
	else
	{
		return false;
	}

	std::vector<std::wstring> strings = Parser::instance().parseAnswers(lastName, lastSituation);

	for (int i = 0; i < strings.size(); i++)
	{
		sf::Text ans(strings[i], font, fontSize);
		ans.setPosition(320, (455 + i * fontSize) + i * 4);
		ans.setColor(sf::Color::Black);
		answers.push_back(ans);
	}

	isAnswering = true;
	ended = true;
	visible = true;
	return true;
}

void DialoguePanel::hide()
{
	selected = 0;
	isAnswering = false;
	visible = false;
	ended = true;
	text.setString("");
	actualString = L"";
	answers.clear();
}

void DialoguePanel::update()
{
	if (visible)
	{
		sf::Uint32 elapsed = clock.getElapsedTime().asMilliseconds();

		//Update tween if animation is not ended
		if (nby != 0 || nax != 0)
		{
			oTweener.step(elapsed / 1000.f);
			background.setPosition(0, nby);
			art.setPosition(nax, 0);
			clock.restart();
		}
		else
		{
			if (ended == false)
			{
				if (elapsed > 60 && character < actualString.length())
				{
					//Play sound if it is not a space
					if (text.getString().toAnsiString().c_str()[text.getString().getSize()] != ' ')
					{
						SoundManager::instance().playClickSound();
					}
					if (text.getString().getSize() + 1 == actualString.size())
					{
						//If last - play enter sound
						SoundManager::instance().playEnterSound();
					}
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

void DialoguePanel::input(sf::Event &event)
{
	if (visible)
	{
		//Check if tween ended...
		if (nby == 0)
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
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::C) 
				{
					SoundManager::instance().playEnterSound();

					applyAnswer(selected);
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::C && ended == true) 
			{
				SoundManager::instance().playEnterSound();

				//Leave dialogue if no text
				if (nextString == L"")
				{
					if (showAnswers() == false) hide();
				}	
				else openDialogue(lastName, lastSituation);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::C && ended != true)
			{
				SoundManager::instance().playEnterSound();

				stop();
			}
		}
	}
}

void DialoguePanel::applyAnswer(unsigned int number)
{
	openDialogue(lastName, lastSituation + "speech" + std::to_string(number + 1));
}

void DialoguePanel::draw(sf::RenderTarget &rt)
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

bool DialoguePanel::isHided()
{
	return visible;
}

bool DialoguePanel::isEnded()
{
	return ended;
}