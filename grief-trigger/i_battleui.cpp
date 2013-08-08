#include "i_battleui.h"

#include <SFML/Window.hpp>

#include "g_scenemanager.h"

sf::Vector2f interpolate(
	const sf::Vector2f& pointA,
	const sf::Vector2f& pointB,
	float factor
	) {
		if( factor > 1.f ) 
			factor = 1.f;

		else if( factor < 0.f )
			factor = 0.f;

		return pointA + (pointB - pointA) * factor;
}

float interpolateLinear(
	const float pointA,
	const float pointB,
	float factor
	) {
		if( factor > 1.f ) 
			factor = 1.f;

		else if( factor < 0.f )
			factor = 0.f;

		return pointA + (pointB - pointA) * factor;
}

Logger::Logger()
{

}

void Logger::init(sf::Vector2f pos)
{
	text = sf::Text();
	text.setPosition(pos);
	text.setString("");
	text.setFont(DFont::instance().getFont());
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
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
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

Damage::Damage(sf::Vector2f pos, std::string str)
{
	text = sf::Text(str, DFont::instance().getFont(), 80);
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
	items.clear();
	for (auto i = newItems.begin(); i != newItems.end(); i++)
	{
		sf::String s(*i);

		sf::Text text(s, DFont::instance().getFont(), 20);
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
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			select();
			disappear();
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
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
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
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

void SpellMenu::setSpells(std::vector<Spell> newSpells)
{
	const unsigned int startX = WIDTH / 3;
	const unsigned int startY = HEIGHT / 5;

	spells.clear();
	for (auto i = newSpells.begin(); i != newSpells.end(); i++)
	{
		Spell &spell = *i;
		Item item(spell);
		item.setPosition(sf::Vector2f(startX, 15 + startY + ((i - newSpells.begin()) * CHARACTER_SIZE * 3)));

		spells.push_back(item);
	}

	verticalPointer.setPosition(startX + (CHARACTER_SIZE / 2), 0);
	horizontalPointer.setPosition(sf::Vector2f(0, spells[selection].getPosition().y));

	verticalPointer.setFillColor(YELLOW);
	horizontalPointer.setFillColor(YELLOW);

	selected = -1;
	selection = 0;

	state = WORKING;
}

void SpellMenu::draw(sf::RenderTarget &tg)
{
	if (state != IDLE)
	{
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

		for (auto i = spells.begin(); i != spells.end(); i++)
		{
			i->draw(tg);
		}
	}	
}

void SpellMenu::input(sf::Event &event)
{
	if (state == WORKING)
	{
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
		{
			if (selection > 0)
			{
				selection--;
			}
			else
			{
				selection = spells.size() - 1;
			}

			SoundManager::instance().playSelectSound();
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
		{
			if (selection + 1 < spells.size())
			{
				selection++;
			}
			else
			{
				selection = 0;
			}

			SoundManager::instance().playSelectSound();
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			select();
			close();

			SoundManager::instance().playEnterSound();
		}
	}
}

void SpellMenu::update(sf::Time time)
{
	if (state != IDLE)
	{
		horizontalPointer.setPosition(sf::Vector2f(0, spells[selection].getPosition().y + (CHARACTER_SIZE / 2)));

		if(state == WORKING)
		{
			for (auto i = spells.begin(); i != spells.end(); i++)
			{
				i->update(i - spells.begin() == selection ? true : false);
				if (i - spells.begin() == selection)
				{
					i->setPosition(sf::Vector2f(WIDTH / 3 + 10, i->getPosition().y));
				}
				else
				{
					i->setPosition(sf::Vector2f(WIDTH / 3, i->getPosition().y));
				}
			}
		}
		if (state == TRANSITION)
		{
			//spells[selected].setPosition(sf::Vector2f(tempSelectedX, spells[selected].getPosition().y));
			//oTweener.step(time.asSeconds());
			//if (tempY == HEIGHT || tempSelectedX == WIDTH + CHARACTER_SIZE) state = IDLE;
		}
	}	
}

void SpellMenu::select()
{
	state = IDLE;
	selected = selection;
}

SpellQTE::SpellQTE()
{
	state = IDLE;
	
	line.setPosition(0, startY );
	
	cell.setPosition((WIDTH / 2) - CHARACTER_SIZE * 2 - 12, startY + CHARACTER_SIZE - 14); //Some position fixes
	shape.setSize(sf::Vector2f(30,10));
	shape.setPosition(cell.getPosition().x + 10, cell.getPosition().y + 14);
	shape.setFillColor(sf::Color::Green);
}

void SpellQTE::draw(sf::RenderTarget &tg)
{
	if (state == PREPARE)
	{
		tg.draw(prepare[hero]);

		if ((int)(rand() % 50) == 3) tg.clear(sf::Color::White);
	}
	else
	{
		tg.draw(line);

		cell.move(77,0);
		cell.setTextureRect(sf::IntRect(81,0,155 - 81,156));
		tg.draw(cell);

		for (auto i = blocks.begin(); i != blocks.end(); i++)
		{
			if (i->state != SLEEP)
			{
				i->draw(tg);
			}	
		}

		cell.move(-77,0);
		cell.setTextureRect(sf::IntRect(0,0,80,156));
		tg.draw(cell);

		for (auto i = etexts.begin(); i != etexts.end(); i++)
		{
			i->draw(tg);
		}
	}

	/*sf::Vector2f p = shape.getPosition();
	shape.setPosition(blocks.front().sprite.getPosition());
	tg.draw(shape);
	shape.setPosition(p);
	tg.draw(shape);*/
}

void SpellQTE::update(sf::Time time)
{
	if (state == PREPARE)
	{
		if ((int)(rand() % 50) == 3) SceneManager::instance().getScene().setCurrentEffect("rgb", sf::seconds(0.1));
		prepare[hero].setPosition(sf::Vector2f(interpolateLinear(prepare[hero].getPosition().x, -120, 0.1f), prepare[hero].getPosition().y));
	}
	if (state == PREPARE && timer.getElapsedTime().asSeconds() > 1.0)
	{
		state = WORKING;
		timer.restart();
	}
	for (auto i = etexts.begin(); i != etexts.end(); i++)
	{
		i->update(time);
	}
	if (type == 0)
	{
		if (state == TRANSITION)
		{
			line.setColor(sf::Color(255,255,255,interpolateLinear(line.getColor().a, 0, 0.05)));
			cell.setColor(sf::Color(255,255,255,interpolateLinear(cell.getColor().a, 0, 0.05)));
			blocks.back().sprite.setColor(sf::Color(255,255,255,interpolateLinear(blocks.back().sprite.getColor().a, 0, 0.05)));
			if (blocks.back().sprite.getColor().a == 0 && !etexts.front().isWorking())
			{
				state = IDLE;
			}
		}
		if (state == WORKING || state == TRANSITION)
		{
			for (auto i = blocks.begin(); i != blocks.end();)
			{
				Block &block = *i;
				if (i->state == GOING)
				{
					//If player skipped it
					if (waiting && timer.getElapsedTime().asSeconds() > 0.7)
					{
						waiting = false;
						timer.restart();
						block.state = ENDED;
						next(i);
						break;
					}
					//Start timer
					if (block.sprite.getPosition().x <= ((WIDTH / 2) - CHARACTER_SIZE * 2) + 10 && !waiting)
					{
						waiting = true;
						timer.restart();
					}
					else
					{
						//Else move block
						block.sprite.setPosition(interpolate(block.sprite.getPosition(), sf::Vector2f((WIDTH / 2) - CHARACTER_SIZE * 2, block.sprite.getPosition().y), 0.15));
					}
					break;
				}
				else if (i->state == ENDED)
				{
					//Move and destroy
					block.sprite.setPosition(interpolate(block.sprite.getPosition(), sf::Vector2f(-128, block.sprite.getPosition().y), 0.2));
					if (block.sprite.getPosition().x + 128 == 0)
					{
						block.state = SLEEP;

						i = blocks.erase(i);
					}
					else
					{
						i++;
					}
				}
			}		
		}
	}
	else
	{
		if (state == TRANSITION)
		{
			line.setColor(sf::Color(255,255,255,interpolateLinear(line.getColor().a, 0, 0.05)));
			cell.setColor(sf::Color(255,255,255,interpolateLinear(cell.getColor().a, 0, 0.05)));
			blocks.back().sprite.setColor(sf::Color(255,255,255,interpolateLinear(blocks.back().sprite.getColor().a, 0, 0.05)));
			if (blocks.back().sprite.getColor().a == 0)
			{
				state = IDLE;
			}
		}
		if (state == WORKING || state == TRANSITION)
		{
			for (auto i = blocks.begin(); i != blocks.end(); i++)
			{
				Block &block = *i;
				if (block.state == GOING || block.state == NEXT)
				{
					block.sprite.move(-QTE_FACTOR,0);
				}

				if (i->sprite.getPosition().x < shape.getPosition().x - 40 && i->state != ENDED)
				{
					etexts.push_back(EText("bad"));
					SoundManager::instance().playFailSound();
					i->state = ENDED;
					next(i);
				}

				if (block.state == ENDED)
				{
					//Move and destroy
					block.sprite.setPosition(interpolate(block.sprite.getPosition(), sf::Vector2f(-128, block.sprite.getPosition().y), 0.05));
					block.sprite.setColor(sf::Color(255,255,255,interpolateLinear(block.sprite.getColor().a, 0, 0.1)));
				}
			}
		}
	}
}

void SpellQTE::next(std::deque<Block>::iterator &i)
{
	if (i - blocks.begin() + 1 < blocks.size())
	{
		i++;
		i->state = GOING;
	}		
	else
	{
		state = TRANSITION;
	}
}

void SpellQTE::input(sf::Event &event)
{
	if (state == WORKING)
	{
		if (type == 0)
		{
			if (state == WORKING)
			{
				if(event.type == sf::Event::KeyPressed)
				{
					for (auto i = blocks.begin(); i != blocks.end(); i++)
					{
						if (i->state != ENDED)
						{
							Block &block = *i;
							if (shape.getGlobalBounds().intersects(block.sprite.getGlobalBounds()))
							{
								if ( event.key.code == i->b)
								{
									double p;
									if (shape.getPosition().x < block.sprite.getPosition().x)
									{
										p = (shape.getPosition().x / block.sprite.getPosition().x) * 100;
									}
									else
									{
										p = (block.sprite.getPosition().x / shape.getPosition().x) * 100;
									}

									std::cout << std::to_string(p)+"\n";
									//std::cout << std::to_string(shape.getPosition().x)+"x" + std::to_string(block.sprite.getPosition().x) + "\n";
									if (p >= 0 && p <= 97)
									{
										etexts.push_back(EText("safe"));
									}
									else if (p >= 97.5 && p < 98)
									{
										etexts.push_back(EText("cool"));
									}
									else if (p >= 98)
									{								
										if (rand() % 2)
										{
											etexts.push_back(EText("awesome"));
										}		
										else
										{
											etexts.push_back(EText("cool"));
										}
										if ((int)(rand() % 50) == 3)
										{
											SceneManager::instance().getScene().setCurrentEffect("inversion", sf::seconds(1));
										}
									}
									else
									{
										etexts.push_back(EText("safe"));
									}
									i->state = ENDED;
									waiting = false;
									timer.restart();
									next(i);
									damaged = true;
									//std::cout << "asdfsdfs\n";
									break;
								}	
							}
							else break;
						}
					}		
				}
			}
		}
		else
		{
			if (state == WORKING)
			{
				for (auto i = blocks.begin(); i != blocks.end(); i++)
				{
					Block &block = *i;
					if (i->state == GOING)
					{
						if (shape.getGlobalBounds().intersects(block.sprite.getGlobalBounds()))
						{
							if(event.type == sf::Event::KeyPressed)
							{
								if ( event.key.code == block.b)
								{
									double p;
									if (shape.getPosition().x < block.sprite.getPosition().x)
									{
										p = (shape.getPosition().x / block.sprite.getPosition().x) * 100;
									}
									else
									{
										p = (block.sprite.getPosition().x / shape.getPosition().x) * 100;
									}

									std::cout << std::to_string(p)+"\n";
									//std::cout << std::to_string(shape.getPosition().x)+"x" + std::to_string(block.sprite.getPosition().x) + "\n";
									if (p >= 0 && p <= 96)
									{
										etexts.push_back(EText("safe"));
									}
									else if (p >= 97 && p < 98.09)
									{
										etexts.push_back(EText("cool"));
									}
									else if (p >= 98.1)
									{								
										etexts.push_back(EText("awesome"));

										if ((int)(rand() % 50) == 3)
										{
											SceneManager::instance().getScene().setCurrentEffect("inversion", sf::seconds(1));
										}
									}	
									else
									{
										etexts.push_back(EText("cool"));
									}
									i->state = ENDED;
									next(i);
									damaged = true;
									break;
								}	
							}
						}
					}			
				}		
			}
		}
	}
}

void SpellQTE::clean()
{
	damaged = false;
}

void SpellQTE::loadResources()
{
	prepare[0].setTexture(TextureManager::instance().getTexture("assets/prepare_player.png"));
	prepare[1].setTexture(TextureManager::instance().getTexture("assets/prepare_red.png"));
	prepare[2].setTexture(TextureManager::instance().getTexture("assets/prepare_blue.png"));
	line.setTexture(TextureManager::instance().getTexture("assets/line.png"));
	cell.setTexture(TextureManager::instance().getTexture("assets/circle.png"));
}

void SpellQTE::start(Spell &sp,  std::string name)
{
	if (name == "player")
	{
		hero = 0;
	}
	if (name == "red")
	{
		hero = 1;
	}
	if (name == "blue")
	{
		hero = 2;
	}

	spell = sp;

	std::vector<int> &v = spell.getCombo();

	line.setColor(sf::Color(255,255,255,255));
	cell.setColor(sf::Color(255,255,255,255));
	blocks.clear();
	etexts.clear();

	for (auto i = v.begin(); i != v.end(); i++)
	{
		Block block;
		std::string name;
		switch (*i)
		{
		case UP:
			name = "up";
			break;
		case RIGHT:
			name = "right";
			break;
		case LEFT:
			name = "left";
			break;
		case DOWN:
			name = "down";
			break;
		case _Z:
			name = "Z";
			break;
		case _X:
			name = "X";
			break;
		default:
			break;
		}
		block.sprite.setTexture(TextureManager::instance().getTexture("assets/" + name + ".png"));
		block.sprite.setPosition(WIDTH + ((i - v.begin()) * (CHARACTER_SIZE * 5)) + ((i - v.begin()) * 8),startY + CHARACTER_SIZE);
		block.b = *i;

		if (type == 0)
		{
			block.state = SLEEP;
		}
		else
		{
			block.state = NEXT;
			if (rand() % 2)
			{
				//block.sprite.move(((1) * (CHARACTER_SIZE * 5)) + OFFSET,0);
			}
		}

		blocks.push_back(block);
	}

	blocks.front().state = GOING;

	SceneManager::instance().getScene().setCurrentEffect("rgb", sf::seconds(0.05));

	clean();
	state = PREPARE;
	timer.restart();
	waiting = false;
}

HitEffect::HitEffect()
{
	sprite = AnimatedSprite(sf::seconds(0.1f));

	animation.setSpriteSheet(TextureManager::instance().getTexture("assets/hit.png"));

	for (int a = 0; a < 7; a++)
	{
		animation.addFrame(sf::IntRect(0, a * 193, 784, 193));
	}
}

void HitEffect::init(sf::Vector2f pos)
{
	sprite.setAnimation(animation);
	sprite.setLooped(false);
	sprite.play();

	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	sprite.setRotation((rand() % (int)(361)));
	sprite.setPosition(pos.x + sprite.getLocalBounds().width / 2, pos.y + sprite.getLocalBounds().height / 2);

	working = true;
}

void HitEffect::draw(sf::RenderTarget &tg)
{
	if (working)
	{
		tg.draw(sprite);
	}
}

void HitEffect::update(sf::Time time)
{
	sprite.update(sf::seconds(1.f / 15.f));
	if (!sprite.isPlaying())
	{
		working = false;
	}
}