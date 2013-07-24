#include "d_objects.h"

#include "d_resourcesmanager.h"
#include "h_config.h"

ObjectSounds::ObjectSounds()
{
	doorBuffer.loadFromFile("assets/door.wav");
	doorSound.setBuffer(doorBuffer);
}

void ObjectSounds::playDoorSound()
{
	doorSound.play();
}

Door::Door()
{}

void Door::init(sf::Vector2f pos, tmx::MapObject &obj)
{
	object = &obj;
	sprite.setPosition(pos);
	sprite.setTexture(ResourcesManager::instance().getTexture("assets/tileset.png"));
	sprite.setTextureRect(sf::IntRect(192, 96, CHARACTER_SIZE, CHARACTER_SIZE));

	opened = false;
}

void Door::draw(sf::RenderTarget &tg)
{
	tg.draw(sprite);
}

void Door::open()
{
	ObjectSounds::instance().playDoorSound();
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