#ifndef BATTLE_INCLUDE
#define BATTLE_INCLUDE

#include <SFML/Graphics.hpp>

class Bar
{
private:
public:
};

class Battle
{
private:
public:
	Battle(void);
	void draw(sf::RenderTarget &tg);
	void update(sf::Time time);
	void input(sf::Event &event);
};

#endif