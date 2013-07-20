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

using namespace sf;
using namespace tmx;

int main()
{
	RenderWindow window(VideoMode(1280, 720), "Grief Trigger Turbo HD", sf::Style::Default, sf::ContextSettings(32));
	window.setFramerateLimit(60);

	sf::View camera(sf::FloatRect(-1280 / 3, -720/ 3, 1280, 720));
	sf::View unscalable(sf::FloatRect(0, 0, 1280, 720));
	camera.zoom(1.f/2.f);

	MapLoader ml("assets/");
	ml.Load("test.tmx");

	std::locale::global(std::locale("Russian"));

	PlayerObject po = PlayerObject(128, 128);

	DialogueInfo di = DialogueInfo("text.xml");

	AppearingText at = AppearingText(di);

	sf::Clock frameClock;

	sf::Texture texture;
	texture.loadFromFile("assets/girl.png");
	thor::ParticleSystem system(texture);
	sf::Clock clock;
	thor::UniversalEmitter::Ptr emitter = thor::UniversalEmitter::create();
	emitter->setEmissionRate(30);
	emitter->setParticleLifetime(sf::seconds(5));
	system.addEmitter(emitter);
	emitter->setParticleLifetime( thor::Distributions::uniform(sf::seconds(5), sf::seconds(7)) );
	emitter->setParticlePosition( thor::Distributions::circle(sf::Vector2f(10, 10), 90) );   // Emit particles in given circle
	emitter->setParticleVelocity( thor::Distributions::deflect(sf::Vector2f(10, 10), 15.f) ); // Emit towards direction with deviation of 15°
	emitter->setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );      // Rotate randomly

	EffectManager ef = EffectManager(sf::Vector2f(window.getSize()));
	
	sf::RenderTexture finalTexture;
	if (!finalTexture.create(window.getSize().x, window.getSize().y)) return -1;


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) ef.setCurrentEffect("distortion", sf::seconds(1));
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) ef.setCurrentEffect("shader", sf::seconds(1));

			at.input(event);
		}

		ef.update();

		at.update();

		po.move(ml.GetLayers().back().objects, at);
		po.update(frameClock.restart());

		//system.update(clock.restart());

		finalTexture.clear();

		//Game content
		finalTexture.setView(camera);
		ml.Draw(finalTexture, false);
		po.draw(finalTexture);

		//Unscalable
		finalTexture.setView(unscalable);
		at.draw(finalTexture);
		//finalTexture.draw(system);

		finalTexture.display();

		window.clear();

		//Draw trough effect manager
		ef.draw(finalTexture, window);

		window.display();
	}

	return 0;
}