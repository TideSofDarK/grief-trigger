#include "g_playerobject.h"

#include "h_config.h"
#include "d_objects.h"
#include "g_scenemanager.h"

PlayerObject::PlayerObject()
{
}

void PlayerObject::init(sf::Uint32 x, sf::Uint32 y, sf::Vector2f cameraStart, tmx::MapObject &playerObject, std::vector<Squad> &squadsList)
{
	object = &playerObject;
	squads = &squadsList;

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
	animatedSprite.setPosition(x, y);
	animatedSprite.stop();
	animatedSprite.setLooped(true);

	direction = DIR_TOP;
	walking = false;
	nx = animatedSprite.getPosition().x;
	ny = animatedSprite.getPosition().y;

	camStart = cameraStart;

	ncy = ncx = 0;
}

void PlayerObject::move()
{
	if (DialoguePanel::instance().isHided() == false && !walking)
	{
		bool isMovable = true;

		/************************************************************************/
		/* Check for input and movable space									*/
		/************************************************************************/
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			step(DIR_TOP);
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
}

void PlayerObject::draw(sf::RenderTarget &tg)
{
	tg.draw(animatedSprite);
}

AnimatedSprite& PlayerObject::getSprite()
{
	return animatedSprite;
}

void PlayerObject::input(sf::Event &event)
{

}

void PlayerObject::update(sf::Time &time, sf::View &camera)
{
	/************************************************************************/
	/* Scrolling                                                            */
	/************************************************************************/
	if (animatedSprite.getPosition().y > camera.getCenter().y + (HALF_HEIGHT / 2) - (CHARACTER_SIZE / 2))
	{
		oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_IN, 1.0f, &ncy, ncy + HALF_HEIGHT);
	}
	if (animatedSprite.getPosition().y < camera.getCenter().y - (HALF_HEIGHT / 2) - (CHARACTER_SIZE / 2))
	{
		oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_IN, 1.0f, &ncy, ncy - HALF_HEIGHT);
	}
	if (animatedSprite.getPosition().x > camera.getCenter().x + (HALF_WIDTH / 2) - (CHARACTER_SIZE / 2))
	{
		oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_IN, 1.0f, &ncx, ncx + HALF_WIDTH);
	}
	if (animatedSprite.getPosition().x < camera.getCenter().x - (HALF_WIDTH / 2) - (CHARACTER_SIZE / 2))
	{
		oTweener.addTween(&CDBTweener::TWEQ_LINEAR, CDBTweener::TWEA_IN, 1.0f, &ncx, ncx - HALF_WIDTH);
	}

	if ((int)ncy % HALF_HEIGHT != 0)
	{
		camera.setCenter((camera.getCenter().x), (camStart.y + (int)ncy));
	}

	if ((int)ncx % HALF_WIDTH != 0)
	{
		camera.setCenter((camStart.x + (int)ncx), (camera.getCenter().y));
	}

	oTweener.step(time.asSeconds());

	/************************************************************************/
	/* Process smooth walking														*/
	/************************************************************************/
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

	//Update animation and position
	animatedSprite.setPosition(nx, ny);
	object->SetPosition(sf::Vector2f(nx, ny));
	animatedSprite.update(time);
}

bool PlayerObject::step(int dir)
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

		for (std::vector<tmx::MapObject>::iterator it = SceneManager::instance().getScene().getObjects().begin(); it != SceneManager::instance().getScene().getObjects().end(); ++it)
		{
			tmx::MapObject &object = *it;
			if (object.GetAABB().intersects(sf::FloatRect(animatedSprite.getPosition().x + movement.x, animatedSprite.getPosition().y + movement.y, CHARACTER_SIZE, CHARACTER_SIZE)) 
				&& object.GetName() == "" ) isMovable = false;
			if (walking == false && object.GetAABB().intersects(sf::FloatRect(animatedSprite.getPosition().x + movement.x, animatedSprite.getPosition().y + movement.y, CHARACTER_SIZE, CHARACTER_SIZE)) 
				&& object.GetName() != "" 
				&& object.GetName() != "hero" 
				&& object.GetName() != "door")
			{
				isMovable = false;
			}
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

	return true;
}