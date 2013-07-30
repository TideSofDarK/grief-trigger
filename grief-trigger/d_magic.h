#ifndef MAGIC_INCLUDE
#define MAGIC_INCLUDE

#include <string>
#include <vector>

#define TOP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

class Spell
{
private:
	std::string name;

	std::vector<int> combo;

public:
	Spell(std::string newName, std::string newCombo);
	std::string getName() {return name;};
	std::vector<int> getCombo() {return combo;};
};

#endif