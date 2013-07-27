#ifndef GAMEDATA_INCLUDE
#define GAMEDATA_INCLUDE

#include "d_objects.h"
#include "g_monsters.h"

class HeroStats : BasicStats
{
private:
	//Just additional variable for heroes
	unsigned int social;
	unsigned int mp;

public:
	//Bla-bla-bla
	unsigned int &getStrength() {return strength;};
	unsigned int &getAgility() {return agility;};
	unsigned int &getIntelligence() {return intelligence;};
	unsigned int &getHP() {return hp;};
	unsigned int &getMP() {return mp;};
	unsigned int &getSocial() {return social;};
	void setStrength(unsigned int newStrength) {if (newStrength >= 0) strength = newStrength;};
	void setAgility(unsigned int newAgility) {if (newAgility >= 0) agility = newAgility;};
	void setIntelligence(unsigned int newIntelligence) {if (newIntelligence >= 0) intelligence = newIntelligence;};
	void setHP(unsigned int newHP) {if (newHP >= 0) hp = newHP;};
	void setMP(unsigned int newMP) {if (newMP >= 0) mp = newMP;};
	void setSocial(unsigned int newSocial) {if (newSocial >= 0) social = newSocial;};

	void setStats(HeroStats stats)
	{
		setStrength(stats.getStrength() >= 0 ? stats.getStrength() : 0);
		setAgility(stats.getAgility() >= 0 ? stats.getAgility() : 0);
		setIntelligence(stats.getIntelligence() >= 0 ? stats.getIntelligence() : 0);
		setHP(stats.getHP() >= 0 ? stats.getHP() : 0);
		setSocial(stats.getSocial() >= 0 ? stats.getSocial() : 0);
		setMP(stats.getMP() >= 0 ? stats.getMP() : 0);
	}
};

class GameData
{
public:
	static GameData& instance()
	{
		static GameData theSingleInstance;
		return theSingleInstance;
	}

private:
	GameData();
	GameData( const GameData& );
	GameData& operator =( const GameData& );

private:
	HeroStats player;
	HeroStats ember;
	HeroStats thunder;

public:

	HeroStats &getPlayer() {return player;};
	HeroStats &getEmber() {return ember;};
	HeroStats &getThunder() {return thunder;};
};

#endif