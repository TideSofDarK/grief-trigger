#include "g_shadermanager.h"

ShaderManager::ShaderManager(void)
{
}

void ShaderManager::init(sf::Vector2f resolution)
{
	size = resolution;
	working = false;
}

void ShaderManager::update()
{
	if (working)
	{
		counter++;
		//If minute 
		if (counter > 60)
		{
			counter = 0;
		}
		
		if (clock.getElapsedTime().asSeconds() > duration.asSeconds())
		{
			working = false;
		}

		shader.setParameter("seconds", (float)counter);
	}
}

void ShaderManager::setCurrentEffect(std::string name, sf::Time time)
{
	if (!working)
	{
		shader.loadFromFile("assets/" + name + ".frag", sf::Shader::Fragment);
		shader.setParameter("size", size);
		working = true;
		duration = time;
		clock.restart();
	}
}

void ShaderManager::draw(sf::RenderTexture &rt, sf::RenderTarget &target)
{
	sf::Sprite sprite(rt.getTexture());

	if (working)
	{
		shader.setParameter("texture", rt.getTexture());
		target.draw(sprite, &shader);
	}
	else
	{
		target.draw(sprite);
	}
}

sf::Shader& ShaderManager::getCurrentEffect()
{
	return shader;
}