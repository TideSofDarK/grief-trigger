#include <string>
#include <vector>
#include <io.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include <Thor/Particles.hpp>
#include <Thor/Graphics.hpp>
#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#include "os_animation.hpp"
#include "os_animatedsprite.hpp"
#include "os_x360controller.hpp"

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

bool running;
bool isMenu;

class MainMenu
{
private:
	AnimatedSprite	sprite;
	Animation		anim;

	sf::Sprite pointer;

	sf::Sprite back2;

	sf::Sprite v;
	sf::Sprite logo;
	sf::Sprite press;
	sf::Sprite white;
	bool pressFading;
	float pressFactor;
	float newLogoY;

	CDBTweener oTweener;

	bool first;
	bool transition;
	bool check;
	sf::Clock timer;

	ShaderManager sm;

	sf::RenderTexture rt;

	thor::ParticleSystem system;
	thor::UniversalEmitter emitter;

	unsigned int selection;

public:
	MainMenu() : system(TextureManager::instance().getTexture("assets/rticle.png"))
	{
		pointer.setTexture(TextureManager::instance().getTexture("assets/two_pointer.png"));
		//pointer.setPosition(1183, 427);

		//system = new thor::ParticleSystem();

		emitter.setEmissionRate(0.5f);
		emitter.setParticleLifetime( sf::seconds(40) );

		emitter.setParticleRotation( thor::Distributions::uniform(0.f, 360.f) );
		emitter.setParticlePosition( sf::Vector2f(1260,740) );
		emitter.setParticleScale(sf::Vector2f(0.1,0.1));

		system.addEmitter(thor::refEmitter(emitter));

		thor::ColorGradient gradient;
		gradient[0.0f] = sf::Color::Magenta;
		gradient[0.1f] = sf::Color::Red;
		gradient[0.2f] = sf::Color::Blue;
		gradient[0.3f] = sf::Color::Cyan;
		gradient[0.4f] = sf::Color::Green;
		gradient[0.5f] = sf::Color::Red;
		gradient[0.6f] = sf::Color::Magenta;
		gradient[0.7f] = sf::Color::Cyan;
		gradient[0.8f] = sf::Color::Red;
		gradient[0.9f] = sf::Color::Blue;
		gradient[1.0f] = sf::Color::Red;

		thor::ColorAnimation colorizer(gradient);
		thor::FadeAnimation fader(0.1f, 0.1f);

		system.addAffector( thor::ScaleAffector(sf::Vector2f(0.1f,0.1)));	
		system.addAffector( thor::AnimationAffector(fader) );
		system.addAffector( thor::AnimationAffector(colorizer) );

		sprite = AnimatedSprite(sf::seconds(0.3));

		v.setTexture(TextureManager::instance().getTexture("assets/one_upper.png"));
		back2.setTexture(TextureManager::instance().getTexture("assets/two_menu.png"));
		logo.setTexture(TextureManager::instance().getTexture("assets/one_logo.png"));
		press.setTexture(TextureManager::instance().getTexture("assets/one_anybutton.png"));
		anim.setSpriteSheet(TextureManager::instance().getTexture("assets/one_animooted.jpg"));

		for (int i = 0; i < 5; i++)
		{
			anim.addFrame(sf::IntRect(0, 800 * i, WIDTH, HEIGHT));
		}

		sprite.setAnimation(anim);
		sprite.setLooped(true);
		sprite.play();

		logo.setColor(sf::Color(255,255,255,0));
		press.setColor(sf::Color(255,255,255,0));
		pointer.setColor(sf::Color(255,255,255,0));
		press.setPosition(WIDTH/2 - press.getTextureRect().width/2, HEIGHT/2 - press.getTextureRect().height/2 + 100);
		pressFading = true;

		newLogoY = 800;
		oTweener.addTween(&CDBTweener::TWEQ_BACK, CDBTweener::TWEA_INOUT, 2.f, &newLogoY, 0.0f);

		first = true;

		pressFactor = 0.2;
		transition = false;
		check = false;
		
		sm.init(sf::Vector2f(WIDTH, HEIGHT));
		rt.create(WIDTH, HEIGHT);

		MusicManager::instance().playMusicFast("low");

		selection = 1;
	};

	void draw(sf::RenderTarget &tg)
	{
		if (first)
		{
			rt.clear();
			rt.draw(sprite);
			rt.draw(logo);
			if (logo.getPosition().y == 0.0)rt.draw(press);

			if (check && timer.getElapsedTime().asSeconds() < 1)
			{
				if ((int)(rand() % 10) == 3) rt.clear(sf::Color::White);
			}
		}
		else
		{
			rt.clear(sf::Color::White);
			rt.draw(system);
			rt.draw(back2);
			rt.draw(pointer);
		}
		rt.display();
		sm.draw(rt, tg);
		if (first) tg.draw(v);
	};
	void update(sf::Time time)
	{
		MusicManager::instance().update(time);

		system.update(time);

		sm.update();
		oTweener.step(time.asMilliseconds() / 1000.f);

		if ((int)(rand() % 90) == 3 && first) sm.setCurrentEffect("distortion", sf::seconds(0.05));
		if ((int)(rand() % 100) == 3 && !first) sm.setCurrentEffect("inversion", sf::seconds(1));

		if (first)
		{
			logo.setColor(sf::Color(255,255,255,interpolateLinear(logo.getColor().a, 255, 0.05f)));
			logo.setPosition(0, newLogoY);

			if (!pressFading)
			{
				press.setColor(sf::Color(255,255,255,interpolateLinear(press.getColor().a, 0,pressFactor)));
				if (press.getColor().a < 7)
				{
					pressFading = true;
				}
			}
			else
			{
				press.setColor(sf::Color(255,255,255,interpolateLinear(press.getColor().a, 255, pressFactor)));
				if (press.getColor().a > 243)
				{
					pressFading = false;
				}
			}

			sprite.update(time);

			if (transition && timer.getElapsedTime().asSeconds() > 0.5)
			{
				press.setColor(sf::Color(255,255,255,0));
			}

			if (transition && timer.getElapsedTime().asSeconds() > 0.5)
			{
				transition = false;
				oTweener.addTween(&CDBTweener::TWEQ_BACK, CDBTweener::TWEA_INOUT, 2.f, &newLogoY, -800.0f);
			}

			if (!transition && logo.getPosition().y <= -800 && !check)
			{
				sm.setCurrentEffect("rgb", sf::seconds(1));
				check = true;
				timer.restart();SoundManager::instance().playNoiseSound();
			}
			if (check && timer.getElapsedTime().asSeconds() > 1)
			{
				first = false;
				MusicManager::instance().playMusicFast("battle");				
				pressFading = true;
			}
		}		
		else
		{
			pressFactor = 0.2;
			if (!pressFading)
			{
				pointer.setColor(sf::Color(255,255,255,interpolateLinear(pointer.getColor().a, 0,pressFactor)));
				if (pointer.getColor().a < 7)
				{
					pressFading = true;
				}
			}
			else
			{
				pointer.setColor(sf::Color(255,255,255,interpolateLinear(pointer.getColor().a, 255, pressFactor)));
				if (pointer.getColor().a > 243)
				{
					pressFading = false;
				}
			}

			switch (selection)
			{
			case 0:
				pointer.setPosition(1183, 415);
				break;
			case 1:
				pointer.setPosition(1183, 531);
				break;
			case 2:			
				pointer.setPosition(1183, 645);
				break;
			default:
				break;
			}
		}
	};
	void input(sf::Event &event)
	{
		if (first && logo.getPosition().y == 0 && (event.type == sf::Event::KeyPressed || event.type == sf::Event::JoystickButtonPressed))
		{
			SoundManager::instance().playPressSound();
			pressFactor = 0.64;
			transition = true;
			timer.restart();
		}

		if (!first)
		{
			if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X|| event.type == sf::Event::JoystickButtonPressed && xb::Joystick::isButtonPressed(0, xb::Joystick::A)))
			{
				switch (selection)
				{
				case 0:
					if( _access( "save.dat", 0 ) != -1 )
					{
						isMenu = false;
						Parser::instance().loadGame();

						GameData::instance().getThunder().clearOffset();
						GameData::instance().getPlayer().clearOffset();
						GameData::instance().getEmber().clearOffset();

						SceneManager::instance().startTransition(Parser::instance().getSceneInfo(Level::instance().getDay(), Level::instance().getScene()));

						if (Level::instance().getDay() == 6 && Level::instance().getScene() == 7) SceneManager::instance().getScene().setBattleState();
						if (Level::instance().getDay() == 6 && Level::instance().getScene() == 8) SceneManager::instance().getScene().setBattleState();
					}
					else
					{
						SoundManager::instance().playFailSound();
					}
					
					break;
				case 1:
					isMenu = false;
					remove("save.dat");
					Level::instance().setDay(1); 
					Level::instance().setScene(1);

					GameData::instance().addMana();
					GameData::instance().addMana();
					GameData::instance().addMana();

					GameData::instance().addMed();
					GameData::instance().addMed();
					GameData::instance().addMed();
					GameData::instance().addMed();
					GameData::instance().addMed();

					GameData::instance().addShard();

					GameData::instance().addMoney(2000);

					SceneManager::instance().setScene(Parser::instance().getSceneInfo(Level::instance().getDay(), Level::instance().getScene()));
					break;
				case 2:
					running = false;
					break;
				default:
					break;
				}
			}
			
			if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) || (xb::Joystick::getAxisDir() == DIR_UP && event.type == sf::Event::JoystickMoved)))
			{
				if (selection > 0)
				{
					selection--;
				}
				else
				{
					selection = 2;
				}

				SoundManager::instance().playSelectSound();
			}
			if(((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) || (xb::Joystick::getAxisDir() == DIR_DOWN && event.type == sf::Event::JoystickMoved)))
			{
				if (selection < 2)
				{
					selection++;
				}
				else
				{
					selection = 0;
				}

				SoundManager::instance().playSelectSound();
			}
		}
	};
};

MainMenu mm;

int main()
{
#ifdef __GNUC__
	XInitThreads();
#endif

	xb::Joystick::isAnyXBox360ControllerConnected();

	bool isSmall;
	sf::View v(sf::FloatRect(0,0,sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height));

	RenderWindow *window;

	HEIGHT = 800;
	WIDTH = 1280;

	if (sf::VideoMode::getDesktopMode().height > 800)
	{
		window = new RenderWindow(VideoMode(WIDTH, HEIGHT), "Grief Trigger Turbo HD", sf::Style::Titlebar | sf::Style::Close);
		isSmall = false;
	}
	else
	{
		//HEIGHT = sf::VideoMode::getDesktopMode().width;
		//WIDTH = sf::VideoMode::getDesktopMode().height;

		window = new RenderWindow(VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grief Trigger Turbo HD", sf::Style::Titlebar | sf::Style::Close);
		isSmall = true;

		window->setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width/2, 800/2));
		v = window->getView();
		//v.zoom(2.f);
		window->setView(v);

		
	}
	//window->setFramerateLimit(60);
	window->setVerticalSyncEnabled(true);
//	window.setKeyRepeatEnabled(false);

	sf::Clock frameClock;
	sf::Clock updateClock;
	sf::Int32 nextUpdate = updateClock.getElapsedTime().asMilliseconds();
	float updateRate(1.0f / 15.f);
	float maxUpdates = 1;

	Level::instance().setDay(0);  // Menu
	Level::instance().setScene(0);//

	GameData::instance().getPlayer().getWeapon().init(L"Клеймор x");
	GameData::instance().getEmber().getWeapon().init(L"Посох x");
	GameData::instance().getThunder().getWeapon().init(L"Катана x");

	running = true;
	
	isMenu = true;

	

	while (running && window->isOpen())
	{
		sf::Int32 updateTime = updateClock.getElapsedTime().asMilliseconds();
		Uint32 updates = 0;

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window->close();

			if (!isMenu) SceneManager::instance().input(event);
			else mm.input(event);
		}	

		while((updateTime - nextUpdate) >= updateRate && updates++ < maxUpdates)
		{
			if (!isMenu)SceneManager::instance().update(sf::seconds(updateRate));
			else mm.update(sf::seconds(updateRate));		
			nextUpdate += updateRate;
		} 		

		//float lt, rt;
		//xb::Joystick::getTriggers(0, lt, rt);

		//// Triggers controls the vibration
		//xb::Joystick::setVibration(0, lt, rt);

		if (Level::instance().getDay()==0 && Level::instance().getScene()==0)
		{
			isMenu = true;
		}
		if (Level::instance().getDay()==0 && Level::instance().getScene()==0)
		{
			isMenu = true;
		}

		window->clear();
		if (isSmall)
		{		
			if (!isMenu) SceneManager::instance().draw(*window);
			else mm.draw(*window);
		}
		else
		{
			if (!isMenu) SceneManager::instance().draw(*window);
			else mm.draw(*window);
		}	
		window->display();
	}

	//xb::Joystick::setVibration(0);

	return 0;
}