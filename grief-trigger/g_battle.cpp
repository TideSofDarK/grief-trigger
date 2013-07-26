#include "g_battle.h"

#include "h_config.h"

Bar::Bar()
{

}

Battle::Battle()
{

}

void Battle::loadResources(std::string fileName)
{
	//Load resources list file
	pugi::xml_parse_result result = doc.load_file((fileName).c_str());

	std::string resourcesList = doc.child("monsters").child_value();

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
}

void Battle::start(Squad &squad_)
{
	squad = squad_;

	std::vector<Monster> &list = squad.getMonsters();

	sf::FloatRect bounds(HALF_WIDTH / 4, HALF_HEIGHT / 7, (HALF_WIDTH / 4) * 3, (HALF_HEIGHT / 5) * 3);

	for (auto i = list.begin(); i != list.end(); i++)
	{
		sf::Sprite sprite;
		sprite.setTexture(TextureManager::instance().getTexture("assets/" + i->getName() + ".png"));
		sprite.setPosition(((HALF_WIDTH) - (sprite.getTextureRect().width * (list.size() + 1))) + (sprite.getTextureRect().width * (i - list.begin())), bounds.top + ((((i - list.begin()) % 2)) ? 10 : -10));
		monsters.push_back(sprite);
	}

	selected = 0;

	state = AI;
}

void Battle::draw(sf::RenderTarget &tg)
{
	for (auto i = monsters.begin(); i != monsters.end(); ++i)
	{
		tg.draw(*i);
	}
}

void Battle::update(sf::Time time)
{
	for (auto i = monsters.begin(); i != monsters.end(); ++i)
	{

	}
}

void Battle::input(sf::Event &event)
{
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
	{
		if (selected > 0)
		{
			selected--;
		}
		else
		{
			selected = monsters.size() - 1;
		}

		std::cout << std::to_string(selected) << std::endl;
	}
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
	{
		if (selected + 1 < monsters.size())
		{
			selected++;
		}
		else
		{
			selected = 0;
		}

		std::cout << std::to_string(selected) << std::endl;
	}
}