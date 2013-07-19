#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <pugixml/pugixml.hpp>

class DialogueInfo
{
protected:
	std::string file;
public:
	DialogueInfo(std::string file);
	std::string getDefaultDialogue(std::string name);
	std::string getDialogue(std::string name, std::string situation);
	std::string getAnswers(std::string name, std::string situation);
};

