#include "g_particlesmanager.h"

ParticlesManager::ParticlesManager(void) : system(texture)
{
	texture.loadFromFile("assets/smoke.png");
}

void ParticlesManager::init(std::string name)
{
	//texture.loadFromFile("assets/smoke.png");
	//system.setTexture(texture);

	//thor::UniversalEmitter emitter;
	//emitter.setEmissionRate(30.f);
	//emitter.setParticleLifetime(sf::seconds(5.f));
	//system.addEmitter(thor::refEmitter(emitter));

	//thor::ColorGradient gradient;
	//gradient[0.f] = sf::Color(0, 150, 0);
	//gradient[0.5f] = sf::Color(0, 150, 100);
	//gradient[1.f] = sf::Color(0, 0, 150);

	//thor::ColorAnimation colorizer(gradient);
	//thor::FadeAnimation fader(0.1f, 0.1f);

	//system.addAffector( thor::AnimationAffector(colorizer) );
	//system.addAffector( thor::AnimationAffector(fader) );
	//system.addAffector( thor::TorqueAffector(100.f) );
	//system.addAffector( thor::ForceAffector(sf::Vector2f(100.f, 0.f))  );

	//thor::PolarVector2f velocity(200.f, -90.f);
}

void ParticlesManager::draw(sf::RenderTarget &tg)
{
	tg.draw(system);
}

void ParticlesManager::update(sf::Time &time)
{
	system.update(time);
}