#include "g_battle.h"

#include "h_config.h"
#include "d_parser.h"
#include "g_scenemanager.h"

//Every time when i use "* 2" means that i'm fag

Enemy::Enemy(sf::Vector2f pos, const sf::Texture &texture, Monster &monsterRef) : monster(monsterRef)
{
	sprite.setPosition(pos);
	sprite.setTexture(texture);
	hpBar.init(true, sf::Vector2f(sprite.getPosition().x * 2, sprite.getPosition().y), 
		monster.getHP());
	counter = 0;
	fading = false;
	animation = false;
	bounds = sf::FloatRect(sprite.getPosition().x - 10, sprite.getPosition().y - 10, texture.getSize().x + 10, texture.getSize().y + 10);
}

void Enemy::draw(sf::RenderTarget &tg, bool selected)
{
	if (animation)
	{
		sprite.setColor(sf::Color(counter, counter, counter, 255));
	}
	else if (selected)
	{
		sprite.setColor(sf::Color(255, 255, 255, counter));
	}
	else
	{
		sprite.setColor(sf::Color(255u, 255u, 255u, 255u));
	}
	if (monster.isDied() && !animation)
	{
		sprite.setColor(sf::Color(100, 100, 100, 255u));
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
			float ox = -1 + (rand() % (int)(3));
			float oy =  -1 + (rand() % (int)(3));

			if (bounds.intersects(sf::FloatRect(sprite.getPosition().x  + ox, sprite.getPosition().y + oy, sprite.getLocalBounds().width, sprite.getLocalBounds().height), sf::FloatRect(0, 0, sprite.getLocalBounds().width, sprite.getLocalBounds().height)))
			{
				sprite.move(ox, oy);
			}	
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

	TextureManager::instance().getTexture("assets/hit.png");

	fire.loadFromFile("assets/fire.frag", sf::Shader::Fragment);

	battleBox.setTexture(TextureManager::instance().getTexture("assets/battlebox.png"));

	effectRect = sf::RectangleShape(sf::Vector2f(WIDTH, HEIGHT));
	effectRect.setFillColor(sf::Color(0,0,0,205));

	log.init(sf::Vector2f(110, 21));

	pointer.setTexture(TextureManager::instance().getTexture("assets/pointer.png"));

	background.create(WIDTH, HEIGHT);

	std::vector<std::string> resourcesNames = Parser::instance().parseResources("battle");
	for (int i = 0; i < resourcesNames.size(); i++)
	{
		TextureManager::instance().getTexture("assets/" + resourcesNames[i]);
	}

	std::vector<std::wstring> str;
	str.push_back(L"Атака");
	str.push_back(L"Способность");
	str.push_back(L"Предметы");

	menu.init(str);
}

void Battle::start(Squad &squad_)
{
	squad = squad_;

	std::vector<Monster> &list = squad.getMonsters();

	sf::FloatRect bounds(HALF_WIDTH / 4, HALF_HEIGHT / 5, (HALF_WIDTH / 4) * 3, (HALF_HEIGHT / 5) * 3);
	enemies.clear();
	for (auto i = list.begin(); i != list.end(); i++)
	{
		Monster &m = *i;
		Enemy newEnemy(sf::Vector2f(HALF_WIDTH - ((67 * 2) * (list.size())) + ((67 * 2) * (i - list.begin())), bounds.top + ((((i - list.begin()) % 2)) ? 10 : -10)), TextureManager::instance().getTexture("assets/" + i->getName() + ".png"), m);
		enemies.push_back(newEnemy);
	}

	selected = 0;
	seconds = 0;
	currentAttacking = 0;
	turnNumber = 0;

	state = AI;

	clock.restart();

	res = BattleResult();
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

	if (state == SPELL)
	{
		tg.draw(effectRect);
	}
	if (state == SPELL) 
	{		
		spellMenu.draw(tg);
	}

	if (state == PLAYER) tg.draw(pointer);

	if (state == QTE) spellQTE.draw(tg);

	//Hit effects
	he.draw(tg);
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
		damageEffects.push_back(Damage(sf::Vector2f(playerSprite.getPosition().x + (playerSprite.getTextureRect().width / 3), 
			playerSprite.getPosition().y + (playerSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg)));
		GameData::instance().getPlayer().setHP(GameData::instance().getPlayer().getHP() - dmg);
		name = L"Игрок";
		break;
	case 1:
		damageEffects.push_back(Damage(sf::Vector2f(emberSprite.getPosition().x + (emberSprite.getTextureRect().width / 3), 
			emberSprite.getPosition().y + (emberSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg)));
		GameData::instance().getEmber().setHP(GameData::instance().getEmber().getHP() - dmg);
		name = L"Эмбер";
		break;
	case 2:
		damageEffects.push_back(Damage(sf::Vector2f(thunderSprite.getPosition().x + (thunderSprite.getTextureRect().width / 3), 
			thunderSprite.getPosition().y + (thunderSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg)));
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

	SceneManager::instance().getScene().setCurrentEffect("distortion", sf::seconds(0.1));
}

void Battle::loadResources()
{
	spellQTE.loadResources();
}

void Battle::update(sf::Time time)
{
	if (state != SPELL)
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

		//Log on top of the screen
		log.update(time);

		//Is message completely read
		if (state == AI && log.isRead())
		{
			//Let them turn
			nextAIStep();
		}

		//Change pointer position
		if (state == PLAYER)
		{
			switch (currentAttacking)
			{
			case 0:
				pointer.setPosition(playerSprite.getPosition().x + 150, playerSprite.getPosition().y + 100);
				break;
			case 1:
				pointer.setPosition(emberSprite.getPosition().x + 190, emberSprite.getPosition().y + 100);
				break;
			case 2:
				pointer.setPosition(thunderSprite.getPosition().x + 170, thunderSprite.getPosition().y + 100);
				break;
			default:
				break;
			}		
		}

		//Menu, opens by ENTER
		menu.update(time);
	}

	//Using menu, 0 is attack, 1 is spell, 2 is item
	if (state == PLAYER && menu.getSelected() != NOT_SELECTED) 
	{
		if (menu.getSelected() == 0)
		{			
			nextPlayerStep();
			menu.clean();
			menu.disappear();
		}

		if (menu.getSelected() == 1)
		{			
			state = SPELL;
			menu.clean();
			menu.disappear();

			switch (currentAttacking)
			{
			case 0:
				spellMenu.setSpells(Parser::instance().parseSpells("player", GameData::instance().getPlayer().getLevel()));
				break;
			case 1:
				spellMenu.setSpells(Parser::instance().parseSpells("ember", GameData::instance().getEmber().getLevel()));
				break;
			case 2:
				spellMenu.setSpells(Parser::instance().parseSpells("thunder", GameData::instance().getThunder().getLevel()));
				break;
			default:
				break;
			}	
		}

		if (menu.getSelected() == 2)
		{			
		}
	}

	//Very bad handling of game's end
	bool allDead = true;
	for (auto i = enemies.begin(); i != enemies.end(); i++)
	{
		if (!i->isDied())
		{
			allDead = false;
			break;
		}
	}
	if(allDead && state != ENDED)
	{
		state = ENDED;
		log.setString(L"Бой окончен!");
		SoundManager::instance().playWinSound();
	}

	//Spells
	if (state == SPELL)
	{
		spellMenu.update(time);
		if (spellMenu.getSelected() != -1)
		{
			state = QTE;
			menu.clean();
			menu.disappear();

			switch (currentAttacking)
			{
			case 0:
				spellQTE.start(spellMenu.getSelectedSpell(), "player");
				break;
			case 1:
				spellQTE.start(spellMenu.getSelectedSpell(), "red");
				break;
			case 2:
				spellQTE.start(spellMenu.getSelectedSpell(), "blue");
				break;
			default:
				break;
			} 
		}
	}

	//QTE
	if(state == QTE) 
	{
		if (spellQTE.isDamaged())
		{
			damageSpell();
			spellQTE.clean();
		}

		if (spellQTE.isEnded())
		{
			state = PLAYER;
			if (currentAttacking == 2)
			{
				currentAttacking = 0;
				state = AI;
			}
			else
			{
				currentAttacking++;
				for (auto i = enemies.begin(); i != enemies.end(); i++)
				{
					if (!i->isDied())
					{
						selected = i - enemies.begin();
						break;
					}
				}
			}
		}
		spellQTE.update(time);
	}

	//Hit effects
	he.update(time);

	//Update shader
	fire.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
	fire.setParameter("seconds", seconds);
	if (state == QTE)
	{
		seconds+=5;
		fire.setParameter("type", 1);
	}
	else
	{
		seconds++;
		fire.setParameter("type", 0);
	}
}

void Battle::damageSpell()
{
	unsigned int dmg;
	switch (currentAttacking)
	{
	case 0:
		dmg = (GameData::instance().getPlayer().getIntelligence() / 4) + (rand() % (int)(2));
		break;
	case 1:
		dmg = (GameData::instance().getEmber().getIntelligence() / 4) + (rand() % (int)(2));
		break;
	case 2:
		dmg = (GameData::instance().getThunder().getIntelligence() / 4) + (rand() % (int)(2));
		break;
	default:
		break;
	} 

	enemies[selected].playAnimation();
	enemies[selected].getMonster().receiveDamage(dmg);

	if (enemies[selected].isDied())
	{
		res.playerXP += enemies[selected].getMonster().getHP();
		res.emberXP += enemies[selected].getMonster().getHP();
		res.thunderXP += enemies[selected].getMonster().getHP();
		//Select next alive enemy
		for (auto i = enemies.begin(); i != enemies.end(); i++)
		{
			if (!i->isDied())
			{
				selected = i - enemies.begin();
			}
		}
	}

	SoundManager::instance().playHurtSound();
}

void Battle::damageMonster()
{
	int dmg;
	//Count damage
	switch (currentAttacking)
	{
	case 0:
		dmg = (GameData::instance().getPlayer().getStrength() / 4 + GameData::instance().getPlayer().getAgility() / 5 + GameData::instance().getPlayer().getIntelligence() / 6) + (rand() % (int)(2));
		break;
	case 1:
		dmg = (GameData::instance().getEmber().getStrength() / 4 + GameData::instance().getEmber().getAgility() / 5 + GameData::instance().getEmber().getIntelligence() / 6) + (rand() % (int)(2));
		break;
	case 2:
		dmg = (GameData::instance().getThunder().getStrength() / 4 + GameData::instance().getThunder().getAgility() / 5 + GameData::instance().getThunder().getIntelligence() / 6) + (rand() % (int)(2));
		break;
	default:
		break;
	} 

	he.init(sf::Vector2f((enemies[selected].getPosition().x * 2) + (enemies[selected].getTexture()->getSize().x / 2) -  (784 / 2) + 40, enemies[selected].getPosition().y + (enemies[selected].getTexture()->getSize().y / 2)));

	enemies[selected].playAnimation();
	enemies[selected].getMonster().receiveDamage(dmg);

	if (enemies[selected].isDied())
	{
		res.playerXP += enemies[selected].getMonster().getHP();
		res.emberXP += enemies[selected].getMonster().getHP();
		res.thunderXP += enemies[selected].getMonster().getHP();
		//Select next alive enemy
		for (auto i = enemies.begin(); i != enemies.end(); i++)
		{
			if (!i->isDied())
			{
				selected = i - enemies.begin();
			}
		}
	}

	SoundManager::instance().playHurtSound();
}

void Battle::nextPlayerStep()
{
	if (state == PLAYER)
	{
		SoundManager::instance().playHurtSound();
		damageMonster();

		if (currentAttacking == 2)
		{
			currentAttacking = 0;
			state = AI;
		}
		else
		{
			currentAttacking++;
		}
	}
}

void Battle::clean()
{
	//Should i do it in start()?
	squad = Squad();
	enemies.clear();
	selected = 0;
	seconds = 0;
	clock.restart();
}

void Battle::nextAIStep()
{
	//Some shit here
	if (state == AI && log.isEnded())
	{
		while (currentAttacking < enemies.size() && enemies[currentAttacking].isDied())
		{
			currentAttacking++;
		}

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
			for (auto i = enemies.begin(); i != enemies.end(); i++)
			{
				if (!i->isDied())
				{
					selected = i - enemies.begin();
					break;
				}
			}		
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
	if (menu.isWorking() && state == PLAYER)
	{
		menu.input(event);
	}

	//Input qtes
	if (state == QTE)
	{
		spellQTE.input(event);
	}

	//End battle
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X && state == ENDED && log.isRead())
	{
		//GameData::instance().appendResult(res);
		GameData::instance().getPlayer().addXP(res.playerXP);
		GameData::instance().getPlayer().addXP(res.emberXP);
		GameData::instance().getPlayer().addXP(res.thunderXP);
		SceneManager::instance().endBattle();
	}

	//Open menu when player's turn
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X && state == PLAYER && !menu.isWorking() && log.isRead() && !enemies[selected].isDied())
	{
		menu.appear(sf::Vector2f(enemies[selected].getPosition().x * 2, enemies[selected].getPosition().y));
	}

	//Close menu on ESCAPE
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && state == PLAYER && menu.isWorking())
	{
		menu.disappear();
		SoundManager::instance().playSelectSound();
	}

	//Close spells menu
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && state == SPELL && spellMenu.isWorking())
	{
		state = PLAYER;
		spellMenu.close();
		SoundManager::instance().playEnterSound();
	}

	if (state == SPELL)
	{
		spellMenu.input(event);
	}

	//Stop animation if log is read
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X && !log.isRead() && state == AI)
	{
		enemies[currentAttacking - 1].stopAnimation();
	}

	//Update selected
	if (state == PLAYER && !menu.isWorking())
	{
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && !menu.isWorking())
		{
			do 
			{
				if (selected > 0)
				{
					selected--;
				}
				else
				{
					selected = enemies.size() - 1;
				}
			} while (enemies[selected].isDied());

			std::cout << std::to_string(selected) << std::endl;

			SoundManager::instance().playSelectSound();
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && !menu.isWorking())
		{
			do 
			{
				if (selected + 1 < enemies.size())
				{
					selected++;
				}
				else
				{
					selected = 0;
				}
			} while (enemies[selected].isDied());	

			std::cout << std::to_string(selected) << std::endl;

			SoundManager::instance().playSelectSound();
		}
	}
}