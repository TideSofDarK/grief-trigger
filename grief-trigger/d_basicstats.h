#ifndef BASICSTATS_INCLUDE
#define BASICSTATS_INCLUDE

class BasicStats
{
protected:
	//Strength start variable
	unsigned int strength;

	//Agility start variable
	unsigned int agility;

	//Intelligence start variable
	unsigned int intelligence;

	//HP start variable
	unsigned int hp;

public:
	unsigned int getStrength() {return strength;};
	unsigned int getAgility() {return agility;};
	unsigned int getIntelligence() {return intelligence;};
	unsigned int getHP() {return hp;};
	void setStrength(unsigned int newStrength) {strength = newStrength;};
	void setAgility(unsigned int newAgility) {agility = newAgility;};
	void setIntelligence(unsigned int newIntelligence) {intelligence = newIntelligence;};
	void setHP(unsigned int newHP) {hp = newHP;};

	void setStats(BasicStats stats)
	{
		setStrength(stats.getStrength());
		setAgility(stats.getAgility());
		setIntelligence(stats.getIntelligence());
		setHP(stats.getHP());
	}
};

#endif