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

class ParticlesManager
{
private:
	//Particle texture
	sf::Texture texture;
	
	//List of systems
	thor::ParticleSystem system;
public:
	ParticlesManager(void);
	void init(std::string name);
	void update(sf::Time &time);
	void draw(sf::RenderTarget &tg);
};

#endif