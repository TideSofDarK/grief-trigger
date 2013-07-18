#include "g_playerobject.h"

#include "h_config.h"

PlayerObject::PlayerObject(sf::Uint32 x, sf::Uint32 y)
{
	texture.loadFromFile("assets/player.png");

	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 4; i++)
		{
			walkingAnimation[a].setSpriteSheet(texture);
			walkingAnimation[a].addFrame(sf::IntRect(i * 32, a * 32, 32, 32));
		}
	}

	animatedSprite = AnimatedSprite(sf::seconds(0.05));
	animatedSprite.setAnimation(walkingAnimation[DIR_TOP]);
	animatedSprite.setPosition(x, y);
	animatedSprite.stop();
	animatedSprite.setLooped(true);

	direction = DIR_TOP;
	walking = false;
	nx = animatedSprite.getPosition().x;
	ny = animatedSprite.getPosition().y;
}

void PlayerObject::move(std::vector<tmx::MapObject> &objects)
{
	bool isMovable = true;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			tmx::MapObject &object = *it;
			if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x, animatedSprite.getPosition().y - CHARACTER_SIZE))) isMovable = false;
		}

		if(walking == false && isMovable == true)
		{
			nextspot = animatedSprite.getPosition().y - CHARACTER_SIZE; 
			direction = DIR_TOP;
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

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			tmx::MapObject &object = *it;
			if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x, animatedSprite.getPosition().y + CHARACTER_SIZE))) isMovable = false;
		}

		if(walking == false && isMovable == true)
		{
			nextspot = animatedSprite.getPosition().y + CHARACTER_SIZE; 
			direction = DIR_DOWN;
			walking = true;

			animatedSprite.setAnimation(walkingAnimation[direction]);
			animatedSprite.play();
		}
		else if (isMovable == false)
		{
			animatedSprite.setAnimation(walkingAnimation[DIR_DOWN]);
			animatedSprite.setFrame(0);
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			tmx::MapObject &object = *it;
			if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x - CHARACTER_SIZE, animatedSprite.getPosition().y))) isMovable = false;
		}

		if(walking == false && isMovable == true)
		{
			nextspot = animatedSprite.getPosition().x - CHARACTER_SIZE; 
			direction = DIR_LEFT;
			walking = true;

			animatedSprite.setAnimation(walkingAnimation[direction]);
			animatedSprite.play();
		}
		else if (isMovable == false)
		{
			animatedSprite.setAnimation(walkingAnimation[DIR_LEFT]);
			animatedSprite.setFrame(0);
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		for (std::vector<tmx::MapObject>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			tmx::MapObject &object = *it;
			if (object.Contains(sf::Vector2f(animatedSprite.getPosition().x + CHARACTER_SIZE, animatedSprite.getPosition().y))) isMovable = false;
		}

		if(walking == false && isMovable == true)
		{
			nextspot = animatedSprite.getPosition().x + CHARACTER_SIZE; 
			direction = DIR_RIGHT;
			walking = true;

			animatedSprite.setAnimation(walkingAnimation[direction]);
			animatedSprite.play();
		}
		else if (isMovable == false)
		{
			animatedSprite.setAnimation(walkingAnimation[DIR_RIGHT]);
			animatedSprite.setFrame(0);
		}
	}
}

void PlayerObject::draw(sf::RenderTarget &tg)
{
	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 4; i++)
		{
			walkingAnimation[a].setSpriteSheet(texture);
		}
	}
	tg.draw(animatedSprite);
}

AnimatedSprite& PlayerObject::getSprite()
{
	return animatedSprite;
}

void PlayerObject::update(sf::Time &time)
{
	if(walking == true)
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