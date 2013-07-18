#include "g_npc.h"

NPC::NPC(std::string name, sf::Uint32 x, sf::Uint32 y)
{
	texture.loadFromFile("assets/" + name + ".png");

	sprite.setTexture(texture);
	sprite.setPosition(x, y);
}

void NPC::draw(sf::RenderTarget &tg)
{
	sprite.setTexture(texture);
	tg.draw(sprite);
}