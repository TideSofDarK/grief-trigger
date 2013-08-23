#include "g_playerobject.h"

#include "h_config.h"
#include "d_objects.h"
#include "g_scenemanager.h"
#include "os_x360controller.hpp"

void addFrames(thor::FrameAnimation& animation, int y, int xFirst, int xLast, float duration = 1.f)
{
	const int step = (xFirst < xLast) ? +1 : -1;
	xLast += step; // so yLast is excluded in the range

	for (int x = xFirst; x != xLast; x += step)
		animation.addFrame(duration, sf::IntRect(CHARACTER_SIZE*x, CHARACTER_SIZE*y, CHARACTER_SIZE, CHARACTER_SIZE));
}

PlayerObject::PlayerObject()
{
	sprite.setTexture(TextureManager::instance().getTexture("assets/player.png"));

	for (int a = 0; a < 4; a++)
	{
		for (int i = 0; i < 4; i++)
		{
			animations[a].addFrame(1, sf::IntRect(i * CHARACTER_SIZE, a * CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE));
		}
	}

	for (int a = 0; a < 4; a++)
	{
		idleAnimations[a].addFrame(1, sf::IntRect(0, a * CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE));
	}

	animator.addAnimation("up", animations[DIR_UP], sf::seconds(1.5f));
	animator.addAnimation("right", animations[DIR_RIGHT], sf::seconds(1.5f));
	animator.addAnimation("left", animations[DIR_LEFT], sf::seconds(1.5f));
	animator.addAnimation("down", animations[DIR_DOWN], sf::seconds(1.5f));

	animator.addAnimation("idle_up", idleAnimations[DIR_UP], sf::seconds(1.f));
	animator.addAnimation("idle_right", idleAnimations[DIR_RIGHT], sf::seconds(1.f));
	animator.addAnimation("idle_left", idleAnimations[DIR_LEFT], sf::seconds(1.f));
	animator.addAnimation("idle_down", idleAnimations[DIR_DOWN], sf::seconds(1.f));

	animator.playAnimation("idle_down");
}

void PlayerObject::init(sf::Vector2f pos, tmx::MapObject &playerObject)
{
	object = &playerObject;

	sprite.setPosition(pos);

	nx = sprite.getPosition().x;
	ny = sprite.getPosition().y;

	direction = DIR_UP;
	walking = false;
}

void PlayerObject::draw(sf::RenderTarget &tg)
{
	sprite.move(0,-11);
	tg.draw(sprite);
	sprite.move(0,11);
}

sf::Sprite& PlayerObject::getSprite()
{
	return sprite;
}

void PlayerObject::update(sf::Time &time)
{
	/************************************************************************/
	/* Process smooth walking												*/
	/************************************************************************/
	if (DialoguePanel::instance().isHided() == false && !walking)
	{
		bool isMovable = true;

		/************************************************************************/
		/* Check for input														*/
		/************************************************************************/

		sf::Vector2f ls, rs;
		xb::Joystick::getSticksPosition(0, ls, rs);
		
		sf::Vector2f ja = xb::Joystick::sfJoystick_normaliseDirectionProportional(ls);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || xb::Joystick::getAxisDir() == DIR_UP)
		{
			step(DIR_UP);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || xb::Joystick::getAxisDir() == DIR_DOWN)
		{
			step(DIR_DOWN);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)  || xb::Joystick::getAxisDir() == DIR_LEFT)
		{
			step(DIR_LEFT);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)  || xb::Joystick::getAxisDir() == DIR_RIGHT)
		{
			step(DIR_RIGHT);
		}	
	}
	if(walking == true)
	{
		if(direction == DIR_UP)
		{
			ny -= playerMoveSpeed;
			SoundManager::instance().playStep1Sound();

			if(ny <= nextspot)
			{
				ny = nextspot; 
				walking = false;

				if (animator.isPlayingAnimation() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					animator.playAnimation("idle_up",true);
				}
			}
		}
		if(direction == DIR_DOWN)
		{
			ny += playerMoveSpeed;SoundManager::instance().playStep1Sound();

			if(ny >= nextspot)
			{
				ny = nextspot;
				walking = false;

				if (animator.isPlayingAnimation() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					animator.playAnimation("idle_down",true);
				}
			}
		}
		if(direction == DIR_LEFT)
		{
			nx -= playerMoveSpeed;SoundManager::instance().playStep1Sound();

			if(nx <= nextspot)
			{
				nx = nextspot;
				walking = false;

				if (animator.isPlayingAnimation() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					animator.playAnimation("idle_left",true);
				}
			}
		}
		if(direction == DIR_RIGHT)
		{
			nx += playerMoveSpeed;SoundManager::instance().playStep1Sound();

			if(nx >= nextspot)
			{
				nx = nextspot;
				walking = false;

				if (animator.isPlayingAnimation() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					animator.playAnimation("idle_right",true);
				}
			}
		}
	}
	else //vot tut-ta bydlokod vo vse polya
	{
		if (animator.isPlayingAnimation() && animator.getPlayingAnimation() == "up" & !sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			animator.playAnimation("idle_up",true);
		}
		if (animator.isPlayingAnimation() && animator.getPlayingAnimation() == "down" & !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			animator.playAnimation("idle_down",true);
		}
		if (animator.isPlayingAnimation() && animator.getPlayingAnimation() == "left" & !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			animator.playAnimation("idle_left",true);
		}
		if (animator.isPlayingAnimation() && animator.getPlayingAnimation() == "right" & !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			animator.playAnimation("idle_right",true);
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
				&& object.GetName() != "null")
			{
				isMovable = false;
			}
			if (walking == false && object.GetAABB().intersects(sf::FloatRect(sprite.getPosition().x + movement.x, sprite.getPosition().y + movement.y, CHARACTER_SIZE, CHARACTER_SIZE)) 
				&& object.GetName() == "door")
			{
				for (auto i = SceneManager::instance().getScene().getDoors().begin(); i !=SceneManager::instance().getScene().getDoors().end(); ++i)
				{
					if (i->getOnMap().GetPosition() == object.GetPosition())
					{
						if (!i->isOpened())
						{
							isMovable = false;
						}
						else
						{
							isMovable = true;
						}
					}			
				}			
			}
			if (!isMovable)
			{
				break;
			}
		}

		direction = dir;

		if(isMovable == true)
		{
			if (dir == DIR_DOWN || dir == DIR_UP) nextspot = sprite.getPosition().y + movement.y; 
			else if (dir == DIR_LEFT || dir == DIR_RIGHT) nextspot = sprite.getPosition().x + movement.x; 			
			walking = true;

			switch (direction)
			{
			case DIR_UP:
				if ((animator.isPlayingAnimation() && animator.getPlayingAnimation() != "up") || !animator.isPlayingAnimation())
				{
					animator.playAnimation("up", true);
				}
				break;
			case DIR_DOWN:
				if ((animator.isPlayingAnimation() && animator.getPlayingAnimation() != "down") || !animator.isPlayingAnimation())
				{
					animator.playAnimation("down", true);
				}
				break;
			case DIR_LEFT:
				if ((animator.isPlayingAnimation() && animator.getPlayingAnimation() != "left") || !animator.isPlayingAnimation())
				{
					animator.playAnimation("left", true);
				}
				break;
			case DIR_RIGHT:
				if ((animator.isPlayingAnimation() && animator.getPlayingAnimation() != "right") || !animator.isPlayingAnimation())
				{
					animator.playAnimation("right", true);
				}
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
				animator.playAnimation("idle_up", true);
				break;
			case DIR_DOWN:
				animator.playAnimation("idle_down", true);
				break;
			case DIR_LEFT:
				animator.playAnimation("idle_left", true);
				break;
			case DIR_RIGHT:			
				animator.playAnimation("idle_right", true);
				break;
			default:
				break;
			}
		}
	}

	return true;
}