#include "i_battleui.h"

Logger::Logger()
{

}

void Logger::init(sf::Vector2f pos)
{
	font.loadFromFile(fontPath);
	text = sf::Text();
	text.setPosition(pos);
	text.setString("");
	text.setFont(font);
	text.setCharacterSize(33);
	text.setColor(sf::Color::Black);
	ended = true;
	read = true;
	character = 0;
}

void Logger::draw(sf::RenderTarget &tg)
{
	tg.draw(text);
}

void Logger::update(sf::Time time)
{
	if (ended == false)
	{
		if (clock.getElapsedTime().asMilliseconds() > 60 && character < actualString.length())
		{
			if (text.getString().toAnsiString().c_str()[text.getString().getSize()] != ' ')
			{
				SoundManager::instance().playClickSound();
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
}

void Logger::input(sf::Event &event)
{
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) 
	{
		SoundManager::instance().playEnterSound();
		if (ended) read = true;
		else stop();
	}
}

void Logger::stop()
{
	text.setString(actualString);
	ended = true;
	character = actualString.length();
}

void Logger::setString(std::wstring str)
{
	if (ended) 
	{
		ended = false;
		read = false;
		actualString = str;
		character = 0;
		clock.restart();
	}
}

Damage::Damage(sf::Vector2f pos, std::string str, sf::Font &font)
{
	text = sf::Text(str, font, 80);
	text.setColor(sf::Color::Red);
	text.setPosition(pos);

	active = true;

	ny = pos.y;
	na = 255;
	dirY = pos.y - 100;
}

void Damage::draw(sf::RenderTarget &tg)
{
	if (active)
	{
		text.setColor(sf::Color(255u, 255u, 255u, na));
		text.move(1,0);
		tg.draw(text);

		text.move(-2,0);
		tg.draw(text);

		text.move(1,0);

		text.move(0,1);
		tg.draw(text);

		text.move(0,-2);
		tg.draw(text);

		text.move(0,1);

		text.setColor(sf::Color(255u, 0, 0, na));
		tg.draw(text);
	}
}

void Damage::update(sf::Time time)
{
	if (active)
	{
		if (na > 0)
		{
			na -= 5.0;
		}

		if (ny > dirY)
		{
			ny -= 5.0;
		}

		if (na <= 0)
		{
			changeActive();
		}

		text.setPosition(text.getPosition().x, ny);
		text.setColor(sf::Color(255, 255, 255, na));
	}
}

Bar::Bar()
{
}

void Bar::init(bool vert, sf::Vector2f pos, unsigned int &var, sf::Color color)
{
	variable = &var;
	maxVariable = var;
	vertical = vert;

	if (!vertical)
	{
		shape = sf::RectangleShape(sf::Vector2f(SHAPE_WIDTH, SHAPE_HEIGHT));
		bar = sf::RectangleShape(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
		shape.setPosition(pos);
		bar.setPosition(pos.x + OFFSET, pos.y + OFFSET);
	}
	else
	{
		shape = sf::RectangleShape(sf::Vector2f(SHAPE_HEIGHT / 2, SHAPE_WIDTH));
		bar = sf::RectangleShape(sf::Vector2f(BAR_HEIGHT/ 2, BAR_WIDTH));
		shape.setPosition(pos);
		shape.move(0, SHAPE_WIDTH / 2);		
		bar.setPosition(shape.getPosition().x + OFFSET, shape.getPosition().y + OFFSET);	
	}

	bar.setFillColor(color);
	shape.setFillColor(sf::Color::White);

	bar.setOutlineColor(sf::Color::Black);
	bar.setOutlineThickness(1.0);
}

void Bar::draw(sf::RenderTarget &tg)
{
	tg.draw(shape);
	tg.draw(bar);
}

void Bar::update(sf::Time time, sf::Vector2f np)
{
	float t = *variable;
	if (t > 0)
	{
		double p = (t / maxVariable) * 100.0;

		if (np != sf::Vector2f())
		{
			bar.setPosition(sf::Vector2f(np.x + OFFSET - OFFSET, np.y + (BAR_WIDTH / 2) + OFFSET)); // Im tired so enjoy shit
			shape.setPosition(sf::Vector2f(np.x - OFFSET, np.y + (BAR_WIDTH / 2)));
		}

		if(!vertical)
		{
			bar.setSize(sf::Vector2f(((BAR_WIDTH / 100) * p) + OFFSET, BAR_HEIGHT)); 
			shape.setSize(sf::Vector2f(bar.getSize().x + (OFFSET * 2), SHAPE_HEIGHT));
		}
		else
		{
			bar.setSize(sf::Vector2f((BAR_HEIGHT / 2) - OFFSET, ((BAR_WIDTH / 100) * p) + OFFSET)); 
			shape.setSize(sf::Vector2f(SHAPE_HEIGHT/ 2, bar.getSize().y + (OFFSET * 2)));
		}
	}
	else if (t <= 0)
	{
		if(!vertical)
		{
			bar.setSize(sf::Vector2f(((BAR_WIDTH / 100) * 1) + OFFSET, BAR_HEIGHT)); 
			shape.setSize(sf::Vector2f(bar.getSize().x + (OFFSET * 2), SHAPE_HEIGHT));
		}
		else
		{
			bar.setSize(sf::Vector2f((BAR_HEIGHT / 2) - OFFSET, ((BAR_WIDTH / 100) * 1) + OFFSET)); 
			shape.setSize(sf::Vector2f(SHAPE_HEIGHT/ 2, bar.getSize().y + (OFFSET * 2)));
		}
		bar.setFillColor(sf::Color::Black);
	}
}

Menu::Menu()
{
	back.setTexture(TextureManager::instance().getTexture("assets/menu.png"));
}

void Menu::init(std::vector<std::wstring> newItems)
{
	font.loadFromFile("assets/fonts/default.TTF");

	items.clear();
	for (auto i = newItems.begin(); i != newItems.end(); i++)
	{
		sf::String s(*i);

		sf::Text text(s, font, 20);
		text.setColor(sf::Color::Black);
		text.setPosition(0, (i - newItems.begin()) * 20);
		items.push_back(text);
	}

	initialized = true;
	working = false;

	selector = sf::RectangleShape(sf::Vector2f());
	selector.setFillColor(sf::Color::Blue);

	selected = NOT_SELECTED;
}

void Menu::update(sf::Time time)
{
	if (items.size() != 0 && working)
	{
		selector.setSize(sf::Vector2f(items[selection].getGlobalBounds().width, items[selection].getGlobalBounds().height));
		selector.setPosition(items[selection].getPosition().x, items[selection].getPosition().y + (items[selection].getGlobalBounds().height / 3) - 2);
	}
}

void Menu::draw(sf::RenderTarget &tg)
{
	if (working)
	{
		tg.draw(back);
		tg.draw(selector);
		for (auto i = items.begin(); i != items.end(); i++)
		{
			if (i - items.begin() == selection) i->setColor(sf::Color::White);
			else i->setColor(sf::Color::Black);
			tg.draw(*i);
		}	
	}
}

void Menu::input(sf::Event &event)
{
	if (working)
	{
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		{
			select();
			disappear();
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
		{
			if (selection > 0)
			{
				selection--;
			}
			else
			{
				selection = items.size() - 1;
			}

			SoundManager::instance().playSelectSound();
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
		{
			if (selection + 1 < items.size())
			{
				selection++;
			}
			else
			{
				selection = 0;
			}

			SoundManager::instance().playSelectSound();
		}
	}
}

void Menu::appear(sf::Vector2f pos)
{
	working = true;
	back.setPosition(pos.x, (pos.y * 2) + 150);
	for (auto i = items.begin(); i != items.end(); i++)
	{
		i->setPosition(pos.x + 20, ((back.getPosition().y) + (i - items.begin()) * 20) + 20);
	}
}

void Menu::disappear()
{
	if (working)
	{
		working = false;
	}
}

void Menu::select()
{
	selected = selection;
	working = false;
}