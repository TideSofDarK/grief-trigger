#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "MapLoader.h"

#include "Animation.hpp"
#include "AnimatedSprite.hpp"

#include "h_config.h"
#include "g_playerobject.h"
#include "i_appearingtext.h"
#include "g_sceneobject.h"
#include "g_npc.h"
#include "d_dialogueinfo.h"

using namespace sf;
using namespace tmx;

void playAnimation(AnimatedSprite &sprite, Animation &animation)
{
	if(!sprite.isPlaying()) 
	{
		sprite.setAnimation(animation);
		sprite.play(); 
	}
}

int main()
{
	RenderWindow window(VideoMode(1280, 720), "Grief Trigger Turbo HD", sf::Style::Default, sf::ContextSettings(32));
	window.setFramerateLimit(60);

	sf::View camera(sf::FloatRect(-1280 / 3, -720/ 3, 1280, 720));
	sf::View unscalable(sf::FloatRect(0, 0, 1280, 720));
	camera.zoom(1.f/2.f);
	window.setView(camera);

	MapLoader ml("assets/");
	ml.Load("test.tmx");

	std::locale::global(std::locale("Russian"));

	PlayerObject po = PlayerObject(128, 128);

	DialogueInfo di = DialogueInfo("text.xml");

	AppearingText at = AppearingText(di);

	sf::Clock frameClock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			at.input(event);
		}

		at.update();

		po.move(ml.GetLayers().back().objects, at);
		po.update(frameClock.restart());

		window.clear();

		window.setView(camera);
		ml.Draw(window, true);
		po.draw(window);

		window.setView(unscalable);
		at.draw(window);

		window.display();
	}

	return 0;
}