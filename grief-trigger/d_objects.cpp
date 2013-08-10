#include "d_objects.h"

#include "d_resourcesmanager.h"
#include "h_config.h"

Door::Door()
{}

void Door::init(sf::Vector2f pos, tmx::MapObject &obj)
{
	object = &obj;
	sprite.setPosition(pos);
	sprite.setTexture(TextureManager::instance().getTexture("assets/tileset.png"));
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

	animation.addFrame(sf::IntRect(5 * 17, 0, 17, 14));
	animation.addFrame(sf::IntRect(5 * 17, 0, 17, 14));
	animation.addFrame(sf::IntRect(5 * 17, 0, 17, 14));
	animation.addFrame(sf::IntRect(5 * 17, 0, 17, 14));
	animation.addFrame(sf::IntRect(5 * 17, 0, 17, 14));
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
	sprite.update(sf::seconds(1.f / 15.f));
	if (!sprite.isPlaying())
	{
		working = false;
	}
}