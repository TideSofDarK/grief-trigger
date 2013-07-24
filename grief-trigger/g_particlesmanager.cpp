#include "g_particlesmanager.h"

#include "d_resourcesmanager.h"

float RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

void ParticlesManager::init(std::string name)
{
	system.setTexture(ResourcesManager::instance().getTexture("assets/smoke.png"));

	thor::UniversalEmitter emitter;
	emitter.setEmissionRate(0.2f);
	emitter.setParticleLifetime(sf::seconds(15));

	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(5), sf::seconds(9)));
	emitter.setParticlePosition(thor::Distributions::rect(sf::Vector2f(2.f, HALF_HEIGHT), sf::Vector2f(1, HALF_HEIGHT / 2)) );

	system.addEmitter(emitter);

	sf::Vector2f acceleration(70.f, 0.f);
	thor::ForceAffector gravityAffector(acceleration);
	system.addAffector(gravityAffector);

	thor::FadeAnimation fader(0.15f, 0.15f);

	system.addAffector(thor::AnimationAffector(fader));

	//Additional
	additionalSystem.setTexture(ResourcesManager::instance().getTexture("assets/smoke.png"));

	emitter.setEmissionRate(0.3f);
	emitter.setParticleLifetime(sf::seconds(15));

	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(5), sf::seconds(9)));
	emitter.setParticlePosition(thor::Distributions::rect(sf::Vector2f(RandomFloat(30, 50), HALF_HEIGHT), sf::Vector2f(RandomFloat(15, 25), HALF_HEIGHT / 2)) );

	additionalSystem.addEmitter(emitter);
	additionalSystem.addAffector(gravityAffector);
	additionalSystem.addAffector(thor::AnimationAffector(fader));
}

void ParticlesManager::draw(sf::RenderTarget &tg)
{
	tg.draw(system);
}

void ParticlesManager::drawUnder(sf::RenderTarget &tg)
{
	tg.draw(additionalSystem);
}

void ParticlesManager::update(sf::Time &time)
{
	system.update(time);
	additionalSystem.update(time);
}