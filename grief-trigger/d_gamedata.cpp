#include "d_gamedata.h"


GameData::GameData(void)
{
	//Init default stats
	HeroStats stats;

	stats.setSocial(7);

	stats.setMaxMP(6);
	stats.setMP(6);
	stats.setMaxHP(15);
	stats.setHP(15);
	stats.setStrength(4);
	stats.setIntelligence(6);
	stats.setAgility(8);

	player.setStats(stats);

	stats.setSocial(3);

	stats.setMaxMP(9);
	stats.setMP(9);
	stats.setMaxHP(13);
	stats.setHP(13);
	stats.setStrength(5);
	stats.setIntelligence(10);
	stats.setAgility(4);

	ember.setStats(stats);

	stats.setSocial(9);

	stats.setMaxMP(6);
	stats.setMP(6);
	stats.setMaxHP(15);
	stats.setHP(15);
	stats.setStrength(9);
	stats.setIntelligence(5);
	stats.setAgility(6);

	thunder.setStats(stats);	
}

void Level::nextScene()
{
	if (day == 7 && scene == 3)
	{
		day = scene = 0;
	}
	else if (day == 8 && scene == 3)
	{
		day = scene = 0;
	}
	else
	{
		bool next = Parser::instance().nextScene(day,scene);
		if (next == false)
		{
			if (day == 6)
			{
				if (GameData::instance().getPlayer().getSocial() >= 11)
				{
					day++;
				}
				else
				{
					day+=2;
				}
			}
			else
			{
				day++;
			}

			scene=1;
		}
		else
		{
			scene++;
		}	

		Parser::instance().saveGame();
	}
}