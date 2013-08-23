#include "i_ui.h"

#include "h_config.h"
#include "d_gamedata.H"
#include "g_scenemanager.h"

#include "os_x360controller.hpp"

StatsWindow::StatsWindow() : listener(goMap, state), listener2(state)
{
	back.setTexture(TextureManager::instance().getTexture("assets/girl_screen.jpg"));

	for (int i = 0; i < 5; i++)
	{
		stats[i].setFont(DFont::instance().getGothicFont());
		stats[i].setPosition(164, 100 + (120*i));
		stats[i].setCharacterSize(41);
		stats[i].setColor(sf::Color(249, 217, 36));
	}
	current = 0;
	state = IDLE;

	heroStats[0].setTexture(TextureManager::instance().getTexture("assets/stat_girl.png"));
	heroStats[1].setTexture(TextureManager::instance().getTexture("assets/stat_red.png"));
	heroStats[2].setTexture(TextureManager::instance().getTexture("assets/stat_blue.png"));

	heroBars[0].setTexture(TextureManager::instance().getTexture("assets/bluebar.png"));
	heroBars[1].setTexture(TextureManager::instance().getTexture("assets/bluebar.png"));
	heroBars[0].setOrigin(0, 325);
	heroBars[1].setOrigin(0, 325);

	name.setFont(DFont::instance().getFont());
	name.setPosition(1273, 71);
	name.setCharacterSize(55);
	name.setColor(sf::Color(255, 0, 96,255));
	name.setRotation(86);

	feeling.setFont(DFont::instance().getFont());
	feeling.setPosition(747, 487);
	//feeling.setCharacterSize(55);
	feeling.setColor(sf::Color::Black);
	character = 0;

	speech.setTexture(TextureManager::instance().getTexture("assets/speech.png"));
	speech.setPosition(697, 320);
	speech.setColor(sf::Color(255,255,255,0));
}

void StatsWindow::setText(std::wstring wstr)
{
	feeling.setString("");
	character = 0;
	clock.restart();
	actualString = GameData::instance().wordWrap3(wstr);
}

void StatsWindow::close()
{
	if (state != DIS)
	{
		speech.setColor(sf::Color(255,255,255,0));
		setText(L"");

		newY = 0;
		back.setPosition(0, 0);
		oTweener.addTween(&CDBTweener::TWEQ_ELASTIC, CDBTweener::TWEA_INOUT, 3.0f, &newY, 800.0f);
		for (int i = 0; i < 5; i++)
		{
			statsPos[i] = stats[i].getPosition().y;
		}
		state = DIS;
		oTweener.addListener(&listener);
	}
}

void StatsWindow::show()
{
	if (state != FALLING)
	{
		speech.setColor(sf::Color(255,255,255,0));
		setText(getFeeling());

		newY = -800;
		back.setPosition(0, -800);
		for (int i = 0; i < 5; i++)
		{
			stats[i].setPosition(164, 100 + (120*i));
			statsPos[i] = stats[i].getPosition().y;	
			stats[i].move(0, -HEIGHT);
		}
		oTweener.addTween(&CDBTweener::TWEQ_ELASTIC, CDBTweener::TWEA_INOUT, 3.0f, &newY, 0.0f);
		oTweener.addListener(&listener2);
		state = FALLING;
	}	
}

void StatsWindow::draw(sf::RenderTarget &tg)
{
	tg.draw(back);
	for (int i = 0; i < 5; i++)
	{
		tg.draw(stats[i]);
	}
	switch (current)
	{
	case 0:
		tg.draw(heroStats[1]);
		tg.draw(heroStats[2]);
		break;
	case 1:
		tg.draw(heroStats[0]);
		tg.draw(heroStats[2]);
		break;
	case 2:
		tg.draw(heroStats[1]);
		tg.draw(heroStats[0]);
		break;
	default:	
		break;
	}
	tg.draw(heroBars[1]);
	tg.draw(heroBars[0]);
	tg.draw(name);
	tg.draw(speech);
	tg.draw(feeling);
}

std::wstring StatsWindow::getFeeling()
{
	std::wstring str;

	float hp;

	switch (current)
	{
	case 0:		
		hp = GameData::instance().getPlayer().getHP();
		if (hp > (float)((float)GameData::instance().getPlayer().getMaxHP()/100.f)*(float)80.0)
		{
			str = L"Я в порядке.";
		}
		else
		{
			str = L"Мне нужно лечение.";
		}

		if ((float)GameData::instance().getPlayer().getMP() < (float)GameData::instance().getPlayer().getMaxMP() / (float)3.0)
		{
			str = L"Я устала, я должна восстановить свои силы.";
		}
		else
		{
			//str = L"...";
		}
		break;
	case 1:
		if ((float)GameData::instance().getEmber().getHP() > (float)((float)GameData::instance().getEmber().getMaxHP()/100.f)*(float)8.00)
		{
			str = L"Со мной все хорошо.";
		}
		else if ((float)GameData::instance().getEmber().getMP() < (float)GameData::instance().getEmber().getMaxMP() / (float)3.0)
		{
			str = L"Мои силы иссякли, мне нужен отдых.";
		}
		else if ((float)GameData::instance().getEmber().getHP() < (float)GameData::instance().getEmber().getMaxHP() / (float)2.0)
		{
			str = L"Мне нужно лечение.";
		}
		else
		{
			str = L"...";
		}
		break;
	case 2:
		if ((float)GameData::instance().getThunder().getHP() > (float)((float)GameData::instance().getThunder().getMaxHP()/100.f)*(float)80.0)
		{
			str = L"Я отлично себя чувствую.";
		}
		else if ((float)GameData::instance().getThunder().getMP() < (float)GameData::instance().getThunder().getMaxMP() / (float)3.0)
		{
			str = L"Мне нужно передохнуть после такой нагрузки!.";
		}
		else if ((float)GameData::instance().getThunder().getHP() < (float)GameData::instance().getThunder().getMaxHP() / (float)2.0)
		{
			str = L"Мне бы не помешало бы лечение.";
		}
		else
		{
			str = L"...";
		}
		break;
	default:
		break;
	} 
	return str;
}

void StatsWindow::update(sf::Time time)
{
	oTweener.step((float)time.asMilliseconds() * 0.001f);

	back.setPosition(0, newY);		

	for (int i = 0; i < 5; i++)
	{
		stats[i].setPosition(stats[i].getPosition().x, newY + statsPos[i]);
	}

	if (state == IDLE)
	{
		speech.setColor(sf::Color(255,255,255,interpolateLinear(speech.getColor().a, 255, 0.1f)));

		sf::Uint32 elapsed = clock.getElapsedTime().asMilliseconds();
		if (speech.getColor().a > 200)
		{
			if (elapsed > 60 && character < actualString.getSize())
			{
				//Play sound if it is not a space
				if (feeling.getString().toAnsiString().c_str()[feeling.getString().getSize()] != ' ' && character % 2 == 0)
				{
					SoundManager::instance().playClickSound();
				}
				if (feeling.getString().getSize() + 1 >= actualString.getSize())
				{
					//If last - play enter sound
					SoundManager::instance().playEnterSound();
				}
				clock.restart();
				character+=2;
				feeling.setString( sf::String(actualString.toWideString().substr(0, character)));				
			}
		}	

		switch (current)
		{
		case 0:
			heroStats[1].setPosition(170, 800 - 672);
			heroStats[2].setPosition(100 + 365, 800 - 672);
			break;
		case 1:
			heroStats[0].setPosition(170, 800 - 672);
			heroStats[2].setPosition(100 + 365, 800 - 672);
			break;
		case 2:
			heroStats[1].setPosition(170, 800 - 672);
			heroStats[0].setPosition(100 + 365, 800 - 672);
			break;
		default:	
			break;
		}

		heroBars[0].setPosition(337, 0 + 660 - (325/2));
		heroBars[1].setPosition(305 + 326, 0 + 660 - (325/2));
		name.setPosition(1273, 71);
	}
	else
	{
		switch (current)
		{
		case 0:
			heroStats[1].setPosition(170, newY + 800 - 672);
			heroStats[2].setPosition(100 + 365, newY + 800 - 672);
			break;
		case 1:
			heroStats[0].setPosition(170, newY + 800 - 672);
			heroStats[2].setPosition(100 + 365, newY + 800 - 672);
			break;
		case 2:
			heroStats[1].setPosition(170, newY + 800 - 672);
			heroStats[0].setPosition(100 + 365, newY + 800 - 672);
			break;
		default:	
			break;
		}

		heroBars[0].setPosition(337, newY + 0 + 660- (325/2));
		heroBars[1].setPosition(305 + 326, newY + 0 + 660 - (325/2));
		name.setPosition(1273, newY + 71);
	}

	float p;

	switch (current)
	{
	case 0:
		p = ((float)((float)GameData::instance().getPlayer().getSocial() + (float)GameData::instance().getEmber().getSocial()) / 50.0f) * 100.0f;
		heroBars[0].setTextureRect(sf::IntRect(0, 0, 29, (325/100.f)*p));
		p = ((float)((float)GameData::instance().getPlayer().getSocial() + (float)GameData::instance().getThunder().getSocial()) / 50.0f) * 100.0f;
		heroBars[1].setTextureRect(sf::IntRect(0, 0, 29,  (325/100.f)*p));

		name.setString(L"Джейд");
		break;
	case 1:
		p = ((float)((float)GameData::instance().getEmber().getSocial() + (float)GameData::instance().getPlayer().getSocial()) / 50.0f) * 100.0f;
		heroBars[0].setTextureRect(sf::IntRect(0, 0, 29, (325/100.f)*p));
		p = ((float)((float)GameData::instance().getEmber().getSocial() + (float)GameData::instance().getThunder().getSocial()) / 50.0f) * 100.0f;
		heroBars[1].setTextureRect(sf::IntRect(0, 0, 29,  (325/100.f)*p));

		name.setString(L"Эмбер");
		break;
	case 2:
		p = ((float)((float)GameData::instance().getThunder().getSocial() + (float)GameData::instance().getEmber().getSocial()) / 50.0f) * 100.0f;
		heroBars[0].setTextureRect(sf::IntRect(0, 0, 29, (325/100.f)*p));
		p = ((float)((float)GameData::instance().getThunder().getSocial() + (float)GameData::instance().getPlayer().getSocial()) / 50.0f) * 100.0f;
		heroBars[1].setTextureRect(sf::IntRect(0, 0, 29,  (325/100.f)*p));

		name.setString(L"Сандер");
		break;
	default:	
		break;
	}

	switch (current)
	{
	case 0:
		stats[0].setString(std::to_string(GameData::instance().getPlayer().getHP()));
		stats[1].setString(std::to_string(GameData::instance().getPlayer().getMP()));
		stats[2].setString(std::to_string(GameData::instance().getPlayer().getStrength()));
		stats[3].setString(std::to_string(GameData::instance().getPlayer().getAgility()));
		stats[4].setString(std::to_string(GameData::instance().getPlayer().getIntelligence()));
		break;
	case 1:
		stats[0].setString(std::to_string(GameData::instance().getEmber().getHP()));
		stats[1].setString(std::to_string(GameData::instance().getEmber().getMP()));
		stats[2].setString(std::to_string(GameData::instance().getEmber().getStrength()));
		stats[3].setString(std::to_string(GameData::instance().getEmber().getAgility()));
		stats[4].setString(std::to_string(GameData::instance().getEmber().getIntelligence()));
		break;
	case 2:
		stats[0].setString(std::to_string(GameData::instance().getThunder().getHP()));
		stats[1].setString(std::to_string(GameData::instance().getThunder().getMP()));
		stats[2].setString(std::to_string(GameData::instance().getThunder().getStrength()));
		stats[3].setString(std::to_string(GameData::instance().getThunder().getAgility()));
		stats[4].setString(std::to_string(GameData::instance().getThunder().getIntelligence()));
		break;
	default:	
		break;
	}
	if (goMap)
	{
		SceneManager::instance().getScene().setMapState();
		goMap = false;
		oTweener.removeListener(&listener);
	}
}

void StatsWindow::input(sf::Event &event)
{	
	if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) || (xb::Joystick::isButtonPressed(0, xb::Joystick::B)  && event.type == sf::Event::JoystickButtonPressed) && state == IDLE)
	{
		close();
	}
	if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) || (xb::Joystick::getAxisDir() == DIR_LEFT && event.type == sf::Event::JoystickMoved)) && state == IDLE)
	{
		if (current > 0)
		{
			current--;
		}
		else
		{
			current = 2;
		}

		speech.setColor(sf::Color(255,255,255,0));
		setText(getFeeling());

		SoundManager::instance().playSelectSound();SceneManager::instance().getScene().setCurrentEffect("distortion", sf::seconds(0.1f));
	}
	if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) || (xb::Joystick::getAxisDir() == DIR_RIGHT && event.type == sf::Event::JoystickMoved)) && state == IDLE)
	{
		if (current + 1 < 3)
		{
			current++;
		}
		else
		{
			current = 0;
		}

		speech.setColor(sf::Color(255,255,255,0));
		setText(getFeeling());

		SoundManager::instance().playSelectSound();SceneManager::instance().getScene().setCurrentEffect("distortion", sf::seconds(0.1f));
	}

	switch (current)
	{
	case 0:
		back.setTexture(TextureManager::instance().getTexture("assets/girl_screen.jpg"));
		break;
	case 1:
		back.setTexture(TextureManager::instance().getTexture("assets/red_screen.jpg"));
		break;
	case 2:
		back.setTexture(TextureManager::instance().getTexture("assets/blue_screen.jpg"));
		break;
	default:
		break;
	}
}

Tip::Tip(std::wstring string, unsigned int k, std::string newType)
{
	key = k;
	type = newType;

	text.setFont(DFont::instance().getFont());
	text.setColor(sf::Color::White);
	if (newType == "black")
		text.setColor(sf::Color::Black);
	text.setString(string);

	switch (k)
	{
	case UP:
		sprite.setTexture(TextureManager::instance().getTexture("assets/up.png"));
		break;
	case RIGHT:
		sprite.setTexture(TextureManager::instance().getTexture("assets/right.png"));
		break;
	case DOWN:
		sprite.setTexture(TextureManager::instance().getTexture("assets/down.png"));
		break;
	case LEFT:
		sprite.setTexture(TextureManager::instance().getTexture("assets/left.png"));
		break;
	case _Z:
		sprite.setTexture(TextureManager::instance().getTexture("assets/Z.png"));
		break;
	case _X:
		sprite.setTexture(TextureManager::instance().getTexture("assets/X.png"));
		break;
	default:
		break;
	}

	sprite.scale(0.3, 0.3);
}

void Tip::draw(sf::RenderTarget &tg, sf::Vector2f pos)
{
	sprite.setPosition(pos);
	text.setPosition(pos);
	text.move(50, 1);
	tg.draw(sprite);
	tg.draw(text);
}

XPBar::XPBar()
{
	portrait.setTexture(TextureManager::instance().getTexture("assets/portrait.png"));
	xpbar.setTexture(TextureManager::instance().getTexture("assets/xpbar.png"));
	xpbar.move(253, 0);
	level.setFont(DFont::instance().getFont());
	level.setPosition(HALF_WIDTH - 90, 17);
}

void XPBar::draw(sf::RenderTarget &tg)
{
	sf::IntRect rect = xpbar.getTextureRect();
	xpbar.setTextureRect(sf::IntRect(0,0, 737, 67));
	xpbar.setColor(sf::Color(255,255,255,100));
	tg.draw(xpbar);
	xpbar.setColor(sf::Color(255,255,255,255));
	xpbar.setTextureRect(rect);
	tg.draw(xpbar);
	tg.draw(portrait);
	tg.draw(level);
}

void XPBar::update(sf::Time time)
{
	level.setString(L"Уровень " + std::to_wstring(GameData::instance().getPlayer().getLevel()));
	float p = ((float)GameData::instance().getPlayer().getXP() / (float)GameData::instance().getPlayer().getNextLevelXP()) * 100.0;
	xpbar.setTextureRect(sf::IntRect(0,0, (737 / 100) * p, 67));
}

Days::Days()
{
	back.setTexture(TextureManager::instance().getTexture("assets/day.png"));
	dayText.setFont(DFont::instance().getFont());

	back.setPosition(WIDTH - back.getTextureRect().width, 0);
	dayText.setPosition(back.getPosition().x + 154, 33);

	dayText.setCharacterSize(20);
	dayText.setColor(sf::Color::Black);
	//dayText.setStyle(sf::Text::Bold);
}

void Days::draw(sf::RenderTarget &tg)
{
	tg.draw(back);
	tg.draw(dayText);
}

void Days::update(sf::Time time)
{
	std::wstring day;
	int dl = 1;
	switch (Level::instance().getDay())
	{
	case 1:
		day = L"Понедельник";
		dl = 1;
		break;
	case 2:
		day = L"Вторник";
		dl = 2;
		break;
	case 3:
		day = L"Среда";
		dl = 3;
		break;
	case 4:
		day = L"Четверг";
		dl = 4;
		break;
	case 5:
		day = L"Пятница";
		dl = 5;
		break;
	case 6:
		day = L"Суббота";
		dl = 6;
		break;
	case 7:
		day = L"Понедельник";
		dl = 8;
		break;
	case 8:
		day = L"Понедельник";
		dl = 8;
		break;
	default:
		break;
	}
	dayText.setString(L"0"+std::to_wstring(dl)+L"/09, " + day);
}

Shop::ShopItem::ShopItem(std::wstring name)
{
	n = name;
}

void Shop::ShopItem::init()
{
	if (n == L"Медикаменты")
	{
		sprite.setTexture(TextureManager::instance().getTexture("assets/icons/item_health.png"));
		text.setString(L"Лечит часть максимального здоровья. (x" + std::to_wstring(GameData::instance().getMeds()) + L")");
		cost.setString(L"500¥");
		c = 500;
	}
	if (n == L"Эфир")
	{
		sprite.setTexture(TextureManager::instance().getTexture("assets/icons/item_mana.png"));
		text.setString(L"Восстанавливает часть максимальной маны. (x" + std::to_wstring(GameData::instance().getManas()) + L")");
		cost.setString(L"600¥");
		c = 600;
	}
	if (n == L"Феникс")
	{
		sprite.setTexture(TextureManager::instance().getTexture("assets/icons/item_res.png"));
		text.setString(L"Воскрешает мертвого героя. (x" + std::to_wstring(GameData::instance().getShards()) + L")");
		cost.setString(L"1500¥");
		c = 1500;
	}
	if (n == L"Клеймор")
	{
		sprite.setTexture(TextureManager::instance().getTexture("assets/icons/sword_" + std::to_string(GameData::instance().getPlayer().getWeapon().getLevel()) +".png"));
		text.setString(L"Тяжелый двуручный меч.");	
		switch (GameData::instance().getPlayer().getWeapon().getLevel())
		{
		case 1:
			cost.setString(L"5000¥");
			c = 5000;
			break;
		case 2:
			cost.setString(L"10000¥");
			c= 10000;
			break;
		default:
			cost.setString(L"N/A");
			break;
		}
	}
	if (n == L"Посох")
	{
		sprite.setTexture(TextureManager::instance().getTexture("assets/icons/staff_" + std::to_string(GameData::instance().getEmber().getWeapon().getLevel()) +".png"));
		text.setString(L"Легкий посох, помогающий использовать магию.");
		switch (GameData::instance().getEmber().getWeapon().getLevel())
		{
		case 1:
			cost.setString(L"5000¥");
			c = 5000;
			break;
		case 2:
			cost.setString(L"10000¥");
			c= 10000;
			break;
		default:
			cost.setString(L"N/A");
			break;
		}
	}
	if (n == L"Катана")
	{
		sprite.setTexture(TextureManager::instance().getTexture("assets/icons/katana_" + std::to_string(GameData::instance().getThunder().getWeapon().getLevel()) +".png"));
		text.setString(L"Длинный изогнутый меч с односторонним клинком.");
		switch (GameData::instance().getThunder().getWeapon().getLevel())
		{
		case 1:
			cost.setString(L"5000¥");
			c = 5000;
			break;
		case 2:
			cost.setString(L"10000¥");
			c= 10000;
			break;
		default:
			cost.setString(L"N/A");
			break;
		}
	}

	text.setColor(sf::Color::White);
	text.setFont(DFont::instance().getFont());

	cost.setColor(sf::Color::White);
	cost.setFont(DFont::instance().getFont());
}

void Shop::show()
{
	const unsigned int startX = WIDTH / 3;
	const unsigned int startY = HEIGHT / 7;

	verticalPointer.setPosition(startX + (CHARACTER_SIZE / 2), 0);
	horizontalPointer.setPosition(sf::Vector2f(0, items[selection].getPosition().y));

	verticalPointer.setFillColor(YELLOW);
	horizontalPointer.setFillColor(YELLOW);

	selection = 0;

	working = true;

	blackRect.setSize(sf::Vector2f(WIDTH, HEIGHT));
	blackRect.setFillColor(sf::Color(0,0,0,190));

	for (auto i = items.begin(); i != items.end(); i++)
	{
		i->init();
	}
}

void Shop::close()
{
	working = false;
}

void Shop::draw(sf::RenderTarget &tg)
{
	if (working)
	{
		tg.draw(blackRect);

		verticalPointer.setFillColor(BLUE);
		horizontalPointer.setFillColor(BLUE);

		tg.draw(verticalPointer);
		tg.draw(horizontalPointer);

		verticalPointer.setFillColor(YELLOW);
		horizontalPointer.setFillColor(YELLOW);

		verticalPointer.move(3, 0);
		horizontalPointer.move(0, -3);

		tg.draw(verticalPointer);
		tg.draw(horizontalPointer);

		verticalPointer.move(-3, 0);
		horizontalPointer.move(0, 3);

		for (auto i = items.begin(); i != items.end(); i++)
		{
			i->draw(tg);
		}

		tg.draw(money);
	}
}

void Shop::update(sf::Time time)
{
	if (working)
	{
		for (auto i = items.begin(); i != items.end(); i++)
		{
			i->init();
		}

		money.setString(L"Баланс: " + std::to_wstring(GameData::instance().getMoney()) + L"¥");

		horizontalPointer.setPosition(sf::Vector2f(0, items[selection].getPosition().y + (CHARACTER_SIZE / 2)));

		for (auto i = items.begin(); i != items.end(); i++)
		{
			i->update(i - items.begin() == selection ? true : false);
			if (i - items.begin() == selection)
			{
				i->setPosition(sf::Vector2f(WIDTH / 3 + 10, i->getPosition().y));
			}
			else
			{
				i->setPosition(sf::Vector2f(WIDTH / 3, i->getPosition().y));
			}
		}

		loaded = true;
	}
}

void Shop::input(sf::Event &event)
{
	if (working && loaded)
	{
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			if (GameData::instance().useMoney(items[selection].getCost()))
			{
				std::cout << "cost: " + std::to_string(items[selection].getCost());
				SoundManager::instance().playPressSound();

				switch (selection)
				{
				case 0:
					GameData::instance().addMed();
					break;
				case 1:
					GameData::instance().addMana();
					break;
				case 2:
					GameData::instance().addShard();
					break;
				case 3:
					if (!GameData::instance().getPlayer().getWeapon().upgrade())
					{
						GameData::instance().addMoney(items[selection].getCost());
					}
					break;
				case 4:
					if (!GameData::instance().getEmber().getWeapon().upgrade())
					{
						GameData::instance().addMoney(items[selection].getCost());
					}
					break;
				case 5:
					if (!GameData::instance().getThunder().getWeapon().upgrade())
					{
						GameData::instance().addMoney(items[selection].getCost());
					}
					break;
				default:
					break;
				}
			}
			else
			{
				SoundManager::instance().playFailSound();
			}		
		}
		if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) || (xb::Joystick::getAxisDir() == DIR_UP)))
		{
			if (selection > 0 )
			{
				selection--;
			}
			else
			{
				selection = items.size();
				selection--;
			}

			SoundManager::instance().playSelectSound();
		}
		if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) || (xb::Joystick::getAxisDir() == DIR_DOWN)))
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

Shop::Shop()
{
	const unsigned int startX = WIDTH / 3;
	const unsigned int startY = HEIGHT / 7;

	money.setFont(DFont::instance().getFont());
	money.setColor(sf::Color::White);
	money.setPosition(startX + 95, startY - 30);

	horizontalPointer = sf::RectangleShape(sf::Vector2f(WIDTH, CHARACTER_SIZE));
	verticalPointer = sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, HEIGHT));

	ShopItem item(L"Медикаменты");
	item.init();
	item.setPosition(sf::Vector2f(startX, 15 + startY + (0 * CHARACTER_SIZE * 3)));
	items.push_back(item);

	item = ShopItem(L"Эфир");
	item.init();
	item.setPosition(sf::Vector2f(startX, 15 + startY + (1 * CHARACTER_SIZE * 3)));
	items.push_back(item);

	item = ShopItem(L"Феникс");
	item.init();
	item.setPosition(sf::Vector2f(startX, 15 + startY + (2 * CHARACTER_SIZE * 3)));
	items.push_back(item);

	item = ShopItem(L"Клеймор");
	item.init();
	item.setPosition(sf::Vector2f(startX, 15 + startY + (3 * CHARACTER_SIZE * 3)));
	items.push_back(item);

	item = ShopItem(L"Посох");
	item.init();
	item.setPosition(sf::Vector2f(startX, 15 + startY + (4 * CHARACTER_SIZE * 3)));
	items.push_back(item);

	item = ShopItem(L"Катана");
	item.init();
	item.setPosition(sf::Vector2f(startX, 15 + startY + (5 * CHARACTER_SIZE * 3)));
	items.push_back(item);

	loaded = false;
}