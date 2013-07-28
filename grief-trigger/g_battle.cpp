#include "g_battle.h"

#include "h_config.h"
#include "d_parser.h"

Enemy::Enemy(sf::Vector2f pos, const sf::Texture &texture, Monster &monsterRef) : monster(monsterRef)
{
	sprite.setPosition(pos);
	sprite.setTexture(texture);
	hpBar.init(true, sf::Vector2f(sprite.getPosition().x * 2, sprite.getPosition().y), 
		monster.getHP());
	counter = 0;
	fading = false;
	animation = false;
	bounds = sf::FloatRect(sprite.getPosition().x, sprite.getPosition().y, texture.getSize().x, texture.getSize().y);
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
			//if (sprite.getGlobalBounds().intersects(bounds))
			//{
			sprite.move(-1 + (rand() % (int)(3)), -1 + (rand() % (int)(3)));
			//}	
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

	std::vector<std::wstring> str;
	str.push_back(L"Атака");
	str.push_back(L"Способность");
	str.push_back(L"Предмет");

	menu.init(str);
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

	menu.draw(tg);
}

void Battle::draw(sf::RenderTarget &tg)
{
	sf::Sprite back(background);
	tg.draw(back, &fire);

	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		if (i - enemies.begin() == selected && state == PLAYER)
		{
			i->draw(tg, true);
		}
		else
		{
			i->draw(tg);
		}
	}
}

int stringToWString(std::wstring &ws, const std::string &s)
{
	std::wstring wsTmp(s.begin(), s.end());

	ws = wsTmp;

	return 0;
}

void Battle::damagePlayer(Monster &monster)
{
	//GameData::instance().getEmber()
	int hero = (rand() % (int)(3));

	//Random formula
	int dmg = (monster.getStrength() / 2 + monster.getAgility() / 3 + monster.getIntelligence() / 4) + (rand() % (int)(2));
	std::wstring name;

	switch (hero)
	{
	case 0:
		damageEffects.push_back(Damage(sf::Vector2f(playerSprite.getPosition().x + (playerSprite.getTextureRect().width / 4), 
			playerSprite.getPosition().y + (playerSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg), font));
		GameData::instance().getPlayer().setHP(GameData::instance().getPlayer().getHP() - dmg);
		name = L"Игрок";
		break;
	case 1:
		damageEffects.push_back(Damage(sf::Vector2f(emberSprite.getPosition().x + (emberSprite.getTextureRect().width / 4), 
			emberSprite.getPosition().y + (emberSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg), font));
		GameData::instance().getEmber().setHP(GameData::instance().getEmber().getHP() - dmg);
		name = L"Эмбер";
		break;
	case 2:
		damageEffects.push_back(Damage(sf::Vector2f(thunderSprite.getPosition().x + (thunderSprite.getTextureRect().width / 4), 
			thunderSprite.getPosition().y + (thunderSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg), font));
		GameData::instance().getThunder().setHP(GameData::instance().getThunder().getHP() - dmg);
		name = L"Сандер";
		break;
	default:
		break;
	}

	std::wstring tmpw;
	std::string tmp = enemies[currentAttacking].getMonster().getName();
	tmp[0] = toupper(tmp[0]);

	stringToWString(tmpw, tmp);

	log.setString(tmpw + L" нанес " + std::to_wstring(dmg) + L" урона персонажу " + name + L".");
}

void Battle::update(sf::Time time)
{
	//Update damage effect
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

	//Update shader
	fire.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
	fire.setParameter("seconds", seconds);
	seconds++;

	//Update enemies
	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		i->update(time);
	}

	//Bla-bla-bla
	emberHPBar.update(time);
	thunderHPBar.update(time);
	playerHPBar.update(time);
	emberManaBar.update(time);
	thunderManaBar.update(time);
	playerManaBar.update(time);

	oTweener.step(time.asSeconds());

	log.update(time);

	//Is message completely read
	if (state == AI && log.isRead())
	{
		//Let them turn
		nextAIStep();
	}

	menu.update(time);
}

void Battle::nextAIStep()
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
				i->stopAnimation();
			}		
			state = PLAYER;
			log.setString(L"Атакуйте!");
			selected = 0;
		}
		else
		{
			damagePlayer(enemies[currentAttacking].getMonster());

			enemies[currentAttacking].playAnimation();
			enemies[currentAttacking].setState(ATTACKED);

			selected = currentAttacking;
			currentAttacking++;	
		}		
	}	
}

void Battle::input(sf::Event &event)
{
	//For some fucking reason only works if update log's input firstly
	log.input(event);
	if (menu.isWorking())
	{
		menu.input(event);
	}

	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && state == PLAYER && !menu.isWorking() && log.isRead())
	{
		

		menu.appear(sf::Vector2f(enemies[selected].getPosition().x * 2, enemies[selected].getPosition().y));
	}

	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && state == PLAYER && menu.isWorking())
	{
		menu.disappear();
		SoundManager::instance().playSelectSound();
	}

	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !log.isRead() && state == AI)
	{
		enemies[currentAttacking - 1].stopAnimation();
	}

	//std::cout << std::to_string(squad.getMonsters().back().getHP()) << std::endl;

	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U)	
	{
		damagePlayer(enemies[selected].getMonster());
	}

	if (state != AI && !menu.isWorking())
	{
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A && !menu.isWorking())
		{
			if (selected > 0)
			{
				selected--;
			}
			else
			{
				selected = enemies.size() - 1;
			}

			SoundManager::instance().playSelectSound();
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D && !menu.isWorking())
		{
			if (selected + 1 < enemies.size())
			{
				selected++;
			}
			else
			{
				selected = 0;
			}

			SoundManager::instance().playSelectSound();
		}
	}
}