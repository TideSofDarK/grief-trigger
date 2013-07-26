#include "d_parser.h"

Parser::Parser()
{
	monstersDoc.load_file("assets/monsters.xml");
	dialoguesDoc.load_file("assets/text.xml");
}

BasicStats Parser::getMonsterStats(std::string name)
{
	pugi::xml_node monster = monstersDoc.child(name.c_str());

	BasicStats newStats;

	newStats.setStrength(atoi(monster.child_value("str")));
	newStats.setIntelligence(atoi(monster.child_value("agi")));
	newStats.setAgility(atoi(monster.child_value("int")));
	newStats.setHP(atoi(monster.child_value("hp")));

	return newStats;
}

std::vector<std::string> Parser::parseSquad(std::string squad)
{
	std::vector<std::string> monsters;

	unsigned pos = 0;
	std::string token;
	while ((pos = squad.find(';')) != std::string::npos) {
		monsters.push_back(squad.substr(0, pos));
		squad.erase(0, pos + 1);
	}

	return monsters;
}

std::vector<std::string> Parser::parseAnswers(std::string name, std::string situation)
{
	pugi::xml_node text = dialoguesDoc.child("text").child(name.c_str());

	std::vector<std::string> situations;
	std::vector<std::string> strings;

	int pos = 0;
	std::string token;
	while ((pos = situation.find('/')) != std::string::npos) {
		situations.push_back(situation.substr(0, pos));
		situation.erase(0, pos + 1);
	}

	for (int i = 0; i < situations.size(); i++)
	{
		text = text.child(situations[i].c_str());
	}

	std::string newAnswers = text.child_value("answers");

	if (newAnswers != "")
	{
		//Remove first newline
		pos = 0; 
		pos = newAnswers.find ("\n",pos);
		newAnswers.erase ( pos, 1 );

		//Count answers
		int s = std::count(newAnswers.begin(), newAnswers.end(), '\n');

		//Add answers
		pos = 0;
		while ((pos = newAnswers.find('\n')) != std::string::npos) {
			strings.push_back(newAnswers.substr(0, pos));
			newAnswers.erase(0, pos + 1);
		}	
	}

	return strings;
}

std::string Parser::parseDialogue(std::string name, std::string situation)
{
	pugi::xml_node text = dialoguesDoc.child("text").child(name.c_str());

	std::vector<std::string> situations;

	int pos = 0;
	std::string token;
	while ((pos = situation.find('/')) != std::string::npos) {
		situations.push_back(situation.substr(0, pos));
		situation.erase(0, pos + 1);
	}

	std::string str;
	for (int i = 0; i < situations.size(); i++)
	{
		str = text.child_value(situations[i].c_str());
		text = text.child(situations[i].c_str());
	}

	return str;
}