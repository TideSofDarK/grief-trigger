#include "d_gamedata.h"


GameData::GameData(void)
{
	//Init default stats
	HeroStats stats;

	stats.setSocial(7);

	stats.setMaxHP(6);
	stats.setMP(6);
	stats.setMaxHP(15);
	stats.setHP(15);
	stats.setStrength(4);
	stats.setIntelligence(6);
	stats.setAgility(8);

	player.setStats(stats);

	stats.setSocial(3);

	stats.setMaxHP(9);
	stats.setMP(9);
	stats.setMaxHP(13);
	stats.setHP(13);
	stats.setStrength(5);
	stats.setIntelligence(10);
	stats.setAgility(4);

	ember.setStats(stats);

	stats.setSocial(9);

	stats.setMaxHP(6);
	stats.setMP(6);
	stats.setMaxHP(15);
	stats.setHP(15);
	stats.setStrength(9);
	stats.setIntelligence(5);
	stats.setAgility(6);

	thunder.setStats(stats);	
}