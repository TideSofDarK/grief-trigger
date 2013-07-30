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
	window.setVerticalSyncEnabled(true);

	sf::Clock frameClock;

	tmx::MapLoader ml("assets/");

	SceneManager::instance().setScene("test.tmx", ml);

	float mCurrentFrameTime = 0;
	float mCurrentRenderTime = 0;
	float mCurrentRenderFrameTime = 0;
	float mUpdateT = 0.1667;
	float mRenderT = 0.16667;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			SceneManager::instance().input(event);
		}	

		//sf::sleep(sf::milliseconds(10));

		SceneManager::instance().update(sf::microseconds(mCurrentFrameTime));

		if(mCurrentRenderTime >= mRenderT){
			window.clear();
			SceneManager::instance().draw(window);
			window.display();
			mCurrentRenderTime = 0.0;
		}

		mCurrentFrameTime = frameClock.restart().asMicroseconds();
		mCurrentRenderTime += mCurrentFrameTime;

		sf::Time mSleepTime = sf::microseconds(mUpdateT - mCurrentFrameTime);
		sf::sleep(mSleepTime);
	}

	return 0;
}