#include "i_ui.h"

#include "h_config.h"

Tip::Tip(std::wstring string, unsigned int k, std::string newType)
{
	key = k;
	type = newType;

	text.setFont(DFont::instance().getFont());
	text.setColor(sf::Color::White);
	text.setString(string);

	switch (k)
	{
	case UP:
		sprite.setTexture(TextureManager::instance().getTexture("assets/up.png"));
		break;
	case RIGHT:
		sprite.setTexture(TextureManager::instance().getTexture("assets/right.png"));
		break;
	case DOWN:
		sprite.setTexture(TextureManager::instance().getTexture("assets/down.png"));
		break;
	case LEFT:
		sprite.setTexture(TextureManager::instance().getTexture("assets/left.png"));
		break;
	case _Z:
		sprite.setTexture(TextureManager::instance().getTexture("assets/Z.png"));
		break;
	case _X:
		sprite.setTexture(TextureManager::instance().getTexture("assets/X.png"));
		break;
	default:
		break;
	}

	sprite.scale(0.3, 0.3);
}

void Tip::draw(sf::RenderTarget &tg, sf::Vector2f pos)
{
	sprite.setPosition(pos);
	text.setPosition(pos);
	text.move(50, 1);
	tg.draw(sprite);
	tg.draw(text);
}

XPBar::XPBar()
{
	portrait.setTexture(TextureManager::instance().getTexture("assets/portrait.png"));
	xpbar.setTexture(TextureManager::instance().getTexture("assets/xpbar.png"));
	xpbar.move(110, 0);
}

void XPBar::draw(sf::RenderTarget &tg)
{
	tg.draw(portrait);
	tg.draw(xpbar);
}