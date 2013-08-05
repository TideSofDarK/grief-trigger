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

	RenderWindow window(VideoMode(1280, 720), "Grief Trigger Turbo HD", sf::Style::Titlebar | sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	sf::Clock frameClock;
	sf::Clock updateClock;
	sf::Int32 nextUpdate = updateClock.getElapsedTime().asMilliseconds();
	float updateRate(1.0f / 15.f);
	float maxUpdates = 1;

	tmx::MapLoader ml("assets/");

	SceneManager::instance().setScene("test.tmx", ml);

	while (window.isOpen())
	{
		sf::Int32 updateTime = updateClock.getElapsedTime().asMilliseconds();
		Uint32 updates = 0;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			SceneManager::instance().input(event);
		}	

		while((updateTime - nextUpdate) >= updateRate && updates++ < maxUpdates)
		{
			SceneManager::instance().updateFixed(sf::seconds(updateRate));	
			nextUpdate += updateRate;
		} 

		SceneManager::instance().update(frameClock.restart());

		window.clear();
		SceneManager::instance().draw(window);
		window.display();

		//sf::sleep(sf::milliseconds(1));
	}

	return 0;
}