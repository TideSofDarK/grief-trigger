#ifndef GAMEDATA_INCLUDE
#define GAMEDATA_INCLUDE

#include "MapLoader.h"

#include "d_objects.h"
#include "g_monsters.h"
#include "g_battle.h"

class Weapon
{
private:
	std::wstring name;
	unsigned int level;
public:
	void init(std::wstring nName)
	{
		name = nName;
		level = 1;
	};
	void setLevel(unsigned int nl)
	{
		level = nl; 
	};
	bool upgrade()
	{
		if (level <= 2)
		{
			level++;
			return true;
		}
		else
		{
			return false;
		}
	};
	std::wstring getName()
	{
		return name + std::to_wstring(level);
	}
	int getLevel()
	{
		return level;
	}
};

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
		ml = new tmx::MapLoader("assets/");

		day = 1;
		scene = 1;
	}
	Level( const Level& );
	Level& operator =( const Level& );

	tmx::MapLoader *ml;
public:
	tmx::MapLoader *getML()
	{
		return ml;
	};
	bool load(std::string fn)
	{
		cleanML();
		return ml->Load(fn);
	}
	void cleanML()
	{
		delete ml;
		ml = new tmx::MapLoader("assets/");
	}
	void nextScene();
	unsigned int getDay(){return day;};
	unsigned int getScene(){return scene;};
	void setDay(unsigned int d){day = d;};
	void setScene(unsigned int d){scene = d;};
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

	Weapon weapon;

public:
	HeroStats()
	{
		xp = 1;
		nextLevelXP = 75;
		level = 1;

		clearOffset();
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
	void setLevel(int l)
	{
		level = l;
	}
	Weapon &getWeapon(){return weapon;};
	void setStrength(unsigned int newStrength) { strength = newStrength;};
	void setAgility(unsigned int newAgility) { agility = newAgility;};
	void setIntelligence(unsigned int newIntelligence) { intelligence = newIntelligence;};
	void addSocial(int nS)
	{
		int tSoc = social + nS;

		if (tSoc>=1)
		{
			social = tSoc;
		}
		else
		{
			social = 1;
		}
	};
	void addInt(int nI)
	{
		//intelligence += nI;

		int tInt = intelligence + nI;

		if (tInt>=1)
		{
			intelligence = tInt;
		}
		else
		{
			intelligence = 1;
		}
	};
	void clearOffset()
	{
		hpOffset = 0;
		mpOffset = 0;
		strOffset = 0;
		intOffset = 0;
		agiOffset = 0;
		levelOffset = 0;
	};
	bool useMana(int mCost)
	{
		int mp1 = mp;
		if (mp1 - mCost >= 0)
		{
			mp = mp1 - mCost;
			return true;
		}
		else
		{
			return false;
		}
	};
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
	void addMP(unsigned int newMP)
	{
		mp+=newMP;
		if (mp > maxMP) {mp = maxMP;}
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
	unsigned int getLevelOffset() {return levelOffset;};
	void setNextLevelXP(unsigned int newXP){nextLevelXP = newXP;};
	void setXP(unsigned int newXP){xp = newXP;};

	void levelUp() 
	{
		hpOffset = maxHP;
		mpOffset = maxMP;
		strOffset = strength;
		intOffset = intelligence;
		agiOffset = agility;
		levelOffset = level;

		//Some crappy magic with social
		level++;
		if (strength > intelligence && strength > agility)
		{
			strength += 4;
			agility += 3;
			if (social > 10)
			{
				intelligence += 3;
				maxMP += 5;
			}

			maxHP += 7;		
		}
		else if (agility > strength && agility > intelligence)
		{
			agility += 4;
			strength += 3;
			if (social > 10)
			{
				intelligence += 3;
				maxMP += 5;
			}

			maxHP += 5;	
		}
		else if (intelligence > strength && intelligence > agility)
		{
			intelligence += 4;
			agility += 3;
			if (social > 10)
			{
				strength += 3;

				maxHP += 5;	
			}

			maxHP += 1;	
			maxMP += 6;
		}

		hpOffset = maxHP - hpOffset;
		mpOffset = maxMP - mpOffset;
		strOffset = strength - strOffset;
		intOffset = intelligence - intOffset;
		agiOffset = agility - agiOffset;
		levelOffset = level - levelOffset;

		hp = maxHP;
		mp = maxMP;
	};
	void addXP(unsigned int &newXP) 
	{
		std::cout << "new xp = " + std::to_string(newXP) << std::endl;
		xp += newXP;
		if (xp >= nextLevelXP)
		{
			nextLevelXP *= 2;
			xp = 1;
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
	HeroStats		player;
	HeroStats		ember;
	HeroStats		thunder;

	int	money;

	int meds;
	int shards;
	int manas;

public:

	HeroStats &getPlayer() {return player;};
	HeroStats &getEmber() {return ember;};
	HeroStats &getThunder() {return thunder;};

	void addMana()
	{
		manas++;
	}

	void addShard()
	{
		shards++;
	}

	void addMed()
	{
		meds++;
	}

	bool useMana()
	{
		if (manas>0)
		{
			manas--;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool useShard()
	{
		if (shards>0)
		{
			shards--;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool useMed()
	{
		if (meds>0)
		{
			meds--;
			return true;
		}
		else
		{
			return false;
		}
	}

	int getMeds()
	{
		return meds;
	}

	int getShards()
	{
		return shards;
	}

	int getManas()
	{
		return manas;
	}

	void setMeds(int n)
	{
		meds = n;
	}

	void setShards(int n)
	{
		shards = n;
	}

	void setManas(int n)
	{
		manas = n;
	}

	unsigned int getMoney()
	{
		return money;
	};

	void addMoney(unsigned int m)
	{
		money+=m;
	};

	bool useMoney(int m)
	{
		int m1 = money;
		if (m1 - m >= 0)
		{
			money -=m;
			return true;
		}
		else
		{
			return false;
		}
	};

	void printInfo()
	{
		std::cout << std::endl;
		std::cout << "Player level: " + std::to_string(player.getLevel()) + "\n";
		std::cout << "Ember level: " + std::to_string(ember.getLevel()) + "\n";
		std::cout << "Thunder level: " + std::to_string(thunder.getLevel()) + "\n";
		std::cout << "Social level: " + std::to_string(player.getSocial()) + "\n";
		std::cout << "int level: " + std::to_string(player.getIntelligence()) + "\n";
		std::wcout << std::to_wstring(GameData::instance().getPlayer().getWeapon().getLevel()) + L"\n";
		std::wcout << std::to_wstring(GameData::instance().getEmber().getWeapon().getLevel()) + L"\n";
		std::wcout << std::to_wstring(GameData::instance().getThunder().getWeapon().getLevel()) + L"\n";
	}

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