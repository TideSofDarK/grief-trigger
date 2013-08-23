#include "g_battle.h"

#include "h_config.h"
#include "d_parser.h"
#include "g_scenemanager.h"
#include "os_x360controller.hpp"

//Every time when i use "* 2" means that i'm fag

Enemy::Enemy(sf::Vector2f pos, const sf::Texture &texture, Monster &monsterRef) : monster(monsterRef)
{
	sprite.setPosition(pos);
	sprite.setTexture(texture);
	hpBar.init(true, sf::Vector2f(sprite.getPosition().x * 2, sprite.getPosition().y / 2), 
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

	DialoguePanel::instance().draw(tg);
}

void Enemy::update(sf::Time time)
{
	DialoguePanel::instance().update();

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
	space.loadFromFile("assets/space.frag", sf::Shader::Fragment);
	sback.loadFromFile("assets/sback.frag", sf::Shader::Fragment);
	fire.loadFromFile("assets/fire.frag", sf::Shader::Fragment);
	light.loadFromFile("assets/light.frag", sf::Shader::Fragment);
	creo.loadFromFile("assets/creo.frag", sf::Shader::Fragment);
	shards.loadFromFile("assets/shards.frag", sf::Shader::Fragment);
	hell.loadFromFile("assets/hell.frag", sf::Shader::Fragment);
	mech.loadFromFile("assets/mech.frag", sf::Shader::Fragment);

	effectRect = sf::RectangleShape(sf::Vector2f(WIDTH, HEIGHT));
	effectRect.setFillColor(sf::Color(0,0,0,205));

	log.init(sf::Vector2f(110, 21));

	std::vector<std::wstring> str;
	str.push_back(L"Атака");
	str.push_back(L"Способность");
	str.push_back(L"Предметы");

	menu.init(str);
}

void Battle::start(std::vector<Monster> &ms, bool p)
{
	sf::FloatRect bounds(HALF_WIDTH / 4, HALF_HEIGHT / 5, (HALF_WIDTH / 4) * 3, (HALF_HEIGHT / 5) * 3);
	enemies.clear();

	if (ms.size() > 1)
	{
		for (auto i = ms.begin(); i != ms.end(); i++)
		{
			Monster &m = *i;
			Enemy newEnemy(sf::Vector2f(HALF_WIDTH - ((67 * 2) * (ms.size())) + ((67 * 2) * (i - ms.begin())), 
				bounds.top + ((((i - ms.begin()) % 2)) ? 10 : -10)), TextureManager::instance().getTexture("assets/" + i->getName() + ".png"), m);
			enemies.push_back(newEnemy);
		}
	}
	else
	{
		Enemy newEnemy(sf::Vector2f(HALF_WIDTH/2 - 10, 
			bounds.top + 10), TextureManager::instance().getTexture("assets/" + ms.back().getName() + ".png"), ms.back());
		enemies.push_back(newEnemy);
	}


	selected = 0;
	seconds = 0;
	currentAttacking = 0;
	turnNumber = 0;

	if (p)
	{
		state = PLAYER;
		log.setString(L"Вы атакуете первыми!");
		advantage.setTexture(TextureManager::instance().getTexture("assets/playersadvantage.png"));

		currentAttacking = -1;
		bool stop;
		do
		{
			currentAttacking++;
			if (currentAttacking == 0 && GameData::instance().getPlayer().getHP() == 0 
				|| currentAttacking == 1 && GameData::instance().getEmber().getHP() == 0 
				|| currentAttacking == 2 && GameData::instance().getThunder().getHP() == 0)
			{
				stop = false;
			}
			else
			{
				stop = true;
				break;
			}
		}
		while (!stop);
	}
	else
	{
		state = AI;
		log.setString(L"Враг атакует первым!");
		advantage.setTexture(TextureManager::instance().getTexture("assets/enemyadvantage.png"));
	}

	clock.restart();

	res = BattleResult();

	advantage.setPosition(-189,0);
	advantage.setColor(sf::Color(255,255,255,0));
	nax = 0.0;
	aEffect = true;
	check = false;

	oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_INOUT, 2.f, &nax, -189);
}

void Battle::drawUI(sf::RenderTarget &tg)
{
	effects.drawSecondary(tg);

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

	if (state == PLAYER || state == HEROSELECT) tg.draw(pointer);

	if (state == QTE) spellQTE.draw(tg);

	//Hit effects
	he.draw(tg);

	effects.draw(tg);

	if (aEffect) tg.draw(advantage);

	if (state == ITEM_MENU)
	{
		itemMenu.draw(tg);
	}
}

void Battle::draw(sf::RenderTarget &tg)
{
	if (currentBackground == spaceType)
	{
		sf::Sprite back(background3);
		tg.draw(back, &space);
	}
	else if (currentBackground == fireType)
	{
		sf::Sprite back(background3);
		tg.draw(back, &fire);
	}
	else if (currentBackground == sbackType)
	{
		sf::Sprite back(background3);
		tg.draw(back, &sback);
	}
	else if (currentBackground == lightType)
	{
		sf::Sprite back(background3);
		tg.draw(back, &light);
	}
	else if (currentBackground == creoType)
	{
		sf::Sprite back(background3);
		tg.draw(back, &creo);
	}
	else if (currentBackground == shardsType)
	{
		sf::Sprite back(background3);
		tg.draw(back, &shards);
	}
	else if (currentBackground == hellType)
	{
		sf::Sprite back(background3);
		tg.draw(back, &hell);
	}
	else if (currentBackground == mechType)
	{
		sf::Sprite back(background3);
		tg.draw(back, &mech);
	}

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
	int hero;
	bool stop;
	do
	{
		hero = (rand() % (int)(3));
		if (hero == 0 && GameData::instance().getPlayer().getHP() == 0 
			|| hero == 1 && GameData::instance().getEmber().getHP() == 0 
			|| hero == 2 && GameData::instance().getThunder().getHP() == 0)
		{
			stop = false;
		}
		else
		{
			stop = true;
			break;
		}
	}
	while (!stop);

	//Random formula
	int dmg = (monster.getStrength() / 2 + monster.getAgility() / 3 + monster.getIntelligence() / 4) + (rand() % (int)(2));
	std::wstring name;

	switch (hero)
	{
	case 0:
		damageEffects.push_back(Damage(sf::Vector2f(playerSprite.getPosition().x + (playerSprite.getTextureRect().width / 3), 
			playerSprite.getPosition().y + (playerSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg)));
		GameData::instance().getPlayer().setHP(GameData::instance().getPlayer().getHP() - dmg);
		name = L"Джейд";

		he.init(sf::Vector2f((playerSprite.getPosition().x * 2) + (playerSprite.getTexture()->getSize().x / 2) -  (784 / 2) + 40, playerSprite.getPosition().y + (playerSprite.getTexture()->getSize().y / 2)));
		break;
	case 1:
		damageEffects.push_back(Damage(sf::Vector2f(emberSprite.getPosition().x + (emberSprite.getTextureRect().width / 3), 
			emberSprite.getPosition().y + (emberSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg)));
		GameData::instance().getEmber().setHP(GameData::instance().getEmber().getHP() - dmg);
		name = L"Эмбер";

		he.init(sf::Vector2f((emberSprite.getPosition().x * 2) + (emberSprite.getTexture()->getSize().x / 2) -  (784 / 2) + 40, emberSprite.getPosition().y + (emberSprite.getTexture()->getSize().y / 2)));
		break;
	case 2:
		damageEffects.push_back(Damage(sf::Vector2f(thunderSprite.getPosition().x + (thunderSprite.getTextureRect().width / 3), 
			thunderSprite.getPosition().y + (thunderSprite.getTextureRect().height / 4)), "-" + std::to_string(dmg)));
		GameData::instance().getThunder().setHP(GameData::instance().getThunder().getHP() - dmg);
		name = L"Сандер";

		he.init(sf::Vector2f((thunderSprite.getPosition().x * 2) + (thunderSprite.getTexture()->getSize().x / 2) -  (784 / 2) + 40, thunderSprite.getPosition().y + (thunderSprite.getTexture()->getSize().y / 2)));
		break;
	default:
		break;
	}

	std::wstring tmpw;
	sf::String tmp = enemies[currentAttacking].getMonster().getName();
	if (tmp == "boss1")
	{
		tmp = L"Темная Мами";
	}
	if (tmp == "boss2")
	{
		tmp = L"Мами-монстр";
	}
	if (tmp == "doom")
	{
		tmp = L"Дум";
	}
	if (tmp == "flower")
	{
		tmp = L"Скрытень";
	}
	if (tmp == "skull")
	{
		tmp = L"Череп";
	}
	if (tmp == "slug")
	{
		tmp = L"Ужас";
	}

	//stringToWString(tmpw, tmp);

	log.setString(tmp.toWideString() + L" нанес " + std::to_wstring(dmg) + L" урона персонажу " + name + L".");

	SceneManager::instance().getScene().setCurrentEffect("distortion", sf::seconds(0.1));
}

void Battle::loadResources()
{
	std::vector<std::string> resourcesNames = Parser::instance().parseResources("battle");
	for (int i = 0; i < resourcesNames.size(); i++)
	{
		TextureManager::instance().getTexture("assets/" + resourcesNames[i]);
	}

	playerSprite.setTexture(TextureManager::instance().getTexture("assets/girl_art.png"));
	emberSprite.setTexture(TextureManager::instance().getTexture("assets/red_art.png"));
	thunderSprite.setTexture(TextureManager::instance().getTexture("assets/blue_art.png"));
	TextureManager::instance().getTexture("assets/hit.png");
	battleBox.setTexture(TextureManager::instance().getTexture("assets/battlebox.png"));
	pointer.setTexture(TextureManager::instance().getTexture("assets/pointer.png"));
	playerSprite.setTextureRect(sf::IntRect(0, 0, playerSprite.getLocalBounds().width, playerSprite.getLocalBounds().height / 1.5));
	playerSprite.scale(0.5, 0.5);
	playerSprite.setPosition(0, HEIGHT - (playerSprite.getTextureRect().height / 2));
	playerHPBar.init(false, sf::Vector2f(playerSprite.getPosition().x + 90, playerSprite.getPosition().y + (playerSprite.getTextureRect().height / 2) - (18 * 3) - 2), 
		GameData::instance().getPlayer().getHP());
	playerManaBar.init(false, sf::Vector2f(playerSprite.getPosition().x + 90, playerSprite.getPosition().y + (playerSprite.getTextureRect().height / 2) - (18 * 2) + 2), 
		GameData::instance().getPlayer().getMP(), sf::Color(105, 186, 199));


	emberSprite.setTextureRect(sf::IntRect(0, 0, emberSprite.getLocalBounds().width, emberSprite.getLocalBounds().height / 1.5));
	emberSprite.scale(0.5, 0.5);
	emberSprite.setPosition(332, HEIGHT - (emberSprite.getTextureRect().height / 2));
	emberHPBar.init(false, sf::Vector2f(emberSprite.getPosition().x + 90, emberSprite.getPosition().y + (emberSprite.getTextureRect().height / 2) - (18 * 3) - 2), 
		GameData::instance().getEmber().getHP());
	emberManaBar.init(false, sf::Vector2f(emberSprite.getPosition().x + 90, emberSprite.getPosition().y + (emberSprite.getTextureRect().height / 2) - (18 * 2) + 2), 
		GameData::instance().getEmber().getMP(), sf::Color(105, 186, 199));


	thunderSprite.setTextureRect(sf::IntRect(0, 0, thunderSprite.getLocalBounds().width, thunderSprite.getLocalBounds().height / 1.5));
	thunderSprite.scale(0.5, 0.5);
	thunderSprite.setPosition(332 * 2, HEIGHT - (thunderSprite.getTextureRect().height / 2));
	thunderHPBar.init(false, sf::Vector2f(thunderSprite.getPosition().x + 90, thunderSprite.getPosition().y + (thunderSprite.getTextureRect().height / 2) - (18 * 3) - 2), 
		GameData::instance().getThunder().getHP());
	thunderManaBar.init(false, sf::Vector2f(thunderSprite.getPosition().x + 90, thunderSprite.getPosition().y + (thunderSprite.getTextureRect().height / 2) - (18 * 2) + 2), 
		GameData::instance().getThunder().getMP(), sf::Color(105, 186, 199));	


	background.loadFromFile("assets/space.jpg");
	background2.loadFromFile("assets/noise.jpg");
	background3.create(WIDTH, HEIGHT);

	currentBackground = fireType;

	spellQTE.loadResources();

	TextureManager::instance().getTexture("assets/playersadvantage.png");
	advantage.setTexture(TextureManager::instance().getTexture("assets/enemyadvantage.png"));
}

void Battle::update(sf::Time time)
{
	if (GameData::instance().getPlayer().getHP() > 0 || GameData::instance().getEmber().getHP() > 0 || GameData::instance().getThunder().getHP() > 0)
	{
		effects.update(time);

		if (state == ITEM_MENU)
		{
			itemMenu.update(time);
		}

		if (check)
		{
			if (t.getElapsedTime().asSeconds() > 1.5)
			{
				aEffect = false;
				check = false;
			}	
		}
		else if (advantage.getColor().a >= 240)
		{
			check = true;
			t.restart();
		}
		advantage.setPosition(interpolateLinear(advantage.getPosition().x, 0, 0.04f),0);

		if (aEffect)
		{
			if ((int)(rand() % 50) == 3) SceneManager::instance().getScene().setCurrentEffect("rgb", sf::seconds(0.3));	
			advantage.setColor(sf::Color(255,255,255,interpolateLinear(advantage.getColor().a, 255, 0.07f)));
		}

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
			emberHPBar.update(time, sf::Vector2f(), GameData::instance().getEmber().getHP(), GameData::instance().getEmber().getMaxHP());
			thunderHPBar.update(time, sf::Vector2f(), GameData::instance().getThunder().getHP(), GameData::instance().getThunder().getMaxHP());
			playerHPBar.update(time, sf::Vector2f(), GameData::instance().getPlayer().getHP(), GameData::instance().getPlayer().getMaxHP());
			emberManaBar.update(time, sf::Vector2f(), GameData::instance().getEmber().getMP(), GameData::instance().getEmber().getMaxMP());
			thunderManaBar.update(time, sf::Vector2f(), GameData::instance().getThunder().getMP(), GameData::instance().getThunder().getMaxMP());
			playerManaBar.update(time, sf::Vector2f(), GameData::instance().getPlayer().getMP(), GameData::instance().getPlayer().getMaxMP());

			//Log on top of the screen
			log.update(time);

			//Is message completely read
			if (state == AI && log.isRead() && !aEffect)
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
			else if (state == HEROSELECT)
			{
				switch (selectedHero)
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
		if (state == PLAYER && menu.getSelected() != NOT_SELECTED && log.isRead()) 
		{
			log.stop();
			if (menu.getSelected() == 0)
			{			
				nextPlayerStep();
			}

			if (menu.getSelected() == 1)
			{			
				state = SPELL;
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
				state = ITEM_MENU;
				itemMenu.show();

				selectedHero = 0;
				itemSelected = NOT_SELECTED;
			}

			menu.clean();
			menu.disappear();
		}

		if (state == ITEM_MENU && itemMenu.getSelected() != NOT_SELECTED)
		{
			itemSelected = itemMenu.getSelected() + 1;
			itemMenu.close();

			state = HEROSELECT;
			log.stop();
			log.setString(L"Выберите героя.");

			itemMenu.clean();
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
			SceneManager::instance().getScene().setCurrentEffect("rgb", sf::seconds(0.2));
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
				menu.clean();
				menu.disappear();
				if (spellMenu.getSelectedSpell().getFileName() == "heal")
				{
					switch (currentAttacking)
					{
					case 0:
						if (GameData::instance().getPlayer().getMP() >= spellMenu.getSelectedSpell().getMana())
						{
							state = HEROSELECT;
							log.setString(L"Выберите героя.");
						}
						else
						{
							state = PLAYER;
							SoundManager::instance().playFailSound();

							log.setString(L"Недостаточно маны!");
						}
						break;
					case 1:
						if (GameData::instance().getEmber().getMP() >= spellMenu.getSelectedSpell().getMana())
						{
							state = HEROSELECT;
							log.setString(L"Выберите героя.");
						}
						else
						{
							state = PLAYER;
							SoundManager::instance().playFailSound();

							log.setString(L"Недостаточно маны!");
						}
						break;
					case 2:
						if (GameData::instance().getThunder().getMP() >= spellMenu.getSelectedSpell().getMana())
						{
							state = HEROSELECT;
							log.setString(L"Выберите героя.");
						}
						else
						{
							state = PLAYER;
							SoundManager::instance().playFailSound();

							log.setString(L"Недостаточно маны!");
						}
						break;
					default:
						break;
					}
				}
				else
				{
					state = QTE;

					switch (currentAttacking)
					{
					case 0:
						if (GameData::instance().getPlayer().useMana(spellMenu.getSelectedSpell().getMana()) == true)
						{
							spellQTE.start(spellMenu.getSelectedSpell(), "player");
						}
						else
						{
							state = PLAYER;
							SoundManager::instance().playFailSound();

							log.setString(L"Недостаточно маны!");
						}
						break;
					case 1:
						if (GameData::instance().getEmber().useMana(spellMenu.getSelectedSpell().getMana()) == true)
						{
							spellQTE.start(spellMenu.getSelectedSpell(), "red");
						}
						else
						{
							state = PLAYER;
							SoundManager::instance().playFailSound();

							log.setString(L"Недостаточно маны!");
						}
						break;
					case 2:
						if (GameData::instance().getThunder().useMana(spellMenu.getSelectedSpell().getMana()) == true)
						{
							spellQTE.start(spellMenu.getSelectedSpell(), "blue");
						}
						else
						{
							state = PLAYER;
							SoundManager::instance().playFailSound();

							log.setString(L"Недостаточно маны!");
						}
						break;
					default:
						break;
					}
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
				SceneManager::instance().getScene().setCurrentEffect("rgb", sf::seconds(0.2));
				state = PLAYER;
				bool stop;
				do
				{
					currentAttacking++;
					if (currentAttacking == 1 && GameData::instance().getEmber().getHP() == 0 || currentAttacking == 2 && GameData::instance().getThunder().getHP() == 0)
					{
						stop = false;
					}
					else
					{
						stop = true;
					}
				}
				while (!stop);
				if (currentAttacking >= 3)
				{
					currentAttacking = 0;
					state = AI;
				}
				else
				{
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

			if (Parser::instance().getBackground(spellMenu.getSelectedSpell().getFileName()) == "space")
			{
				currentBackground = spaceType;
			}
			else if (Parser::instance().getBackground(spellMenu.getSelectedSpell().getFileName()) == "fire")
			{
				currentBackground = fireType;
			}
			else if (Parser::instance().getBackground(spellMenu.getSelectedSpell().getFileName()) == "light")
			{
				currentBackground = lightType;
			}
			else if (Parser::instance().getBackground(spellMenu.getSelectedSpell().getFileName()) == "sback")
			{
				currentBackground = sbackType;
			}
			else if (Parser::instance().getBackground(spellMenu.getSelectedSpell().getFileName()) == "creo")
			{
				currentBackground = creoType;
			}
			else if (Parser::instance().getBackground(spellMenu.getSelectedSpell().getFileName()) == "shards")
			{
				currentBackground = shardsType;
			}
			else if (Parser::instance().getBackground(spellMenu.getSelectedSpell().getFileName()) == "hell")
			{
				currentBackground = hellType;
			}
			else if (Parser::instance().getBackground(spellMenu.getSelectedSpell().getFileName()) == "mech")
			{
				currentBackground = mechType;
			}
		}
		else
		{
			if (Level::instance().getDay() <=3)
			{
				currentBackground = fireType;
			}
			else
			{
				currentBackground = sbackType;
			}
			if (Level::instance().getDay() == 6 && Level::instance().getScene() == 7 || Level::instance().getDay() == 6 && Level::instance().getScene() == 8)
			{
				currentBackground = mechType;
			}
		}

		//Hit effects
		he.update(time);

		if (currentBackground == spaceType)
		{
			//Update shader
			space.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
			space.setParameter("seconds", seconds / 50.0);
			space.setParameter("texture1", background);
			space.setParameter("texture2", background2);
			seconds+=1;
		}
		else if (currentBackground == fireType)
		{
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
		else if (currentBackground == sbackType)
		{
			//Update shader
			sback.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
			sback.setParameter("seconds", seconds);
			seconds++;
		}
		else if (currentBackground == lightType)
		{
			//Update shader
			light.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
			light.setParameter("seconds", seconds);
			light.setParameter("pts", 10.f);
			seconds++;
		}
		else if (currentBackground == creoType)
		{
			//Update shader
			creo.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
			creo.setParameter("seconds", seconds);
			seconds++;
		}
		else if (currentBackground == shardsType)
		{
			//Update shader
			shards.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
			shards.setParameter("seconds", seconds/100.0f);
			seconds++;
		}
		else if (currentBackground == hellType)
		{
			//Update shader
			hell.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
			hell.setParameter("seconds", seconds/100.0f);
			seconds++;
		}
		else if (currentBackground == mechType)
		{
			//Update shader
			mech.setParameter("size", sf::Vector2f(WIDTH, HEIGHT));
			mech.setParameter("seconds", seconds/100.0f);
			seconds++;
		}

		if (GameData::instance().getPlayer().getHP() == 0)
		{
			playerSprite.setColor(sf::Color(100,100,100,255));
		}
		else
		{
			playerSprite.setColor(sf::Color(255,255,255,255));
		}
		if (GameData::instance().getEmber().getHP() == 0)
		{
			emberSprite.setColor(sf::Color(100,100,100,255));
		}
		else
		{
			emberSprite.setColor(sf::Color(255,255,255,255));
		}
		if (GameData::instance().getThunder().getHP() == 0)
		{
			thunderSprite.setColor(sf::Color(100,100,100,255));
		}
		else
		{
			thunderSprite.setColor(sf::Color(255,255,255,255));
		}
	}
	else
	{
		//state = ENDED;

		Parser::instance().loadGame();

		GameData::instance().getThunder().clearOffset();
		GameData::instance().getPlayer().clearOffset();
		GameData::instance().getEmber().clearOffset();

		SceneManager::instance().startTransition(Parser::instance().getSceneInfo(Level::instance().getDay(), Level::instance().getScene()));
	}
}

void Battle::damageSpell()
{
	unsigned int dmg;
	if (spellMenu.getSelectedSpell().getFileName() == "heal")
	{
		switch (currentAttacking)
		{
		case 0:
			dmg = (GameData::instance().getPlayer().getIntelligence() / 6) + (rand() % (int)(2));
			break;
		case 1:
			dmg = (GameData::instance().getEmber().getIntelligence() / 6) + (rand() % (int)(2));
			break;
		case 2:
			dmg = (GameData::instance().getThunder().getIntelligence() / 6) + (rand() % (int)(2));
			break;
		default:
			break;
		}
		switch (selectedHero)
		{
		case 0:
			GameData::instance().getPlayer().addHP(dmg);
			effects.show(sf::Vector2f(playerSprite.getPosition().x+ 80, playerSprite.getPosition().y+190), "magic");
			break;
		case 1:
			GameData::instance().getEmber().addHP(dmg);
			effects.show(sf::Vector2f(emberSprite.getPosition().x+ 80, emberSprite.getPosition().y+190), "magic");
			break;
		case 2:
			GameData::instance().getThunder().addHP(dmg);
			effects.show(sf::Vector2f(thunderSprite.getPosition().x + 80, thunderSprite.getPosition().y+190), "magic");
			break;
		default:
			break;
		} 

		SoundManager::instance().playHurtSound();
	}
	else if (spellMenu.getSelectedSpell().getFileName() == "health")
	{
		int diff;
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) == 0)
		{
			diff = 1;
		}
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) == 1)
		{
			diff = 2;
		}
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) >= 2)
		{
			diff = 3;
		}
		if (GameData::instance().getPlayer().getLevel() < Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()))
		{
			diff = 1;
		}

		switch (currentAttacking)
		{
		case 0:
			dmg = (GameData::instance().getPlayer().getIntelligence() / (7-diff)) + (rand() % (int)(2));
			break;
		case 1:
			dmg = (GameData::instance().getEmber().getIntelligence() / (7-diff)) + (rand() % (int)(2));
			break;
		case 2:
			dmg = (GameData::instance().getThunder().getIntelligence() / (7-diff)) + (rand() % (int)(2));
			break;
		default:
			break;
		}

		GameData::instance().getPlayer().addHP(dmg);
		GameData::instance().getEmber().addHP(dmg);
		GameData::instance().getThunder().addHP(dmg);

		effects.showAOE(sf::Vector2f(thunderSprite.getPosition().x + 80, thunderSprite.getPosition().y+190) , sf::Vector2f(emberSprite.getPosition().x+ 80, emberSprite.getPosition().y+190) ,sf::Vector2f(playerSprite.getPosition().x+ 80, playerSprite.getPosition().y+190) ,"magic");

		SoundManager::instance().playHurtSound();
	}
	else if (spellMenu.getSelectedSpell().getFileName() == "wind" || spellMenu.getSelectedSpell().getFileName() == "snow" || spellMenu.getSelectedSpell().getFileName() == "acid" || spellMenu.getSelectedSpell().getFileName() == "burn" || spellMenu.getSelectedSpell().getFileName() == "fire")
	{
		int diff;
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) == 0)
		{
			diff = 1;
		}
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) == 1)
		{
			diff = 2;
		}
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) >= 2)
		{
			diff = 3;
		}
		if (GameData::instance().getPlayer().getLevel() < Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()))
		{
			diff = 1;
		}

		switch (currentAttacking)
		{
		case 0:
			dmg = (GameData::instance().getPlayer().getIntelligence() / (8 - diff)) + (rand() % (int)(2));
			break;
		case 1:
			dmg = (GameData::instance().getEmber().getIntelligence() / (8 - diff)) + (rand() % (int)(2));
			break;
		case 2:
			dmg = (GameData::instance().getThunder().getIntelligence() / (8 - diff)) + (rand() % (int)(2));
			break;
		default:
			break;
		} 

		enemies[selected].playAnimation();
		enemies[selected].getMonster().receiveDamage(dmg);

		if (enemies[selected].isDied())
		{
			res.playerXP += enemies[selected].getMonster().getMaxHP();
			res.emberXP += enemies[selected].getMonster().getMaxHP();
			res.thunderXP += enemies[selected].getMonster().getMaxHP();
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
	else if (spellMenu.getSelectedSpell().getFileName() == "flame" 
		|| spellMenu.getSelectedSpell().getFileName() == "frost")
	{
		int diff;
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) == 0)
		{
			diff = 1;
		}
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) == 1)
		{
			diff = 2;
		}
		if (GameData::instance().getPlayer().getLevel() - Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()) >= 2)
		{
			diff = 3;
		}
		if (GameData::instance().getPlayer().getLevel() < Parser::instance().getSpellLevel(spellMenu.getSelectedSpell().getFileName()))
		{
			diff = 1;
		}

		switch (currentAttacking)
		{
		case 0:
			dmg = (GameData::instance().getPlayer().getIntelligence() / (7-diff)) + (rand() % (int)(2));
			break;
		case 1:
			dmg = (GameData::instance().getEmber().getIntelligence() / (7-diff)) + (rand() % (int)(2));
			break;
		case 2:
			dmg = (GameData::instance().getThunder().getIntelligence() / (7-diff)) + (rand() % (int)(2));
			break;
		default:
			break;
		} 

		for(auto i = enemies.begin(); i != enemies.end(); i++)
		{
			i->playAnimation();
			i->getMonster().receiveDamage(dmg);

			if (i->isDied())
			{
				res.playerXP += i->getMonster().getMaxHP();
				res.emberXP += i->getMonster().getMaxHP();
				res.thunderXP += i->getMonster().getMaxHP();
			}
		}

		for (auto it = enemies.begin(); it != enemies.end(); it++)
		{
			if (!it->isDied())
			{
				selected = it - enemies.begin();
			}
		}

		SoundManager::instance().playHurtSound();
	}
}

void Battle::selectHero()
{
	if (itemSelected == 1)
	{
		switch (selectedHero)
		{
		case 0:
			if (GameData::instance().getPlayer().getHP() > 0 && GameData::instance().useMed())
			{
				GameData::instance().getPlayer().addHP(((float)GameData::instance().getPlayer().getMaxHP() / 100.0f) * 40.0);
				effects.show(sf::Vector2f(playerSprite.getPosition().x+ 80, playerSprite.getPosition().y+190), "magic");
			}			
			break;
		case 1:
			if (GameData::instance().getEmber().getHP() > 0 && GameData::instance().useMed())
			{
				GameData::instance().getEmber().addHP(((float)GameData::instance().getEmber().getMaxHP() / 100.0f) * 40.0);
				effects.show(sf::Vector2f(emberSprite.getPosition().x+ 80, emberSprite.getPosition().y+190), "magic");
			}
			break;
		case 2:
			if (GameData::instance().getThunder().getHP() > 0 && GameData::instance().useMed())
			{
				GameData::instance().getThunder().addHP(((float)GameData::instance().getThunder().getMaxHP() / 100.0f) * 40.0);
				effects.show(sf::Vector2f(thunderSprite.getPosition().x + 80, thunderSprite.getPosition().y+190), "magic");
			}
			break;
		default:
			break;
		}
		SoundManager::instance().playSuccessSound();		

		state = PLAYER;
	}
	else if (itemSelected == 2 && GameData::instance().useMana())
	{
		switch (selectedHero)
		{
		case 0:
			GameData::instance().getPlayer().addMP(((float)GameData::instance().getPlayer().getMaxMP() / 100.0f) * 40.0);
			effects.show(sf::Vector2f(playerSprite.getPosition().x+ 80, playerSprite.getPosition().y+190), "magic");
			break;
		case 1:
			GameData::instance().getEmber().addMP(((float)GameData::instance().getEmber().getMaxMP() / 100.0f) * 40.0);
			effects.show(sf::Vector2f(emberSprite.getPosition().x+ 80, emberSprite.getPosition().y+190), "magic");
			break;
		case 2:
			GameData::instance().getThunder().addMP(((float)GameData::instance().getThunder().getMaxMP() / 100.0f) * 40.0);
			effects.show(sf::Vector2f(thunderSprite.getPosition().x+ 80, thunderSprite.getPosition().y+190), "magic");
			break;
		default:
			break;
		}
		SoundManager::instance().playSuccessSound();

		state = PLAYER;
	}
	else if (itemSelected == 3)
	{
		switch (selectedHero)
		{
		case 0:
			if (GameData::instance().getPlayer().getHP()==0 && GameData::instance().useShard())
			{
				GameData::instance().getPlayer().addHP(GameData::instance().getPlayer().getMaxHP() / 2);
				effects.show(sf::Vector2f(playerSprite.getPosition().x+ 80, playerSprite.getPosition().y+190), "magic");

				SoundManager::instance().playSuccessSound();
			}
			else
			{
				SoundManager::instance().playFailSound();
				log.setString(L"Цель жива.");
			}
			break;
		case 1:
			if (GameData::instance().getEmber().getHP()==0 && GameData::instance().useShard())
			{
				GameData::instance().getEmber().addHP(GameData::instance().getEmber().getMaxHP() / 2);
				effects.show(sf::Vector2f(emberSprite.getPosition().x+ 80, emberSprite.getPosition().y+190), "magic");

				SoundManager::instance().playSuccessSound();
			}
			else
			{
				SoundManager::instance().playFailSound();
				log.setString(L"Цель жива.");
			}		
			break;
		case 2:
			if (GameData::instance().getThunder().getHP()==0 && GameData::instance().useShard())
			{
				GameData::instance().getThunder().addHP(GameData::instance().getThunder().getMaxHP() / 2);
				effects.show(sf::Vector2f(thunderSprite.getPosition().x+ 80, thunderSprite.getPosition().y+190), "magic");

				SoundManager::instance().playSuccessSound();
			}
			else
			{
				SoundManager::instance().playFailSound();	
				log.setString(L"Цель жива.");
			}		
			break;
		default:
			break;
		}

		state = PLAYER;
	}
	else if (spellMenu.getSelectedSpell().getFileName() == "heal")
	{
		switch (currentAttacking)
		{
		case 0:
			if (GameData::instance().getPlayer().useMana(spellMenu.getSelectedSpell().getMana()))
			{
				spellQTE.start(spellMenu.getSelectedSpell(), "player");
			}		
			break;
		case 1:
			if (GameData::instance().getEmber().useMana(spellMenu.getSelectedSpell().getMana()))
			{
				spellQTE.start(spellMenu.getSelectedSpell(), "red");
			}			
			break;
		case 2:
			if (GameData::instance().getThunder().useMana(spellMenu.getSelectedSpell().getMana()))
			{
				spellQTE.start(spellMenu.getSelectedSpell(), "blue");
			}		
			break;
		default:
			break;
		}

		state = QTE;
	}
	else if (spellMenu.getSelectedSpell().getFileName() == "health")
	{
		switch (currentAttacking)
		{
		case 0:
			if (GameData::instance().getPlayer().useMana(spellMenu.getSelectedSpell().getMana()))
			{
				spellQTE.start(spellMenu.getSelectedSpell(), "player");
			}		
			break;
		case 1:
			if (GameData::instance().getEmber().useMana(spellMenu.getSelectedSpell().getMana()))
			{
				spellQTE.start(spellMenu.getSelectedSpell(), "red");
			}			
			break;
		case 2:
			if (GameData::instance().getThunder().useMana(spellMenu.getSelectedSpell().getMana()))
			{
				spellQTE.start(spellMenu.getSelectedSpell(), "blue");
			}		
			break;
		default:
			break;
		}

		state = QTE;
	}
	itemSelected = NOT_SELECTED;
	//selectedHero = 0;
}

void Battle::damageMonster()
{
	int dmg;
	int crit;
	sf::String tmpw;
	//Count damage
	switch (currentAttacking)
	{
	case 0:
		dmg = (GameData::instance().getPlayer().getStrength() / 4 + GameData::instance().getPlayer().getAgility() / 5 + GameData::instance().getPlayer().getIntelligence() / 6) + (rand() % (int)(2));
		tmpw = L"Джейд";
		crit = GameData::instance().getPlayer().getWeapon().getLevel();
		break;
	case 1:
		dmg = (GameData::instance().getEmber().getStrength() / 4 + GameData::instance().getEmber().getAgility() / 5 + GameData::instance().getEmber().getIntelligence() / 6) + (rand() % (int)(2));
		tmpw = L"Эмбер";
		crit = GameData::instance().getEmber().getWeapon().getLevel();
		break;
	case 2:
		dmg = (GameData::instance().getThunder().getStrength() / 4 + GameData::instance().getThunder().getAgility() / 5 + GameData::instance().getThunder().getIntelligence() / 6) + (rand() % (int)(2));
		tmpw = L"Сандер";
		crit = GameData::instance().getThunder().getWeapon().getLevel();
		break;
	default:
		break;
	} 

	he.init(sf::Vector2f((enemies[selected].getPosition().x * 2) + (enemies[selected].getTexture()->getSize().x / 2) -  (HEIGHT / 2) + 40, enemies[selected].getPosition().y + (enemies[selected].getTexture()->getSize().y / 2)));

	log.stop();

	if ((rand() % (int)(20 - (crit * 2))) == 1)
	{
		dmg *= 5;
		log.setString(tmpw.toWideString() + L" выносит " + std::to_wstring(dmg) + L" урона. Критический удар!");
		SoundManager::instance().playCritSound();

		damageEffects.push_back(Damage(sf::Vector2f(enemies[selected].getPosition().x * 2 + 50, 
			enemies[selected].getPosition().y + (enemies[selected].getTexture()->getSize().y)), "CRIT"));
	}
	else
	{
		log.setString(tmpw.toWideString() + L" наносит " + std::to_wstring(dmg) + L" урона. Отличный удар!");
		SoundManager::instance().playHitSound();

		damageEffects.push_back(Damage(sf::Vector2f(enemies[selected].getPosition().x * 2 + 50, 
			enemies[selected].getPosition().y + (enemies[selected].getTexture()->getSize().y)), "-" + std::to_string(dmg)));
	}

	enemies[selected].playAnimation();
	enemies[selected].getMonster().receiveDamage(dmg);

	if (enemies[selected].isDied())
	{
		res.playerXP += enemies[selected].getMonster().getMaxHP();
		res.emberXP += enemies[selected].getMonster().getMaxHP();
		res.thunderXP += enemies[selected].getMonster().getMaxHP();
		//Select next alive enemy
		for (auto i = enemies.begin(); i != enemies.end(); i++)
		{
			if (!i->isDied())
			{
				selected = i - enemies.begin();
			}
		}
	}
}

void Battle::nextPlayerStep()
{
	if (state == PLAYER)
	{
		SoundManager::instance().playHurtSound();
		damageMonster();

		bool stop;
		do
		{
			currentAttacking++;
			if (currentAttacking == 1 && GameData::instance().getEmber().getHP() == 0 || currentAttacking == 2 && GameData::instance().getThunder().getHP() == 0)
			{
				stop = false;
			}
			else
			{
				stop = true;
			}
		}
		while (!stop);
	}
}

void Battle::clean()
{
	//Should i do it in start()?
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
			currentAttacking = -1;
			bool stop;
			do
			{
				currentAttacking++;
				if (currentAttacking == 0 && GameData::instance().getPlayer().getHP() == 0 
					|| currentAttacking == 1 && GameData::instance().getEmber().getHP() == 0 
					|| currentAttacking == 2 && GameData::instance().getThunder().getHP() == 0)
				{
					stop = false;
				}
				else
				{
					stop = true;
					break;
				}
			}
			while (!stop);
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
	if (DialoguePanel::instance().isEnded())DialoguePanel::instance().input(event);

	if (!aEffect)
	{
		//For some fucking reason only works if update log's input firstly
		log.input(event);
		if (menu.isWorking() && state == PLAYER)
		{
			menu.input(event);
		}

		if (state == ITEM_MENU)
		{
			itemMenu.input(event);
		}

		if (((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A)  && event.type == sf::Event::JoystickButtonPressed)))
		{
			if (currentAttacking >= 3 && state == PLAYER && log.isRead())
			{
				currentAttacking = 0;
				state = AI;
			}
		}

		//Select fucking hero
		if (((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A)  && event.type == sf::Event::JoystickButtonPressed)) && state == HEROSELECT)
		{
			selectHero();
			log.stop();
		}
		else if (((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) || (xb::Joystick::isButtonPressed(0, xb::Joystick::B)  && event.type == sf::Event::JoystickButtonPressed)) && state == HEROSELECT)
		{
			state = PLAYER;
			log.stop();
			log.setString(L"Атакуйте!");		
		}
		if (((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) || (xb::Joystick::isButtonPressed(0, xb::Joystick::B)  && event.type == sf::Event::JoystickButtonPressed)) && itemMenu.isWorking() && state == ITEM_MENU)
		{
			state = PLAYER;
			log.stop();
			log.setString(L"Атакуйте!");		
		}

		//Input qtes
		if (state == QTE)
		{
			spellQTE.input(event);
		}

		//End battle
		if (((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A)  && event.type == sf::Event::JoystickButtonPressed)) && state == ENDED && log.isRead())
		{
			//GameData::instance().appendResult(res);

			if (Level::instance().getDay() == 6)
			{
				if (Level::instance().getScene() == 7)
				{
					GameData::instance().getPlayer().addXP(res.playerXP);
					GameData::instance().getEmber().addXP(res.emberXP);
					GameData::instance().getThunder().addXP(res.thunderXP);
					SceneManager::instance().endBattleB();

					Level::instance().nextScene();

					SceneManager::instance().startTransition(Parser::instance().getSceneInfo(Level::instance().getDay(), Level::instance().getScene()));
				}
				else if  (Level::instance().getScene() == 8)
				{
					SceneManager::instance().endBattleB();

					if (GameData::instance().getPlayer().getSocial() >= 11)
					{
						DialoguePanel::instance().openDialogue("misc", "scene8");
						SoundManager::instance().playSuccessSound();
					}
					else
					{
						DialoguePanel::instance().openDialogue("misc", "scene9");
						SoundManager::instance().playTransitionSound();
					}	
				}
				else
				{
					GameData::instance().getPlayer().addXP(res.playerXP);
					GameData::instance().getEmber().addXP(res.emberXP);
					GameData::instance().getThunder().addXP(res.thunderXP);
					SceneManager::instance().endBattle();

				}
			}
			else
			{
				if (Level::instance().getScene() != 7 && Level::instance().getDay() != 6)
				{
					GameData::instance().getPlayer().addXP(res.playerXP);
					GameData::instance().getEmber().addXP(res.emberXP);
					GameData::instance().getThunder().addXP(res.thunderXP);
					SceneManager::instance().endBattle();
				}
			}
		}

		//Open menu when player's turn
		if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A)  && event.type == sf::Event::JoystickButtonPressed)) && state == PLAYER && !menu.isWorking() && log.isRead() && !enemies[selected].isDied())
		{
			menu.appear(sf::Vector2f(enemies[selected].getPosition().x * 2, enemies[selected].getPosition().y));
		}

		//Close menu on Z
		if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) || (xb::Joystick::isButtonPressed(0, xb::Joystick::B)  && event.type == sf::Event::JoystickButtonPressed)) && state == PLAYER && menu.isWorking())
		{
			menu.disappear();
			SoundManager::instance().playSelectSound();
		}

		//Close spells menu
		if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) || (xb::Joystick::isButtonPressed(0, xb::Joystick::B)  && event.type == sf::Event::JoystickButtonPressed)) && state == SPELL && spellMenu.isWorking())
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
		if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) || (xb::Joystick::isButtonPressed(0, xb::Joystick::A)  && event.type == sf::Event::JoystickButtonPressed)) && !log.isRead() && state == AI)
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

				SoundManager::instance().playSelectSound();
			}
		}

		//Update selected hero FUCK IT FFS
		if (state == HEROSELECT)
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && !menu.isWorking())
			{
				if (selectedHero > 0)
				{
					selectedHero--;
				}
				else
				{
					selectedHero = 2;
				}


				SoundManager::instance().playSelectSound();
			}
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && !menu.isWorking())
			{
				if (selectedHero < 2)
				{
					selectedHero++;
				}
				else
				{
					selectedHero = 0;
				}

				SoundManager::instance().playSelectSound();
			}
		}
	}
}