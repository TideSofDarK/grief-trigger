#include "g_monsters.h"

#include <math.h>

#include "g_scenemanager.h"

Monster::Monster(std::string name_)
{
	name = name_;
	setStats(Parser::instance().getMonsterStats(name));
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

	sprite.setTexture(TextureManager::instance().getTexture("assets/player.png"));
	sprite.setPosition(pos);

	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 4; i++)
		{
			animations[a].addFrame(1.f, sf::IntRect(i * CHARACTER_SIZE, a * CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE));
		}
	}

	animator.addAnimation("up", animations[DIR_UP], sf::seconds(0.9f));
	animator.addAnimation("right", animations[DIR_RIGHT], sf::seconds(0.9f));
	animator.addAnimation("left", animations[DIR_LEFT], sf::seconds(0.9f));
	animator.addAnimation("down", animations[DIR_DOWN], sf::seconds(0.9f));

	animator.playAnimation("down");

	nx = sprite.getPosition().x;
	ny = sprite.getPosition().y;

	direction = DIR_UP;
	walking = false;

	nx = pos.x;
	ny = pos.y;

	counter = 0;
	maxCounter = 50 + (rand() * (int)(60 - 30) / RAND_MAX);

	bounds = sf::FloatRect(sprite.getPosition().x - (CHARACTER_SIZE * 1), sprite.getPosition().y + (CHARACTER_SIZE * 1), CHARACTER_SIZE * 2, CHARACTER_SIZE * 2);
}

void Squad::draw(sf::RenderTarget &tg)
{
	sprite.move(0,-11);
	tg.draw(sprite);
	sprite.move(0,11);
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

		if(direction != DIR_IDLE)
		{
			step(direction);
		}
	}

	if ((rand() % (int)(100)) == 50 && !walking)
	{
		/*if (SceneManager::instance().getScene().getPlayerObject().getOnMap().GetPosition().x == sprite.getPosition().x + CHARACTER_SIZE && SceneManager::instance().getScene().getPlayerObject().getOnMap().GetPosition().y == sprite.getPosition().y)
		{
			SceneManager::instance().initBattle(*this);
		}
		else if (SceneManager::instance().getScene().getPlayerObject().getOnMap().GetPosition().x == sprite.getPosition().x - CHARACTER_SIZE && SceneManager::instance().getScene().getPlayerObject().getOnMap().GetPosition().y == sprite.getPosition().y)
		{
			SceneManager::instance().initBattle(*this);
		}
		else if (SceneManager::instance().getScene().getPlayerObject().getOnMap().GetPosition().x == sprite.getPosition().x && SceneManager::instance().getScene().getPlayerObject().getOnMap().GetPosition().y == sprite.getPosition().y + CHARACTER_SIZE)
		{
			SceneManager::instance().initBattle(*this);
		}
		else if (SceneManager::instance().getScene().getPlayerObject().getOnMap().GetPosition().x == sprite.getPosition().x && SceneManager::instance().getScene().getPlayerObject().getOnMap().GetPosition().y == sprite.getPosition().y - CHARACTER_SIZE)
		{
			SceneManager::instance().initBattle(*this);	
		}*/
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

				animator.stopAnimation();
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

				animator.stopAnimation();
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

				animator.stopAnimation();
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

				animator.stopAnimation();
			}
		}
	}

	animator.update(time);
	animator.animate(sprite);
	object->SetPosition(sf::Vector2f(nx, ny));
	sprite.setPosition(nx, ny);
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
				animator.playAnimation("up", false);			
				break;
			case DIR_DOWN:
				animator.playAnimation("down", false);
				break;
			case DIR_LEFT:
				animator.playAnimation("left", false);
				break;
			case DIR_RIGHT:
				animator.playAnimation("right", false);
				break;
			default:
				break;
			}
		}
	}
}