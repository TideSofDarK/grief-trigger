#include "g_monsters.h"

#include <math.h>

#include "g_scenemanager.h"

Monster::Monster(std::string name_)
{
	name = name_;
	setStats(Parser::instance().getMonsterStats(name));
	std::cout << std::to_string(getStrength()) + "x" + std::to_string(getAgility()) + "x" + std::to_string(getIntelligence()) + "x" + std::to_string(getHP()) << std::endl;
	state = ALIVE;
}

Squad::Squad()
{

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

	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 4; i++)
		{
			walkingAnimation[a].setSpriteSheet(TextureManager::instance().getTexture("assets/player.png"));
			walkingAnimation[a].addFrame(sf::IntRect(i * 32, a * 32, 32, 32));
		}
	}

	animatedSprite = AnimatedSprite(sf::seconds(0.05));
	animatedSprite.setAnimation(walkingAnimation[DIR_TOP]);
	animatedSprite.setPosition(pos);
	animatedSprite.stop();
	animatedSprite.setLooped(true);

	direction = DIR_TOP;
	walking = false;

	nx = pos.x;
	ny = pos.y;

	counter = 0;
	maxCounter = 50 + (rand() * (int)(60 - 30) / RAND_MAX);

	bounds = sf::FloatRect(animatedSprite.getPosition().x - (CHARACTER_SIZE * 1), animatedSprite.getPosition().y + (CHARACTER_SIZE * 1), CHARACTER_SIZE * 2, CHARACTER_SIZE * 2);
}

void Squad::draw(sf::RenderTarget &tg)
{
	tg.draw(animatedSprite);
}

void Squad::update(sf::Time time, std::vector<tmx::MapObject> &objects)
{
	bool isMovable = true;

	//Set player pointer
	for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		tmx::MapObject &object = *it;
		if (object.GetName() == "hero")
		{
			hero = &object;
		}
	}

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

		if(direction != DIR_IDLE)
		{
			step(direction, objects);
		}
	}

	if ((rand() % (int)(100)) == 50 && !walking)
	{
		if (hero->GetPosition().x == animatedSprite.getPosition().x + CHARACTER_SIZE && hero->GetPosition().y == animatedSprite.getPosition().y)
		{
			SceneManager::instance().initBattle(*this);
		}
		else if (hero->GetPosition().x == animatedSprite.getPosition().x - CHARACTER_SIZE && hero->GetPosition().y == animatedSprite.getPosition().y)
		{
			SceneManager::instance().initBattle(*this);
		}
		else if (hero->GetPosition().x == animatedSprite.getPosition().x && hero->GetPosition().y == animatedSprite.getPosition().y + CHARACTER_SIZE)
		{
			SceneManager::instance().initBattle(*this);
		}
		else if (hero->GetPosition().x == animatedSprite.getPosition().x && hero->GetPosition().y == animatedSprite.getPosition().y - CHARACTER_SIZE)
		{
			SceneManager::instance().initBattle(*this);	
		}
	}
	
	if (walking)
	{
		if(direction == DIR_TOP)
		{
			ny -= playerMoveSpeed;

			if(ny <= nextspot)
			{
				ny = nextspot; 
				walking = false;
				direction = DIR_IDLE;

				animatedSprite.stop();
			}
		}
		if(direction == DIR_DOWN)
		{
			ny += playerMoveSpeed;

			if(ny >= nextspot)
			{
				ny = nextspot;
				walking = false;
				direction = DIR_IDLE;

				animatedSprite.stop();
			}
		}
		if(direction == DIR_LEFT)
		{
			nx -= playerMoveSpeed;

			if(nx <= nextspot)
			{
				nx = nextspot;
				walking = false;
				direction = DIR_IDLE;

				animatedSprite.stop();
			}
		}
		if(direction == DIR_RIGHT)
		{
			nx += playerMoveSpeed;

			if(nx >= nextspot)
			{
				nx = nextspot;
				walking = false;
				direction = DIR_IDLE;

				animatedSprite.stop();
			}
		}
	}

	animatedSprite.setPosition(nx, ny);
	animatedSprite.update(time);
	object->SetPosition(animatedSprite.getPosition());
}

void Squad::step(int dir, std::vector<tmx::MapObject> &objects)
{
	if (!walking)
	{
		bool isMovable = true;
		sf::Vector2f movement;

		switch (dir)
		{
		case DIR_TOP:
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

		if (bounds.contains(sf::Vector2f(animatedSprite.getPosition().x + movement.x, animatedSprite.getPosition().y + movement.y))) isMovable = true;
		else isMovable = false;

		for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			tmx::MapObject &object = *it;
			if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x + movement.x, animatedSprite.getPosition().y + movement.y))) isMovable = false;
		}

		if(isMovable == true)
		{
			if (dir == DIR_DOWN || dir == DIR_TOP) nextspot = animatedSprite.getPosition().y + movement.y; 
			else if (dir == DIR_LEFT || dir == DIR_RIGHT) nextspot = animatedSprite.getPosition().x + movement.x; 
			direction = dir;
			walking = true;

			animatedSprite.setAnimation(walkingAnimation[direction]);
			animatedSprite.play();
		}	
		else
		{
			animatedSprite.setAnimation(walkingAnimation[dir]);
			animatedSprite.setFrame(0);
		}
	}
}