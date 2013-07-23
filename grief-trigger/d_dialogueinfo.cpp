#include "d_dialogueinfo.h"

DialogueInfo::DialogueInfo()
{
}

void DialogueInfo::init(std::string file)
{
	this->file = file;
	pugi::xml_parse_result result = doc.load_file(("assets/" + file).c_str());
}

std::string DialogueInfo::getDefaultDialogue(std::string name)
{
	pugi::xml_node text = doc.child("text");

	return text.child(name.c_str()).child_value("default");
}

std::string DialogueInfo::getDialogue(std::string name, std::string situation)
{
	pugi::xml_node text = doc.child("text").child(name.c_str());

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

std::string DialogueInfo::getAnswers(std::string name, std::string situation)
{
	pugi::xml_node text = doc.child("text").child(name.c_str());

	std::vector<std::string> situations;

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

	return text.child_value("answers");
}