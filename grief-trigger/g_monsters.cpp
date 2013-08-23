#include "g_monsters.h"

#include <math.h>

#include "g_scenemanager.h"

Monster::Monster(std::string name_)
{
	name = name_;
	setStats(Parser::instance().getMonsterStats(name));
	maxHP = getHP();
	state = ALIVE;
}

Squad::Squad()
{
	animationA = new Animation();
	spriteA = new AnimatedSprite();

	animationA->setSpriteSheet(TextureManager::instance().getTexture("assets/attention.png"));

	for (int a = 0; a < 3; a++)
	{
		animationA->addFrame(sf::IntRect(a * 32, 0, 32, 32));
	}

	for (int a = 2; a > 0; a--)
	{
		animationA->addFrame(sf::IntRect(a * 32, 0, 32, 32));
	}

	spriteA->setAnimation(*animationA);
	spriteA->setLooped(true);
	spriteA->play();
}

void Squad::init(std::string name, sf::Vector2f pos, tmx::MapObject& onMap)
{
	object = &onMap;

	//Parse squad monsters
	std::vector<std::string> parsedSquad = Parser::instance().parseSquad(name);
	for (auto i = parsedSquad.begin(); i != parsedSquad.end(); ++i)
	{
		std::string str = *i;
		Monster newMonster(str);
		monsters.push_back(newMonster);
	}

	if (Level::instance().getDay() >= 3)
	{
		sprite.setTexture(TextureManager::instance().getTexture("assets/slug2.png"));
	}
	else
	{
		sprite.setTexture(TextureManager::instance().getTexture("assets/slug1.png"));
	}
	
	sprite.setPosition(pos);

	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 4; i++)
		{
			animations[a].addFrame(1.f, sf::IntRect(i * CHARACTER_SIZE, a * CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE));
		}
	}

	animator.addAnimation("up", animations[DIR_UP], sf::seconds(1.3f));
	animator.addAnimation("right", animations[DIR_RIGHT], sf::seconds(1.3f));
	animator.addAnimation("left", animations[DIR_LEFT], sf::seconds(1.3f));
	animator.addAnimation("down", animations[DIR_DOWN], sf::seconds(1.3f));

	animator.playAnimation("down", true);

	nx = sprite.getPosition().x;
	ny = sprite.getPosition().y;

	direction = DIR_UP;
	walking = false;

	counter = 0;
	maxCounter = 50 + (rand() * (int)(60 - 30) / RAND_MAX);

	bounds = sf::FloatRect(sprite.getPosition().x - (CHARACTER_SIZE * 1), sprite.getPosition().y + (CHARACTER_SIZE * 1), CHARACTER_SIZE * 2, CHARACTER_SIZE * 2);

	aggr = false;
}

void Squad::draw(sf::RenderTarget &tg)
{
	sprite.move(0,-11);
	tg.draw(sprite);
	sprite.move(0,11);
	spriteA->setPosition(sprite.getPosition());
	spriteA->move(-2,-16);
	if (aggr) tg.draw(*spriteA);
}

void Squad::update(sf::Time time)
{
	bool isMovable = true;

	if (!walking)
	{
		if (counter <= maxCounter)
		{
			counter++;
		}
		else
		{
			direction = -1 + (rand() % (int)(5));

			counter = 0;
			maxCounter = 50 + (rand() * (int)(60 - 30) / RAND_MAX);
		}

		if(direction != DIR_IDLE && !aggr)
		{
			step(direction);
		}
	}

	if (!walking && !aggr)
	{
		if (sf::FloatRect(sprite.getPosition().x - (CHARACTER_SIZE*2), sprite.getPosition().y - (CHARACTER_SIZE*2), CHARACTER_SIZE * 4, CHARACTER_SIZE * 4).intersects(SceneManager::instance().getScene().getPlayerBoundOnMap()))
		{
			aggr = true;
			aggroTimer.restart();
		}
	}
	if (aggr && aggroTimer.getElapsedTime() > sf::seconds(1.3))
	{	
		if ((animator.isPlayingAnimation() && animator.getPlayingAnimation() != "left") || !animator.isPlayingAnimation())
		{
			animator.playAnimation("left", true);
		}
		if (sprite.getGlobalBounds().intersects(SceneManager::instance().getScene().getPlayerBoundOnMap()))
		{
			SceneManager::instance().initBattle(*this, false);	
		}
		else
		{
			nx = interpolateLinear(sprite.getPosition().x, SceneManager::instance().getScene().getPlayerPositionOnMap().x, 0.03f);
			ny = interpolateLinear(sprite.getPosition().y, SceneManager::instance().getScene().getPlayerPositionOnMap().y, 0.03f);
		}
	}

	if (walking)
	{
		if(direction == DIR_UP)
		{
			ny -= playerMoveSpeed;

			if(ny <= nextspot)
			{
				ny = nextspot; 
				walking = false;
				direction = DIR_IDLE;

				if (Level::instance().getDay() != 1) animator.stopAnimation();
			}
		}
		else if(direction == DIR_DOWN)
		{
			ny += playerMoveSpeed;

			if(ny >= nextspot)
			{
				ny = nextspot;
				walking = false;
				direction = DIR_IDLE;

				if (Level::instance().getDay() != 1) animator.stopAnimation();
			}
		}
		else if(direction == DIR_LEFT)
		{
			nx -= playerMoveSpeed;

			if(nx <= nextspot)
			{
				nx = nextspot;
				walking = false;
				direction = DIR_IDLE;

				if (Level::instance().getDay() != 1) animator.stopAnimation();
			}
		}
		else if(direction == DIR_RIGHT)
		{
			nx += playerMoveSpeed;

			if(nx >= nextspot)
			{
				nx = nextspot;
				walking = false;
				direction = DIR_IDLE;

				if (Level::instance().getDay() != 1) animator.stopAnimation();
			}
		}
	}

	animator.update(time);
	animator.animate(sprite);
	object->SetPosition(sf::Vector2f(nx, ny));
	sprite.setPosition(nx, ny);

	spriteA->update(time);
	spriteA->setPosition(sf::Vector2f(nx, ny));
}

void Squad::step(int dir)
{
	if (!walking)
	{
		bool isMovable = true;
		sf::Vector2f movement;

		switch (dir)
		{
		case DIR_UP:
			movement.y -= CHARACTER_SIZE;
			break;
		case DIR_DOWN:
			movement.y += CHARACTER_SIZE;
			break;
		case DIR_RIGHT:
			movement.x += CHARACTER_SIZE;
			break;
		case DIR_LEFT:
			movement.x -= CHARACTER_SIZE;
			break;
		default:
			break;
		}

		if (bounds.contains(sf::Vector2f(sprite.getPosition().x + movement.x, sprite.getPosition().y + movement.y))) isMovable = true;
		else isMovable = false;

		for (std::vector<tmx::MapObject>::iterator it = SceneManager::instance().getScene().getObjects().begin(); it != SceneManager::instance().getScene().getObjects().end(); ++it)
		{
			tmx::MapObject &object = *it;
			if (object.GetAABB().intersects(sf::FloatRect(sprite.getPosition().x + movement.x, sprite.getPosition().y + movement.y, CHARACTER_SIZE, CHARACTER_SIZE)))
			{
				isMovable = false;
				break;
			}
		}

		if(isMovable == true)
		{
			if (dir == DIR_DOWN || dir == DIR_UP) nextspot = sprite.getPosition().y + movement.y; 
			else if (dir == DIR_LEFT || dir == DIR_RIGHT) nextspot = sprite.getPosition().x + movement.x; 
			direction = dir;
			walking = true;

			switch (direction)
			{
			case DIR_UP:
				animator.playAnimation("up", true);
				break;
			case DIR_DOWN:
				animator.playAnimation("down", true);
				break;
			case DIR_LEFT:
				animator.playAnimation("left", true);
				break;
			case DIR_RIGHT:
				animator.playAnimation("right", true);
				break;
			default:
				break;
			}
		}	
		else
		{
			switch (dir)
			{
			case DIR_UP:
				animator.playAnimation("up", Level::instance().getDay() != 1 ? true : false);			
				break;
			case DIR_DOWN:
				animator.playAnimation("down", Level::instance().getDay() != 1 ? true : false);
				break;
			case DIR_LEFT:
				animator.playAnimation("left", Level::instance().getDay() != 1 ? true : false);
				break;
			case DIR_RIGHT:
				animator.playAnimation("right", Level::instance().getDay() != 1 ? true : false);
				break;
			default:
				break;
			}
		}
	}
}