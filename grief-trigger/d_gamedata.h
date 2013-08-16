#ifndef GAMEDATA_INCLUDE
#define GAMEDATA_INCLUDE

#include "d_objects.h"
#include "g_monsters.h"
#include "g_battle.h"

class Level
{
public:
	static Level& instance()
	{
		static Level theSingleInstance;
		return theSingleInstance;
	}

private:
	Level()
	{
		day = 1;
		scene = 1;
	}
	Level( const Level& );
	Level& operator =( const Level& );
public:
	
	void nextScene()
	{
		std::cout << std::to_string(day) << std::endl;
		std::cout << std::to_string(scene) << std::endl;
		bool next = Parser::instance().nextScene(day,scene);
		if (next == false)
		{
			day++;
			scene=1;
		}
		else
		{
			scene++;
		}	
		std::cout << std::to_string(day) << std::endl;
		std::cout << std::to_string(scene) << std::endl;
	}
	unsigned int getDay(){return day;};
	unsigned int getScene(){return scene;};
private:
	unsigned int day;
	unsigned int scene;
};

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
	unsigned int maxHP;
	unsigned int maxMP;
	unsigned int nextLevelXP;

	unsigned int hpOffset;
	unsigned int mpOffset;
	unsigned int strOffset;
	unsigned int intOffset;
	unsigned int agiOffset;
	unsigned int levelOffset;

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
	unsigned int &getMaxMP() {return maxMP;};
	unsigned int &getMaxHP() {return maxHP;};
	unsigned int &getSocial() {return social;};
	void setStrength(unsigned int newStrength) { strength = newStrength;};
	void setAgility(unsigned int newAgility) { agility = newAgility;};
	void setIntelligence(unsigned int newIntelligence) { intelligence = newIntelligence;};
	void setHP(int newHP)
	{ 
		hp = newHP; 
		if (newHP < 0) 
		{hp = 0; }
		if (hp > maxHP && maxHP > 0) {hp = maxHP;}
	};
	void addHP(unsigned int newHP)
	{
		hp+=newHP;
		if (hp > maxHP) {hp = maxHP;}
	}
	void setMaxHP(unsigned int newMaxHP)
	{ 
		maxHP = newMaxHP;
	};
	void setMaxMP(unsigned int newMaxMP)
	{ 
		maxMP = newMaxMP;
	};
	void setMP(unsigned int newMP) {if (newMP >= 0) mp = newMP;};
	void setSocial(unsigned int newSocial) { social = newSocial;};
	unsigned int &getStrengthOffset() {return strOffset;};
	unsigned int &getAgilityOffset() {return agiOffset;};
	unsigned int &getIntelligenceOffset() {return intOffset;};
	unsigned int &getHPOffset() {return hpOffset;};
	unsigned int &getMPOffset() {return mpOffset;};
	unsigned int &getLevelOffset() {return levelOffset;};

	void levelUp() 
	{
		hpOffset = hp;
		mpOffset = mp;
		strOffset = strength;
		intOffset = intelligence;
		agiOffset = agility;
		levelOffset = level;

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

		hpOffset = hp - hpOffset;
		mpOffset = mp - mpOffset;
		strOffset = strength - strOffset;
		intOffset = intelligence - intOffset;
		agiOffset = agility - agiOffset;
		levelOffset = level - levelOffset;
	};
	void addXP(unsigned int newXP) 
	{
		std::cout << "new xp = " + std::to_string(newXP) << std::endl;
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
		}
		else
		{
			hp -= damage;
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
		setMaxHP(stats.maxHP);
		setMaxMP(stats.maxMP);
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

	std::wstring wordWrap3( std::wstring str, size_t width = 20 ) {
		size_t curWidth = width;
		while( curWidth < str.length() ) {
			std::string::size_type spacePos = str.rfind( ' ', curWidth );
			if( spacePos == std::string::npos )
				spacePos = str.find( ' ', curWidth );
			if( spacePos != std::string::npos ) {
				str[ spacePos ] = '\n';
				curWidth = spacePos + width + 1;
			}
		}

		return str;
	}
};

#endif