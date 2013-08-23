#include "d_objects.h"

#include "d_resourcesmanager.h"
#include "h_config.h"
#include "d_gamedata.H"

MoneyEffect::MoneyEffect(sf::Vector2f pos)
{
	sprite = new AnimatedSprite(sf::seconds(0.1f));
	anim = new Animation();
	anim->setSpriteSheet(TextureManager::instance().getTexture("assets/money.png"));

	for (int a = 0; a < 9; a++)
	{
		if (a == 9)
		{
			for (int i = 0; i < 9; i++)
			{
				anim->addFrame(sf::IntRect(a * 32, 0, 32, 32));
			}		
		}
		else
		{
			anim->addFrame(sf::IntRect(a * 32, 0, 32, 32));
		}		
	}

	sprite->setAnimation(*anim);
	sprite->setLooped(true);
	sprite->play();

	sprite->setPosition(pos);	
	sPos = pos;
	visible = true;
}

void MoneyEffect::draw(sf::RenderTarget &tg)
{
	if (visible)
	{
		sprite->move(0,-11);
		tg.draw(*sprite);
		sprite->move(0, 11);
	}	
}

void MoneyEffect::update(sf::Time time)
{
	sprite->update(time);
	sprite->setPosition(sPos.x, interpolateLinear(sprite->getPosition().y, sPos.y - 20, 0.08f));
	if (sprite->getPosition().y <= sPos.y - 18)
	{
		visible = false;
	}
}

ShopCrystal::ShopCrystal()
{
	sprite = new AnimatedSprite(sf::seconds(0.1f));
	anim = new Animation();
	anim->setSpriteSheet(TextureManager::instance().getTexture("assets/crystal.png"));

	for (int a = 0; a < 5; a++)
	{
		if (a == 0)
		{
			for (int i = 0; i < 5; i++)
			{
				anim->addFrame(sf::IntRect(a * 32, 0, 32, 32));
			}		
		}
		else
		{
			anim->addFrame(sf::IntRect(a * 32, 0, 32, 32));
		}		
	}

	sprite->setAnimation(*anim);
	sprite->setLooped(true);
	sprite->play();
}

void ShopCrystal::init(sf::Vector2f pos, tmx::MapObject &obj)
{
	object = &obj;
	sprite->setPosition(pos);	
	sPos = pos;
}

void ShopCrystal::draw(sf::RenderTarget &tg)
{
	sprite->move(0,-11);
	tg.draw(*sprite);
	sprite->move(0, 11);
}

void ShopCrystal::update(sf::Time time)
{
	if (!falling)
	{
		sprite->setPosition(sPos.x, interpolateLinear(sprite->getPosition().y, sPos.y - 11,0.02));
		if (sprite->getPosition().y <= sPos.y - 6)
		{
			falling = true;
		}
	}
	else
	{
		sprite->setPosition(sPos.x, interpolateLinear(sprite->getPosition().y, sPos.y + 11,0.02));
		if (sprite->getPosition().y >= sPos.y + 6)
		{
			falling = false;
		}
	}
}

Portal::Portal(bool type)
{
	state = type;

	sprite = new AnimatedSprite(sf::seconds(0.1f));
	anim = new Animation();

	if (state == true)
	{
		anim->setSpriteSheet(TextureManager::instance().getTexture("assets/portalstart.png"));
	}
	else
	{
		anim->setSpriteSheet(TextureManager::instance().getTexture("assets/portal.png"));
	}

	for (int a = 0; a < 9; a++)
	{
		anim->addFrame(sf::IntRect(a * 32, 0, 32, 32));
	}

	sprite->setAnimation(*anim);
	sprite->setLooped(true);
	sprite->play();
}

void Portal::init(sf::Vector2f pos, tmx::MapObject &obj)
{
	object = &obj;
	sprite->setPosition(pos);	
	sPos = pos;
}

void Portal::draw(sf::RenderTarget &tg)
{
	sprite->move(0,-11);
	tg.draw(*sprite);
	sprite->move(0, 11);
}

void Portal::update(sf::Time time)
{
	sprite->update(sf::seconds(1.f / 15.f));

	if (!falling)
	{
		sprite->setPosition(sPos.x, interpolateLinear(sprite->getPosition().y, sPos.y - 11,0.05));
		if (sprite->getPosition().y <= sPos.y - 9)
		{
			falling = true;
		}
	}
	else
	{
		sprite->setPosition(sPos.x, interpolateLinear(sprite->getPosition().y, sPos.y + 11,0.05));
		if (sprite->getPosition().y >= sPos.y + 9)
		{
			falling = false;
		}
	}
}

Chest::Chest()
{
	sprite = new AnimatedSprite();
	anim = new Animation();
	idle = new Animation();

	//sprite = AnimatedSprite(sf::seconds(0.5f));
	anim->setSpriteSheet(TextureManager::instance().getTexture("assets/chest.png"));
	idle->setSpriteSheet(TextureManager::instance().getTexture("assets/chest.png"));

	for (int a = 0; a < 11; a++)
	{
		anim->addFrame(sf::IntRect(a * 32, 0, 32, 32));
	}

	idle->addFrame(sf::IntRect(0, 0, 32, 32));

	sprite->setAnimation(*idle);
	sprite->setLooped(true);
	sprite->play();

}

void Chest::init(sf::Vector2f pos, tmx::MapObject &obj)
{
	object = &obj;
	sprite->setPosition(pos);	

	opened = false;
}

void Chest::draw(sf::RenderTarget &tg)
{
	tg.draw(*sprite);
}

void Chest::update(sf::Time time)
{
	sprite->update(sf::seconds(1.f / 15.f));
	if (opened && sprite->getCurrentFrameNumber() == 10)
	{
		sprite->pause();
	}
}

void Chest::open()
{
	SoundManager::instance().playDoorSound();
	opened = true;
	sprite->setAnimation(*anim);
	sprite->setLooped(false);
	sprite->play();

	int r = (rand() % (int)(23));

	if (r == 1 || r == 2)
	{
		GameData::instance().addMana();
	}
	else if (r == 3 || r == 4)
	{
		GameData::instance().addMed();
	}
	else if (r == 5)
	{
		GameData::instance().addShard();
	}
	else 
	{
		GameData::instance().addMoney((1000 + (rand() % (int)(1500 - 1000 + 1))) * (GameData::instance().getPlayer().getLevel()/2));
	}

	SoundManager::instance().playPressSound();
}

bool Chest::isOpened()
{
	return opened;
}

tmx::MapObject& Chest::getOnMap()
{
	return *object;
}

Door::Door()
{}

void Door::init(sf::Vector2f pos, tmx::MapObject &obj)
{
	object = &obj;
	sprite.setPosition(pos);	
	switch (Level::instance().getDay())
	{
	case 1:
		sprite.setTexture(TextureManager::instance().getTexture("assets/tileset.png"));
		break;
	case 2:
		sprite.setTexture(TextureManager::instance().getTexture("assets/tileset.png"));
		break;
	case 3:
		sprite.setTexture(TextureManager::instance().getTexture("assets/tileset.png"));
		break;
	case 4:
		sprite.setTexture(TextureManager::instance().getTexture("assets/castle.png"));
		break;
	case 5:
		sprite.setTexture(TextureManager::instance().getTexture("assets/castle.png"));
		break;
	case 6:
		sprite.setTexture(TextureManager::instance().getTexture("assets/castle.png"));
		break;
	case 7:
		sprite.setTexture(TextureManager::instance().getTexture("assets/castle.png"));
		break;
	default:
		break;
	}
	sprite.setTextureRect(sf::IntRect(192, 96, CHARACTER_SIZE, CHARACTER_SIZE));

	opened = false;
}

void Door::draw(sf::RenderTarget &tg)
{
	tg.draw(sprite);
}

void Door::open()
{
	SoundManager::instance().playDoorSound();
	opened = true;
	sprite.setTextureRect(sf::IntRect(192, 64, CHARACTER_SIZE, CHARACTER_SIZE));
}

bool Door::isOpened()
{
	return opened;
}

tmx::MapObject& Door::getOnMap()
{
	return *object;
}

NPC::NPC()
{}

void NPC::init(sf::Vector2f pos, tmx::MapObject &obj)
{
	object = &obj;
	sprite.setPosition(pos);
	sprite.setTexture(TextureManager::instance().getTexture("assets/" + obj.GetName() + ".png"));
	sprite.move(0, -11);
}

void NPC::draw(sf::RenderTarget &tg)
{
	tg.draw(sprite);
}

tmx::MapObject& NPC::getOnMap()
{
	return *object;
}

Swing::Swing()
{
	sprite = AnimatedSprite(sf::seconds(0.5f));

	animation.setSpriteSheet(TextureManager::instance().getTexture("assets/swing.png"));

	for (int a = 0; a < 6; a++)
	{
		animation.addFrame(sf::IntRect(a * 17, 0, 17, 14));
	}

	animation.addFrame(sf::IntRect(5 * 17, 0, 1, 1));
	animation.addFrame(sf::IntRect(5 * 17, 0, 1, 1));
	animation.addFrame(sf::IntRect(5 * 17, 0, 1, 1));
	animation.addFrame(sf::IntRect(5 * 17, 0, 1, 1));
	animation.addFrame(sf::IntRect(5 * 17, 0, 1, 1));
}

void Swing::init(sf::Vector2f pos)
{
	sprite.setAnimation(animation);
	sprite.setLooped(false);
	sprite.play();
	
	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	sprite.setPosition(pos.x + sprite.getLocalBounds().width / 2, pos.y + sprite.getLocalBounds().height / 2);
	sprite.setScale(2,2);

	working = true;
	played = false;
}

void Swing::draw(sf::RenderTarget &tg)
{
	if (working)
	{
		tg.draw(sprite);
	}
}

void Swing::update(sf::Time time)
{
	if (working && sprite.getCurrentFrameNumber() == 6 && !played)
	{
		SoundManager::instance().playHurtSound();
		played = true;
	}
	sprite.update(sf::seconds(1.f / 15.f));
	if (!sprite.isPlaying())
	{
		working = false;
	}
}

Column::Column()
{}

void Column::init(sf::Vector2f pos, tmx::MapObject &obj)
{
	object = &obj;
	spriteUpper.setPosition(pos.x, pos.y- CHARACTER_SIZE);	
	spriteUpper.setTexture(TextureManager::instance().getTexture("assets/castle.png"));
	spriteUpper.setTextureRect(sf::IntRect(96, 192, CHARACTER_SIZE, CHARACTER_SIZE));

	sprite.setPosition(pos);	
	sprite.setTexture(TextureManager::instance().getTexture("assets/castle.png"));
	sprite.setTextureRect(sf::IntRect(96, 192 + CHARACTER_SIZE, CHARACTER_SIZE, CHARACTER_SIZE));
}

void Column::draw(sf::RenderTarget &tg)
{
	tg.draw(sprite);
}

void Column::drawUpper(sf::RenderTarget &tg)
{
	tg.draw(spriteUpper);
}

tmx::MapObject& Column::getOnMap()
{
	return *object;
}