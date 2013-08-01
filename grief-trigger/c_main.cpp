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
	//window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(true);

	sf::Clock frameClock;
	sf::Clock clock;

	tmx::MapLoader ml("assets/");

	SceneManager::instance().setScene("test.tmx", ml);

	const int FRAMES_PER_SECOND = 120;
	const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

	//DWORD next_game_tick = GetTickCount();
	float next_game_tick = clock.getElapsedTime().asMilliseconds();
	// GetTickCount() returns the current number of milliseconds
	// that have elapsed since the system was started

	int sleep_time = 0;

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			SceneManager::instance().input(event);
		}	

		SceneManager::instance().update(frameClock.restart());

		window.clear();
		SceneManager::instance().draw(window);
		window.display();

		next_game_tick += SKIP_TICKS;
		sleep_time = next_game_tick - clock.getElapsedTime().asMilliseconds();
		if( sleep_time >= 0 ) {
			sf::sleep( sf::milliseconds(sleep_time) );
		}
	}

	return 0;
}