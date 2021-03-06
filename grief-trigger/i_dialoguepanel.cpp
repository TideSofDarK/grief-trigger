﻿#include "i_dialoguepanel.h"

#include "d_parser.h"
#include "d_gamedata.H"
#include "g_scenemanager.h"

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

	background.setTexture(TextureManager::instance().getTexture("assets/dbox.png"));

	for (int i = 0; i < s; i++)
	{
		TextureManager::instance().getTexture("assets/" + resourcesNames[i]);
	}
	return true;
}

void DialoguePanel::init()
{
	nby = HEIGHT / 2;
	background.setPosition(0, nby);

	pointer = sf::RectangleShape();
	pointer.setFillColor(sf::Color::Red);

	hide();

	nextS = false;
}

void DialoguePanel::stop()
{
	text.setString(actualString);
	ended = true;
	character = actualString.length();
}

std::wstring wordWrap( std::wstring str, size_t width = 40 ) {
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
	std::cout << "dsfsdfsdfsdfsdfsd\n";

	selected = 0;
	isAnswering = false;

	if (ended == false && text.getString().toWideString().c_str() != actualString.c_str())
	{
		stop();
	}
	else
	{
		text = sf::Text("", DFont::instance().getFont(), fontSize);

		text.setPosition(370, 535);
		text.setColor(sf::Color::Black);

		if (lastSituation != situation) lastSituation = situation + '/';
		//Load new art image
		if (lastName != name) {
			lastName = name;
		}

		if (Parser::instance().getPortrait(name, lastSituation) == "")
		{      
			t.create(400 ,HEIGHT);
			art.setTexture(t);
			art.setTextureRect(sf::IntRect(0,0,1, 1));
		}
		else
		{
			art.setTexture(TextureManager::instance().getTexture("assets/" + Parser::instance().getPortrait(name, lastSituation) + ".png"));
			art.setTextureRect(sf::IntRect(0,0,400, HEIGHT));
		}              

		if (Parser::instance().getIntBonus(name, lastSituation) != 0)
		{
			GameData::instance().getPlayer().addInt(Parser::instance().getIntBonus(name, lastSituation));

			if (Parser::instance().getIntBonus(name, lastSituation) > 0)
			{
				SoundManager::instance().playSuccessSound();
			}
			else
			{
				SoundManager::instance().playFailSound();
			}
		}
		if (Parser::instance().getSocialBonus(name, lastSituation) != 0)
		{
			GameData::instance().getPlayer().addSocial(Parser::instance().getSocialBonus(name, lastSituation));

			if (Parser::instance().getSocialBonus(name, lastSituation) > 0)
			{
				SoundManager::instance().playSuccessSound();
			}
			else
			{
				SoundManager::instance().playFailSound();
			}
		}

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
		const int maxLines = 2;
		if (n > maxLines)
		{
			//Find first space after last line
			unsigned int a = actualString.find(' ', maxLines*50);
			a++;

			//Cut all to next string
			nextString = actualString.substr(a, actualString.length());
			actualString.erase(a, actualString.length());
		}

		// | delim
		int p = std::find(actualString.begin(), actualString.end(), '|') - actualString.begin();
		actualString.erase(p, 1);
		std::wstring tstr = actualString.substr(p, actualString.size());
		actualString.erase(p, actualString.size());
		nextString = tstr + nextString;

		//Reset tweener
		if (!visible)
		{
			nby = HALF_HEIGHT;
			oTweener.addTween(&CDBTweener::TWEQ_ELASTIC, CDBTweener::TWEA_INOUT, 0.6f, &nby, 0.0f);

			nax = -400;
			oTweener.addTween(&CDBTweener::TWEQ_ELASTIC, CDBTweener::TWEA_INOUT, 0.6f, &nax, 0.0f);

			background.setPosition(0, nby);
			art.setPosition(nax, 0);
		}

		background.setColor(sf::Color(255,255,255,0));
		//art.setColor(sf::Color(255,255,255,0));

		if (Parser::instance().isLast(name, lastSituation) == true)
		{
			nextS = true;
		}
		else
		{
			nextS = false;
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
		sf::Text ans(strings[i], DFont::instance().getFont(), fontSize);
		ans.setPosition(370, (535 + i * fontSize) + i * 4);
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
	if (Parser::instance().isLast(lastName, lastSituation))
	{
		Level::instance().nextScene();
		SceneManager::instance().startTransition(Parser::instance().getSceneInfo(Level::instance().getDay(), Level::instance().getScene()));
	}

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
		art.setColor(sf::Color(255,255,255,interpolateLinear(art.getColor().a, 255, 0.2f)));
		background.setColor(sf::Color(255,255,255,interpolateLinear(background.getColor().a, 255, 0.2f)));

		sf::Uint32 elapsed = clock.getElapsedTime().asMilliseconds();

		//Update tween if animation is not ended
		if (nby != 0 || nax != 0)
		{
			oTweener.step(elapsed / 1000.f);
			background.setPosition(0, nby);
			art.setPosition(nax, 0);
			clock.restart();
			tip.setString(L"");
		}
		else
		{
			tip.setString(L"Остановить");
			if (ended == false)
			{
				tip.setString(L"Продолжить");
				if (elapsed > 60 && character < actualString.length())
				{
					//Play sound if it is not a space
					if (text.getString().toAnsiString().c_str()[text.getString().getSize()] != ' ' && character % 2 == 0)
					{
						SoundManager::instance().playClickSound();
					}
					if (text.getString().getSize() + 1 >= actualString.size())
					{
						//If last - play enter sound
						SoundManager::instance().playEnterSound();
					}
					clock.restart();
					character+=2;
					text.setString( sf::String(actualString.substr(0, character)));				
				}
				else if (character >= actualString.length())
				{
					stop();
				}
			}
			if (isAnswering)
			{
				tip.setString(L"Ответить");
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
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) 
				{
					SoundManager::instance().playEnterSound();

					applyAnswer(selected);
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X && ended == true) 
			{
				SoundManager::instance().playEnterSound();

				//Leave dialogue if no text
				if (nextString == L"")
				{
					//std::cout << lastSituation + "\n";

					hide();

					if (showAnswers() == false) 
					{
						if (Parser::instance().goNext(lastName, lastSituation) != "")
						{
							openDialogue(lastName, Parser::instance().goNext(lastName, lastSituation));
						}
					}										
				}	
				else openDialogue(lastName, lastSituation);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X && ended != true)
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
			if (background.getPosition().y == 0.0)
			{
				rt.draw(text);
			}			
		}

		if (nby == 0 && nax == 0)
		{
			tip.draw(rt, sf::Vector2f(940, 701));
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