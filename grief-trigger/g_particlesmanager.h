#ifndef PARTICLESMANAGER_INCLUDE
#define PARTICLESMANAGER_INCLUDE

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include <Thor/Particles.hpp>
#include <Thor/Animation.hpp>
#include <Thor/Vectors/PolarVector.hpp>
#include <Thor/Math/Distributions.hpp>

#include "h_config.h"

class ParticlesManager
{
private:
	//Particle texture
	sf::Texture texture;
	
	//Particle system
	thor::ParticleSystem system;

	//Additional particle system for background effects
	thor::ParticleSystem additionalSystem;
public:
	ParticlesManager(void) : system(texture), additionalSystem(texture) { texture.loadFromFile("assets/smoke.png"); };
	void init(std::string name);
	void update(sf::Time &time);
	void draw(sf::RenderTarget &tg);
	void drawUnder(sf::RenderTarget &tg);
};

#endif