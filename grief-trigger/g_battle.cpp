#include "g_battle.h"

#include "h_config.h"

#define SHAPE_WIDTH 180.0
#define BAR_WIDTH 170.0

#define SHAPE_HEIGHT 18.0
#define BAR_HEIGHT 8.0

#define OFFSET 5.0

Damage::Damage(sf::Vector2f pos, std::string str, sf::Font &font)
{
	text = sf::Text(str, font, 50);
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
		shape = sf::RectangleShape(sf::Vector2f(SHAPE_HEIGHT, SHAPE_WIDTH));
		bar = sf::RectangleShape(sf::Vector2f(BAR_HEIGHT, BAR_WIDTH));
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
		bar.setSize(sf::Vector2f(BAR_HEIGHT, ((BAR_WIDTH / 100) * p) + OFFSET)); 
		shape.setSize(sf::Vector2f(SHAPE_HEIGHT, bar.getSize().y + (OFFSET * 2)));
	}
}

Battle::Battle()
{
}

void Battle::loadResources(std::string fileName)
{
	std::cout << std::to_string(GameData::instance().getPlayer().getHP()) << std::endl;

	font.loadFromFile("assets/fonts/default.TTF");

	//Some shitty code here
	//At least it is fast lol
	playerSprite.setTexture(TextureManager::instance().getTexture("assets/girl_art.png"));
	playerSprite.setTextureRect(sf::IntRect(0, 0, playerSprite.getLocalBounds().width, playerSprite.getLocalBounds().height / 1.5));
	playerSprite.scale(0.5, 0.5);
	playerSprite.setPosition(0, HEIGHT - (playerSprite.getTextureRect().height / 2));
	playerHPBar.init(false, sf::Vector2f(playerSprite.getPosition().x + 90, playerSprite.getPosition().y + (playerSprite.getTextureRect().height / 2) - (18 * 3) - 2), 
		GameData::instance().getPlayer().getHP());
	playerManaBar.init(false, sf::Vector2f(playerSprite.getPosition().x + 90, playerSprite.getPosition().y + (playerSprite.getTextureRect().height / 2) - (18 * 2) + 2), 
		GameData::instance().getPlayer().getMP(), sf::Color(105, 186, 199));

	emberSprite.setTexture(TextureManager::instance().getTexture("assets/red_art.png"));
	emberSprite.setTextureRect(sf::IntRect(0, 0, emberSprite.getLocalBounds().width, emberSprite.getLocalBounds().height / 1.5));
	emberSprite.scale(0.5, 0.5);
	emberSprite.setPosition(332, HEIGHT - (emberSprite.getTextureRect().height / 2));
	emberHPBar.init(false, sf::Vector2f(emberSprite.getPosition().x + 90, emberSprite.getPosition().y + (emberSprite.getTextureRect().height / 2) - (18 * 3) - 2), 
		GameData::instance().getEmber().getHP());
	emberManaBar.init(false, sf::Vector2f(emberSprite.getPosition().x + 90, emberSprite.getPosition().y + (emberSprite.getTextureRect().height / 2) - (18 * 2) + 2), 
		GameData::instance().getEmber().getMP(), sf::Color(105, 186, 199));

	thunderSprite.setTexture(TextureManager::instance().getTexture("assets/blue_art.png"));
	thunderSprite.setTextureRect(sf::IntRect(0, 0, thunderSprite.getLocalBounds().width, thunderSprite.getLocalBounds().height / 1.5));
	thunderSprite.scale(0.5, 0.5);
	thunderSprite.setPosition(332 * 2, HEIGHT - (thunderSprite.getTextureRect().height / 2));
	thunderHPBar.init(false, sf::Vector2f(thunderSprite.getPosition().x + 90, thunderSprite.getPosition().y + (thunderSprite.getTextureRect().height / 2) - (18 * 3) - 2), 
		GameData::instance().getThunder().getHP());
	thunderManaBar.init(false, sf::Vector2f(thunderSprite.getPosition().x + 90, thunderSprite.getPosition().y + (thunderSprite.getTextureRect().height / 2) - (18 * 2) + 2), 
		GameData::instance().getThunder().getMP(), sf::Color(105, 186, 199));

	fire.loadFromFile("assets/fire.frag", sf::Shader::Fragment);

	background.create(WIDTH, HEIGHT);

	//Load resources list file
	pugi::xml_parse_result result = doc.load_file((fileName).c_str());

	std::string resourcesList = doc.child("battle").child_value();

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

	sf::FloatRect bounds(HALF_WIDTH / 4, HALF_HEIGHT / 5, (HALF_WIDTH / 4) * 3, (HALF_HEIGHT / 5) * 3);

	for (auto i = list.begin(); i != list.end(); i++)
	{
		Monster &m = *i;

		sf::Sprite sprite;
		sprite.setTexture(TextureManager::instance().getTexture("assets/" + i->getName() + ".png"));
		sprite.setPosition(((HALF_WIDTH) - (sprite.getTextureRect().width * (list.size() + 1))) + (sprite.getTextureRect().width * (i - list.begin())), bounds.top + ((((i - list.begin()) % 2)) ? 10 : -10));
		monsters.push_back(sprite);

		Bar b;
		b.init(true, sf::Vector2f(sprite.getPosition().x * 2, sprite.getPosition().y), 
			m.getHP());
		std::cout << std::to_string(i->getHP()) << std::endl;
		enemyBars.push_back(b);
	}

	battleBox.setTexture(TextureManager::instance().getTexture("assets/battlebox.png"));

	selected = 0;
	counter = 0;
	fading = false;
	seconds = 0;

	state = AI;
}

void Battle::drawUI(sf::RenderTarget &tg)
{
	tg.draw(emberSprite);
	tg.draw(thunderSprite);
	tg.draw(playerSprite);
	tg.draw(battleBox);
	emberHPBar.draw(tg);
	thunderHPBar.draw(tg);
	playerHPBar.draw(tg);
	emberManaBar.draw(tg);
	thunderManaBar.draw(tg);
	playerManaBar.draw(tg);
	//std::cout << std::to_string(damageEffects.size()) << std::endl;

	for (auto i = damageEffects.begin(); i != damageEffects.end(); ++i)
	{
		i->draw(tg);
	}

	for (auto i = enemyBars.begin(); i != enemyBars.end(); ++i)
	{
		if (std::distance(enemyBars.begin(), i) == selected)
		{
			i->draw(tg);
		}		
	}
}

void Battle::draw(sf::RenderTarget &tg)
{
	sf::Sprite back(background);
	tg.draw(back, &fire);
	for (auto i = monsters.begin(); i != monsters.end(); ++i)
	{
		if (i - monsters.begin() == selected)
		{
			i->setColor(sf::Color::White);
			i->setColor(sf::Color(255, 255, 255, counter));
		}
		else
		{
			i->setColor(sf::Color(255u, 255u, 255u, 255u));
		}
		tg.draw(*i);
	}	
}

void Battle::update(sf::Time time)
{
	for (auto i = damageEffects.begin(); i != damageEffects.end();)
	{
		Damage &d = *i;
		if (d.isActive())
		{
			d.update(time);
			i++;
		}
		else
		{
			i = damageEffects.erase(i);
		}
	}

	for (auto i = enemyBars.begin(); i != enemyBars.end(); ++i)
	{
		auto t = std::distance(enemyBars.begin(), i);
		i->update(time, sf::Vector2f(monsters[t].getPosition().x * 2, monsters[t].getPosition().y));
	}

	fire.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
	fire.setParameter("seconds", seconds);

	seconds++;

	if (fading == false)
	{
		if (counter<255) counter+=15;
		else 
		{
			counter = 255;
			fading = true;
		}
	}
	if (fading == true)
	{
		if (counter>0) counter-=15;
		else 
		{
			counter = 0; 
			fading = false;
		}
	}

	if (monsters.size() != 0)
	{
		for (auto i = monsters.begin(); i != monsters.end(); ++i)
		{
			if (-20 + (rand() % (int)(21)) == -3)
			{
				i->move(-1 + (rand() % (int)(3)), -1 + (rand() % (int)(3)));
			}
		}
	}	

	emberHPBar.update(time);
	thunderHPBar.update(time);
	playerHPBar.update(time);
	emberManaBar.update(time);
	thunderManaBar.update(time);
	playerManaBar.update(time);

	oTweener.step(time.asSeconds());
}

void Battle::input(sf::Event &event)
{
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T)	
	{
		damageEffects.push_back(Damage(sf::Vector2f(100, playerSprite.getPosition().y), "-1", font));

		GameData::instance().getPlayer().setHP(GameData::instance().getPlayer().getHP() - 1);
		//GameData::instance().getPlayer().setMP(GameData::instance().getPlayer().getMP() - 1);
		//std::cout << std::to_string(GameData::instance().getPlayer().getHP()) << std::endl;
	}
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Y)	
	{
		damageEffects.push_back(Damage(sf::Vector2f(monsters.back().getPosition().x * 2, monsters.back().getPosition().y * 2), "-1", font));

		squad.getMonsters().back().setHP(squad.getMonsters().back().getHP() - 1);
		//std::cout << std::to_string(squad.getMonsters().back().getHP()) << std::endl;
	}
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
	}
}