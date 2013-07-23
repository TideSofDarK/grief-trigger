#include "g_monsters.h"

Monster::Monster(std::string name)
{
	setStats(Parser::instance().getMonsterStats(name));
}

Squad::Squad()
{

}

void Squad::init(std::string name, sf::Vector2f pos)
{
	//monsters = squad;

	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 4; i++)
		{
			walkingAnimation[a].setSpriteSheet(ResourcesManager::instance().getTexture("assets/player.png"));
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

	bounds = sf::FloatRect(animatedSprite.getPosition().x - (CHARACTER_SIZE * 1), animatedSprite.getPosition().y + (CHARACTER_SIZE * 1), CHARACTER_SIZE * 2, CHARACTER_SIZE * 2);
}

void Squad::draw(sf::RenderTarget &tg)
{
	tg.draw(animatedSprite);
}

void Squad::update(sf::Time time, std::vector<tmx::MapObject> &objects)
{
	if (!walking)
	{
		bool isMovable = true;
		
		if (counter <= 60)
		{
			counter++;
		}
		else
		{
			direction = -1 + (rand() % (int)(5));

			counter = 0;
		}

		if(direction == DIR_TOP)
		{
			for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				tmx::MapObject &object = *it;
				if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x, animatedSprite.getPosition().y - CHARACTER_SIZE))) isMovable = false;
			}

			if (bounds.contains(sf::Vector2f(animatedSprite.getPosition().x, animatedSprite.getPosition().y - CHARACTER_SIZE))) isMovable = true;
			else isMovable = false;

			if(isMovable == true)
			{
				nextspot = animatedSprite.getPosition().y - CHARACTER_SIZE; 
				walking = true;

				animatedSprite.setAnimation(walkingAnimation[direction]);
				animatedSprite.play();
			}
			else if (isMovable == false)
			{
				animatedSprite.setAnimation(walkingAnimation[DIR_TOP]);
				animatedSprite.setFrame(0);
			}		
		}

		if(direction == DIR_DOWN)
		{
			for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				tmx::MapObject &object = *it;
				if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x, animatedSprite.getPosition().y + CHARACTER_SIZE))) isMovable = false;
			}

			if (bounds.contains(sf::Vector2f(animatedSprite.getPosition().x, animatedSprite.getPosition().y + CHARACTER_SIZE))) isMovable = true;
			else isMovable = false;

			if(walking == false && isMovable == true)
			{
				nextspot = animatedSprite.getPosition().y + CHARACTER_SIZE; 
				walking = true;

				animatedSprite.setAnimation(walkingAnimation[direction]);
				animatedSprite.play();
			}
			else if (isMovable == false && walking == false)
			{
				animatedSprite.setAnimation(walkingAnimation[DIR_DOWN]);
				animatedSprite.setFrame(0);
			}
		}

		if(direction == DIR_LEFT)
		{
			for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				tmx::MapObject &object = *it;
				if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x - CHARACTER_SIZE, animatedSprite.getPosition().y))) isMovable = false;
			}

			if (bounds.contains(sf::Vector2f(animatedSprite.getPosition().x - CHARACTER_SIZE, animatedSprite.getPosition().y))) isMovable = true;
			else isMovable = false;

			if(walking == false && isMovable == true)
			{
				nextspot = animatedSprite.getPosition().x - CHARACTER_SIZE; 
				walking = true;

				animatedSprite.setAnimation(walkingAnimation[direction]);
				animatedSprite.play();
			}
			else if (isMovable == false && walking == false)
			{
				animatedSprite.setAnimation(walkingAnimation[DIR_LEFT]);
				animatedSprite.setFrame(0);
			}	
		}

		if(direction == DIR_RIGHT)
		{
			for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				tmx::MapObject &object = *it;
				if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x + CHARACTER_SIZE, animatedSprite.getPosition().y))) isMovable = false;
			}

			if (bounds.contains(sf::Vector2f(animatedSprite.getPosition().x + CHARACTER_SIZE, animatedSprite.getPosition().y))) isMovable = true;
			else isMovable = false;

			if(walking == false && isMovable == true)
			{
				nextspot = animatedSprite.getPosition().x + CHARACTER_SIZE; 
				walking = true;

				animatedSprite.setAnimation(walkingAnimation[direction]);
				animatedSprite.play();
			}
			else if (isMovable == false && walking == false)
			{
				animatedSprite.setAnimation(walkingAnimation[DIR_RIGHT]);
				animatedSprite.setFrame(0);
			}	
		}
	}
	else
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
}