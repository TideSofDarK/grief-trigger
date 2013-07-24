#ifndef PARSER_INCLUDE
#define PARSER_INCLUDE

#include <iostream>
#include <string>
#include <vector>

#include <pugixml/pugixml.hpp>

#include "d_basicstats.h"

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

	Parser();
	Parser( const Parser& );
	Parser& operator =( const Parser& );

public:
	BasicStats getMonsterStats(std::string name);
	std::vector<std::string> parseSquad(std::string squad);
	std::vector<std::string> parseAnswers(std::string name, std::string situation);
	std::string parseDialogue(std::string name, std::string situation);
};

#endif