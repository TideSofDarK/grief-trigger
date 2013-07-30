#include "d_gamedata.h"


GameData::GameData(void)
{
	//Init default stats
	HeroStats stats;

	stats.setSocial(5);

	stats.setMP(6);
	stats.setHP(15);
	stats.setStrength(4);
	stats.setIntelligence(6);
	stats.setAgility(8);

	player.setStats(stats);

	stats.setMP(9);
	stats.setHP(13);
	stats.setStrength(5);
	stats.setIntelligence(10);
	stats.setAgility(4);

	ember.setStats(stats);

	stats.setMP(6);
	stats.setHP(15);
	stats.setStrength(9);
	stats.setIntelligence(5);
	stats.setAgility(6);

	thunder.setStats(stats);
}