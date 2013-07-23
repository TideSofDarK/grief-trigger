#include "d_parser.h"

Parser::Parser()
{
	monstersDoc.load("assets/monsters.xml");
}

BasicStats Parser::getMonsterStats(std::string name)
{
	pugi::xml_node monster = monstersDoc.child(name.c_str());

	BasicStats newStats;

	newStats.setStrength(atoi(monster.child_value("str")));
	newStats.setIntelligence(atoi(monster.child_value("agi")));
	newStats.setAgility(atoi(monster.child_value("int")));

	return newStats;
}