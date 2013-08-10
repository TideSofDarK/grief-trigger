#ifndef PARSER_INCLUDE
#define PARSER_INCLUDE

#include <iostream>
#include <string>
#include <vector>

#include <pugixml/pugixml.hpp>

#include "d_basicstats.h"
#include "d_magic.h"

typedef struct SceneInfo 
{
	std::string map;
	std::string type;
};

class Parser
{
public:
	static Parser& instance()
	{
		static Parser theSingleInstance;
		return theSingleInstance;
	}

private:
	pugi::xml_document monstersDoc;
	pugi::xml_document dialoguesDoc;
	pugi::xml_document resourcesDoc;
	pugi::xml_document spellsDoc;
	pugi::xml_document scenesDoc;

	Parser();
	Parser( const Parser& );
	Parser& operator =( const Parser& );

public:
	BasicStats getMonsterStats(std::string name);
	std::vector<std::string> parseSquad(std::string squad);
	std::vector<std::wstring> parseAnswers(std::string name, std::string situation);
	std::wstring parseDialogue(std::string name, std::string situation);
	std::vector<std::string> parseResources(std::string block);
	std::vector<Spell> parseSpells(std::string hero, unsigned int level);
	bool nextScene(unsigned int day, unsigned int scene);
	SceneInfo getSceneInfo(unsigned int day, unsigned int scene);
	std::string getName(std::string name, std::string situation);
	bool isLast(std::string name, std::string situation);
	std::string getBackground(std::string spellName);
	std::string getMusic(unsigned int day, unsigned int scene);
};

#endif