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
	pugi::xml_document saveDoc;

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
	std::string goNext(std::string name, std::string situation);
	std::string getPortrait(std::string name, std::string situation);
	std::string getSpellType(std::string spell);
	std::wstring getDescr(std::string spell);
	unsigned int getSpellLevel(std::string spell);
	void saveGame();
	void loadGame();
	std::string getMap(unsigned int day, unsigned int scene);
	int getSocialBonus(std::string name, std::string situation);
	int getIntBonus(std::string name, std::string situation);
};

#endif