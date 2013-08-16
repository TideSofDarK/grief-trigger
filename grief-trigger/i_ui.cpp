#include "i_ui.h"

#include "h_config.h"
#include "d_gamedata.H"
#include "g_scenemanager.h"

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

void StatsWindow::show()
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
	switch (current)
	{
	case 0:
		if (GameData::instance().getPlayer().getHP() > (float)((float)GameData::instance().getPlayer().getMaxHP()/100.f)*(float)80)
		{
			return L"Я в порядке.";
		}
		if (GameData::instance().getPlayer().getMP() < GameData::instance().getPlayer().getMaxMP() / 3)
		{
			return L"Я устала, я должна восстановить свои силы.";
		}
		if (GameData::instance().getPlayer().getHP() < GameData::instance().getPlayer().getMaxHP() / 2)
		{
			return L"Мне нужно лечение.";
		}
		break;
	case 1:
		if (GameData::instance().getPlayer().getHP() > (float)((float)GameData::instance().getPlayer().getMaxHP()/100.f)*(float)80)
		{
			return L"Со мной все хорошо.";
		}
		if (GameData::instance().getPlayer().getMP() < GameData::instance().getPlayer().getMaxMP() / 3)
		{
			return L"Мои силы иссякли, мне нужен отдых.";
		}
		if (GameData::instance().getPlayer().getHP() < GameData::instance().getPlayer().getMaxHP() / 2)
		{
			return L"Мне нужно лечение.";
		}
		break;
	case 2:
		if (GameData::instance().getPlayer().getHP() > (float)((float)GameData::instance().getPlayer().getMaxHP()/100.f)*(float)80)
		{
			return L"Я отлично себя чувствую.";
		}
		if (GameData::instance().getPlayer().getMP() < GameData::instance().getPlayer().getMaxMP() / 3)
		{
			return L"Мне нужно передохнуть после такой нагрузки!.";
		}
		if (GameData::instance().getPlayer().getHP() < GameData::instance().getPlayer().getMaxHP() / 2)
		{
			return L"Мне бы не помешало бы лечение.";
		}
		break;
	default:
		break;
	} 
	return L"";
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

		name.setString(L"Игрок");
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
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && state == IDLE)
	{
		close();
	}
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && state == IDLE)
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
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && state == IDLE)
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
	str = L"Уровень " + std::to_wstring(GameData::instance().getPlayer().getLevel());
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
	level.setString(str);
	float p = ((float)GameData::instance().getPlayer().getXP() / (float)GameData::instance().getPlayer().getNextLevelXP()) * 100.0;
	xpbar.setTextureRect(sf::IntRect(0,0, (737 / 100) * p, 67));
}