#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "MapLoader.h"

#include <Thor/Particles.hpp>
#include <Thor/Graphics.hpp>
#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#include "os_animation.hpp"
#include "os_animatedsprite.hpp"

#include "h_config.h"
#include "g_playerobject.h"
#include "i_dialoguepanel.h"
#include "d_dialogueinfo.h"
#include "g_shadermanager.h"
#include "g_scenemanager.h"

//Include X11 header
#ifdef __GNUC__
#include <X11/Xlib.h>
#endif

using namespace sf;
using namespace tmx;

int main()
{
#ifdef __GNUC__
	XInitThreads();
#endif

	RenderWindow window(VideoMode(1280, 720), "Grief Trigger Turbo HD", sf::Style::Default);
	window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(true);

	sf::View camera(sf::FloatRect(0, 0, 1280, 720));
	sf::View unscalable(sf::FloatRect(0, 0, 1280, 720));
	camera.zoom(1.f/2.f);

	sf::Clock frameClock;

	tmx::MapLoader ml("assets/");

	SceneManager manager("test.tmx", &camera, &unscalable, ml);

	sf::Font font;
	font.loadFromFile("assets/fonts/default.TTF");
	sf::Text fps = sf::Text("", font, 50);
	float lastTime = 0;
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			manager.input(event);
		}

		float currentTime = clock.restart().asSeconds();
		float fpsf = 1.f / currentTime;
		lastTime = currentTime;

		fps.setString(std::to_string((int)fpsf));
		
		manager.update(frameClock.restart());

		window.clear();

		manager.draw(window);
		window.draw(fps);

		window.display();
	}

	return 0;
}