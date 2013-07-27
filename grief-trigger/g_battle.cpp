#include "g_battle.h"

#include "h_config.h"
#include "d_parser.h"

#define SHAPE_WIDTH 180.0
#define BAR_WIDTH 170.0

#define SHAPE_HEIGHT 18.0
#define BAR_HEIGHT 8.0

#define OFFSET 5.0

#define ANIMATIONTIME 1.0

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
			if (text.getString().getSize() + 1 == actualString.size())
			{
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
}

void Logger::input(sf::Event &event)
{
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && ended) read = true;
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !ended) stop();
}

void Logger::stop()
{
	text.setString(actualString);
	ended = true;
	character = actualString.length();
}

void Logger::setString(std::string str)
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

Enemy::Enemy(sf::Vector2f pos, const sf::Texture &texture, Monster &monsterRef) : monster(monsterRef)
{
	sprite.setPosition(pos);
	sprite.setTexture(texture);
	hpBar.init(true, sf::Vector2f(sprite.getPosition().x * 2, sprite.getPosition().y), 
		monster.getHP());
	counter = 0;
	fading = false;
	animation = false;
}

void Enemy::draw(sf::RenderTarget &tg, bool selected)
{
	if (selected)
	{
		sprite.setColor(sf::Color(255, 255, 255, counter));
	}
	else if (animation)
	{
		sprite.setColor(sf::Color(counter, counter, counter, 255));
	}
	else
	{
		sprite.setColor(sf::Color(255u, 255u, 255u, 255u));
	}
	tg.draw(sprite);
}

void Enemy::drawUI(sf::RenderTarget &tg)
{
	hpBar.draw(tg);

	if (animation && clock.getElapsedTime().asSeconds() >= ANIMATIONTIME) 
	{
		animation = false;
		counter = 0;
		fading = false;
	}
}

void Enemy::update(sf::Time time)
{
	if (!animation)
	{
		if (-20 + (rand() % (int)(21)) == -3)
		{
			sprite.move(-1 + (rand() % (int)(3)), -1 + (rand() % (int)(3)));
		}

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
	}
	else
	{
		if (fading == false)
		{
			if (counter<255) counter+=51;
			else 
			{
				counter = 255;
				fading = true;
			}
		}
		if (fading == true)
		{
			if (counter>0) counter-=51;
			else 
			{
				counter = 0; 
				fading = false;
			}
		}
	}

	hpBar.update(time, sf::Vector2f(sprite.getPosition().x * 2, sprite.getPosition().y));
}

void Enemy::playAnimation()
{
	animation = true;
	clock.restart();
	counter = 0;
	fading = false;
}

Battle::Battle()
{
}

void Battle::init(std::string fileName)
{
	std::cout << std::to_string(GameData::instance().getPlayer().getHP()) << std::endl;

	font.loadFromFile(fontPath);

	log.init(sf::Vector2f(110, 21));

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

	std::vector<std::string> resourcesNames = Parser::instance().parseResources("battle");

	for (int i = 0; i < resourcesNames.size(); i++)
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
		Enemy newEnemy(sf::Vector2f(HALF_WIDTH - ((67 * 2) * (list.size())) + ((67 * 2) * (i - list.begin())), bounds.top + ((((i - list.begin()) % 2)) ? 10 : -10)), TextureManager::instance().getTexture("assets/" + i->getName() + ".png"), m);
		enemies.push_back(newEnemy);
	}

	battleBox.setTexture(TextureManager::instance().getTexture("assets/battlebox.png"));

	selected = 0;
	seconds = 0;
	currentAttacking = 0;
	turnNumber = 0;

	state = AI;

	clock.restart();
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

	for (auto i = damageEffects.begin(); i != damageEffects.end(); ++i)
	{
		i->draw(tg);
	}

	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		i->drawUI(tg);		
	}

	log.draw(tg);
}

void Battle::draw(sf::RenderTarget &tg)
{
	sf::Sprite back(background);
	tg.draw(back, &fire);

	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		if (i - enemies.begin() == selected)
		{
			i->draw(tg, true);
		}
		else
		{
			i->draw(tg);
		}
	}
}

void Battle::damagePlayer(Monster &monster)
{
	//GameData::instance().getEmber()
	int hero = (rand() % (int)(3));

	//Random formula
	int dmg = (monster.getStrength() / 2 + monster.getAgility() / 3 + monster.getIntelligence() / 4) + (rand() % (int)(2));
	std::string name;

	switch (hero)
	{
	case 0:
		damageEffects.push_back(Damage(sf::Vector2f(playerSprite.getPosition().x + (playerSprite.getTextureRect().width / 4), 
			playerSprite.getPosition().y + (playerSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg), font));
		GameData::instance().getPlayer().setHP(GameData::instance().getPlayer().getHP() - dmg);
		name = "Player";
		break;
	case 1:
		damageEffects.push_back(Damage(sf::Vector2f(emberSprite.getPosition().x + (emberSprite.getTextureRect().width / 4), 
			emberSprite.getPosition().y + (emberSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg), font));
		GameData::instance().getEmber().setHP(GameData::instance().getEmber().getHP() - dmg);
		name = "Ember";
		break;
	case 2:
		damageEffects.push_back(Damage(sf::Vector2f(thunderSprite.getPosition().x + (thunderSprite.getTextureRect().width / 4), 
			thunderSprite.getPosition().y + (thunderSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg), font));
		GameData::instance().getThunder().setHP(GameData::instance().getThunder().getHP() - dmg);
		name = "Thunder";
		break;
	default:
		break;
	}

	std::string tmp = enemies[currentAttacking].getMonster().getName();
	tmp[0] = toupper(tmp[0]);

	log.setString(tmp + " damaged " + name + " for " + std::to_string(dmg) + " points.");
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

	fire.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
	fire.setParameter("seconds", seconds);

	seconds++;

	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		i->update(time);
	}

	emberHPBar.update(time);
	thunderHPBar.update(time);
	playerHPBar.update(time);
	emberManaBar.update(time);
	thunderManaBar.update(time);
	playerManaBar.update(time);

	oTweener.step(time.asSeconds());

	log.update(time);

	if (state == AI && log.isRead())
	{
		enemies[currentAttacking].stopAnimation();
		selected = currentAttacking;
		nextStep();
	}
}

void Battle::nextStep()
{
	if (state == AI && log.isEnded())
	{
		if (currentAttacking + 1 > enemies.size())
		{
			turnNumber++;
			currentAttacking = 0;
			for (auto i = enemies.begin(); i != enemies.end(); i++)
			{
				i->setState(NOT_ATTACKED);
			}		
			if (state == AI) state = PLAYER;
		}
		else
		{
			damagePlayer(enemies[currentAttacking].getMonster());

			enemies[currentAttacking].playAnimation();
			enemies[currentAttacking].setState(ATTACKED);

			currentAttacking++;	
		}		
	}	
}

void Battle::input(sf::Event &event)
{
	//For some fucking reason only works if update log's input firstly
	log.input(event);

	//if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && log.isEnded()) nextStep();
	

	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T)	
	{
		damageEffects.push_back(Damage(sf::Vector2f(100, playerSprite.getPosition().y), "-1", font));

		GameData::instance().getPlayer().setHP(GameData::instance().getPlayer().getHP() - 1);
		//GameData::instance().getPlayer().setMP(GameData::instance().getPlayer().getMP() - 1);
		//std::cout << std::to_string(GameData::instance().getPlayer().getHP()) << std::endl;
	}
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Y)	
	{
		damageEffects.push_back(Damage(sf::Vector2f(enemies[selected].getPosition().x * 2, enemies[selected].getPosition().y * 2), "-1", font));

		enemies[selected].getMonster().setHP(enemies[selected].getMonster().getHP() - 1);
		//squad.getMonsters()[selected].setHP(squad.getMonsters()[selected].getHP() - 1);
		//std::cout << std::to_string(squad.getMonsters().back().getHP()) << std::endl;
	}
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U)	
	{
		//damageEffects.push_back(Damage(sf::Vector2f(enemies[selected].getPosition().x * 2, enemies[selected].getPosition().y * 2), "-1", font));
		//setHP(enemies[selected].getMonster().getHP() - 1);

		damagePlayer(enemies[selected].getMonster());

		//squad.getMonsters()[selected].setHP(squad.getMonsters()[selected].getHP() - 1);
		//std::cout << std::to_string(squad.getMonsters().back().getHP()) << std::endl;
	}

	if (state != AI)
	{
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
		{
			if (selected > 0)
			{
				selected--;
			}
			else
			{
				selected = enemies.size() - 1;
			}
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
		{
			if (selected + 1 < enemies.size())
			{
				selected++;
			}
			else
			{
				selected = 0;
			}
		}
	}
}