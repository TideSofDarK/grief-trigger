#ifndef GAMEDATA_INCLUDE
#define GAMEDATA_INCLUDE

#include "d_objects.h"
#include "g_monsters.h"
#include "g_battle.h"

class HeroStats : BasicStats
{
private:
	typedef enum { ALIVE, DIED } HERO_STATE;
	HERO_STATE state;

	//Just additional variable for heroes
	unsigned int social;
	unsigned int mp;
	unsigned int level;
	unsigned int xp;
	unsigned int nextLevelXP;

public:
	HeroStats()
	{
		xp = 1;
		nextLevelXP = 100;
		level = 1;
	}
	//Bla-bla-bla
	unsigned int getNextLevelXP(){return nextLevelXP;};
	unsigned int getXP(){return xp;};
	unsigned int &getLevel() {return level;};
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
	void levelUp() 
	{
		//Some crappy magic with social
		level++;
		if (strength > intelligence && strength > agility)
		{
			strength += 3;
			agility += 2;
			if (social > 10)
			{
				intelligence += 2;
			}
		}
		else if (agility > strength && agility > intelligence)
		{
			agility += 3;
			strength += 2;
			if (social > 10)
			{
				intelligence += 2;
			}
		}
		else if (intelligence > strength && intelligence > agility)
		{
			intelligence += 3;
			agility += 2;
			if (social > 10)
			{
				strength += 2;
			}
		}
	};
	void addXP(unsigned int newXP) 
	{
		xp += newXP;
		if (xp >= nextLevelXP)
		{
			nextLevelXP *= 2;
			xp = 2;
			levelUp();
		}
	};
	bool isDied(){return (state == DIED ? true : false);};

	void receiveDamage(unsigned int damage) 
	{ 
		//got fucked with it
		int finally = hp - damage;
		std::cout << std::to_string(finally) << std::endl;
		if (finally <= 0)
		{
			SoundManager::instance().playDieSound();
			hp = 0;
			state = DIED;
			std::cout << "null" << std::endl;
		}
		else
		{
			hp -= damage;
			std::cout << "minus" << std::endl;
		}
	};

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