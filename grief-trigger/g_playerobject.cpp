#include "g_playerobject.h"

#include "h_config.h"
#include "d_objects.h"
#include "g_scenemanager.h"

void addFrames(thor::FrameAnimation& animation, int x, int yFirst, int yLast, float duration = 1.f)
{
	const int step = (yFirst < yLast) ? +1 : -1;
	yLast += step; // so yLast is excluded in the range

	for (int y = yFirst; y != yLast; y += step)
		animation.addFrame(duration, sf::IntRect(CHARACTER_SIZE*x, CHARACTER_SIZE*y, CHARACTER_SIZE, CHARACTER_SIZE));
}

PlayerObject::PlayerObject()
{
}

void PlayerObject::init(sf::Vector2f pos, tmx::MapObject &playerObject)
{
	object = &playerObject;

	sprite.setTexture(TextureManager::instance().getTexture("assets/player.png"));
	sprite.setPosition(pos);

	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 4; i++)
		{
			animations[a].addFrame(1.f, sf::IntRect(i * CHARACTER_SIZE, a * CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE));
		}
	}

	animator.addAnimation("up", animations[DIR_UP], sf::seconds(0.5f));
	animator.addAnimation("right", animations[DIR_RIGHT], sf::seconds(0.5f));
	animator.addAnimation("left", animations[DIR_LEFT], sf::seconds(0.5f));
	animator.addAnimation("down", animations[DIR_DOWN], sf::seconds(0.5f));

	animator.playAnimation("down");

	nx = sprite.getPosition().x;
	ny = sprite.getPosition().y;

	direction = DIR_UP;
	walking = false;
}

void PlayerObject::draw(sf::RenderTarget &tg)
{
	tg.draw(sprite);
}

sf::Sprite& PlayerObject::getSprite()
{
	return sprite;
}

void PlayerObject::update(sf::Time &time)
{
	if (DialoguePanel::instance().isHided() == false && !walking)
	{
		bool isMovable = true;

		/************************************************************************/
		/* Check for input														*/
		/************************************************************************/
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			step(DIR_UP);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			step(DIR_DOWN);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			step(DIR_LEFT);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			step(DIR_RIGHT);
		}
	}

	/************************************************************************/
	/* Process smooth walking														*/
	/************************************************************************/
	if(walking == true)
	{
		if(direction == DIR_UP)
		{
			ny -= playerMoveSpeed * time.asSeconds();

			if(ny <= nextspot)
			{
				ny = nextspot; 
				walking = false;
				direction = DIR_IDLE;

				animator.stopAnimation();
			}
		}
		if(direction == DIR_DOWN)
		{
			ny += playerMoveSpeed * time.asSeconds();

			if(ny >= nextspot)
			{
				ny = nextspot;
				walking = false;
				direction = DIR_IDLE;

				animator.stopAnimation();
			}
		}
		if(direction == DIR_LEFT)
		{
			nx -= playerMoveSpeed * time.asSeconds();

			if(nx <= nextspot)
			{
				nx = nextspot;
				walking = false;
				direction = DIR_IDLE;

				animator.stopAnimation();
			}
		}
		if(direction == DIR_RIGHT)
		{
			nx += playerMoveSpeed * time.asSeconds();

			if(nx >= nextspot)
			{
				nx = nextspot;
				walking = false;
				direction = DIR_IDLE;

				animator.stopAnimation();
			}
		}
	}

	//Update animation and position
	object->SetPosition(sf::Vector2f(nx, ny));
	sprite.setPosition(sf::Vector2f(nx, ny));

	animator.update(time);
	animator.animate(sprite);
}

bool PlayerObject::step(int dir)
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

		for (std::vector<tmx::MapObject>::iterator it = SceneManager::instance().getScene().getObjects().begin(); it != SceneManager::instance().getScene().getObjects().end(); ++it)
		{
			tmx::MapObject &object = *it;
			if (object.GetAABB().intersects(sf::FloatRect(sprite.getPosition().x + movement.x, sprite.getPosition().y + movement.y, CHARACTER_SIZE, CHARACTER_SIZE)) 
				&& object.GetName() == "" ) isMovable = false;
			if (walking == false && object.GetAABB().intersects(sf::FloatRect(sprite.getPosition().x + movement.x, sprite.getPosition().y + movement.y, CHARACTER_SIZE, CHARACTER_SIZE)) 
				&& object.GetName() != "" 
				&& object.GetName() != "hero" 
				&& object.GetName() != "door"
				&& object.GetName() != "null")
			{
				isMovable = false;
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
			//animator.stopAnimation();
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

	return true;
}