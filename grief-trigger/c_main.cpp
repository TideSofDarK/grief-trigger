#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "MapLoader.h"

#include <Thor/Particles.hpp>
#include <Thor/Graphics.hpp>
#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#include "Animation.hpp"
#include "AnimatedSprite.hpp"

#include "h_config.h"
#include "g_playerobject.h"
#include "i_dialoguepanel.h"
#include "g_sceneobject.h"
#include "g_npc.h"
#include "d_dialogueinfo.h"
#include "g_effectmanager.h"
#include "g_scenemanager.h"

using namespace sf;
using namespace tmx;

int main()
{
	RenderWindow window(VideoMode(1280, 720), "Grief Trigger Turbo HD", sf::Style::Default, sf::ContextSettings(32));
	window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(true);

	sf::View camera(sf::FloatRect(-1280 / 4, -720/ 4, 1280, 720));
	sf::View unscalable(sf::FloatRect(0, 0, 1280, 720));
	camera.zoom(1.f/2.f);

	std::locale::global(std::locale("Russian"));

	sf::Clock frameClock;

	tmx::MapLoader ml("assets/");

	SceneManager manager("test.tmx", &camera, &unscalable, ml);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			manager.input(event);
		}

		manager.update(frameClock.restart());

		window.clear();

		manager.draw(window);

		window.display();
	}

	return 0;
}