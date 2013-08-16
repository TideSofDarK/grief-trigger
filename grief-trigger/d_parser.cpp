#include "d_parser.h"

Parser::Parser()
{
	monstersDoc.load_file("assets/monsters.xml");
	dialoguesDoc.load_file("assets/text.xml");
	resourcesDoc.load_file("assets/resources.xml");
	spellsDoc.load_file("assets/spells.xml");
	scenesDoc.load_file("assets/scenes.xml");
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

	size_t pos = 0;
	std::string token;
	while ((pos = squad.find(';')) != std::string::npos) {
		token = squad.substr(0, pos);
		monsters.push_back(token);
		squad.erase(0, pos + 1);
	}

	return monsters;
}

std::vector<std::wstring> Parser::parseAnswers(std::string name, std::string situation)
{
	std::cout << situation;
	std::cout << "\n";

	pugi::xml_node text = dialoguesDoc.child("text").child(name.c_str());

	std::vector<std::string> situations;
	std::vector<std::wstring> strings;

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

	//std::cout << "\n";
	//std::wcout << text.child_value("answers");
	//std::cout << "\n";

	std::wstring newAnswers = pugi::as_wide(text.child_value("answers"));
	std::cout << text.child_value("answers");
	std::cout << "\n";

	if (newAnswers != L"")
	{
		//Remove first newline
		pos = 0; 
		pos = newAnswers.find (L"\n",pos);
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

std::wstring Parser::parseDialogue(std::string name, std::string situation)
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

	return pugi::as_wide(str);
}

std::vector<std::string> Parser::parseResources(std::string block)
{	
	std::string resourcesList = resourcesDoc.child(block.c_str()).child_value();

	//Remove first newline
	std::string::size_type pos = 0; 
	pos = resourcesList.find ("\n",pos);
	resourcesList.erase ( pos, 1 );

	//Count resources
	int s = std::count(resourcesList.begin(), resourcesList.end(), '\n');

	std::vector<std::string> resourcesNames;

	//Add resources
	pos = 0;
	std::string token;
	while ((pos = resourcesList.find('\n')) != std::string::npos) {
		resourcesNames.push_back(resourcesList.substr(0, pos));
		resourcesList.erase(0, pos + 1);
	}

	return resourcesNames;
}

std::string Parser::getBackground(std::string spellName)
{
	//std::cout << spellName << std::endl;
	pugi::xml_node spellsList = spellsDoc;

	for (pugi::xml_node hero: spellsList.children())
	{
		//std::cout << hero.name() << std::endl;
		for (pugi::xml_node spell: hero.children())
		{
			//std::cout << spell.name() << std::endl;
			if (spell.name() == spellName)
			{
				//std::cout << "sled" << std::endl;
				return spell.attribute("back").as_string();
			}
		}
	}
	return "fire";
}

std::vector<Spell> Parser::parseSpells(std::string hero, unsigned int level)
{
	pugi::xml_node spellsList = spellsDoc.child(hero.c_str());

	std::vector<Spell> newSpells;

	for (pugi::xml_node spell: spellsList.children())
	{
		for (pugi::xml_attribute attr: spell.attributes())
		{
			std::string shit = attr.name();
			unsigned int shitInt = attr.as_int();

			if (shit == "level")
			{
				for (pugi::xml_node child: spell.children())
				{
					Spell newSpell;
					std::wstring str = pugi::as_wide(attr.next_attribute().value());
					newSpell.init(spell.name(), str, child.value(), attr.next_attribute().previous_attribute().previous_attribute().as_int());
					newSpells.push_back(newSpell);
				}
			}
		}
	}

	return newSpells;
}

std::string Parser::getSpellType(std::string spelln)
{
	for (pugi::xml_node hero: spellsDoc.children())
	{
		for (pugi::xml_node spell: hero.children())
		{
			if (spell.name() == spelln)
			{
				return spell.attribute("type").as_string();
			}
		}
	}
	return "";
}


bool Parser::nextScene(unsigned int day, unsigned int scene)
{
	pugi::xml_node sceneNode = scenesDoc.child(("day" + std::to_string(day)).c_str()).child(("scene" + std::to_string(scene)).c_str());

	for (pugi::xml_attribute attr: sceneNode.attributes())
	{
		std::string test = " ";
		//std::cout << attr.name() + test + attr.value() << std::endl;
	}
	//std::cout << sceneNode.next_sibling().name() << std::endl;
	if (sceneNode.next_sibling() != NULL)
	{
		//std::cout << "sled" << std::endl;
		return true;
	}
	return false;
}

SceneInfo Parser::getSceneInfo(unsigned int day, unsigned int scene)
{
	pugi::xml_node sceneNode = scenesDoc.child(("day" + std::to_string(day)).c_str()).child(("scene" + std::to_string(scene)).c_str());

	SceneInfo si;
	si.map = sceneNode.attribute("map").as_string();
	si.type = sceneNode.attribute("type").as_string();
	return si;
}

std::string Parser::getName(std::string name, std::string situation)
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

	return text.attribute("name").as_string();
}

bool Parser::isLast(std::string name, std::string situation)
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

	//std::cout << text.attribute("last").as_bool() << std::endl;
	return text.attribute("last").as_bool();
}

std::string Parser::getMusic(unsigned int day, unsigned int scene)
{
	pugi::xml_node sceneNode = scenesDoc.child(("day" + std::to_string(day)).c_str()).child(("scene" + std::to_string(scene)).c_str());

	return sceneNode.attribute("music").as_string();
}

std::string Parser::goNext(std::string name, std::string situation)
{
	std::string realSit = situation;
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
		if (i < situations.size())
		{
			text = text.child(situations[i].c_str());
		}		
	}

	if (text.child_value(text.name()) != "" && text.child_value(text.name()) != "answers")
	{
		realSit += text.name();
		return realSit;
	}
	else return "";	
}

std::string Parser::getPortrait(std::string name, std::string situation)
{
	std::string realSit = situation;
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
		if (i < situations.size())
		{
			text = text.child(situations[i].c_str());
		}		
	}

	return text.attribute("name").as_string();
}

std::wstring Parser::getDescr(std::string sname)
{
	for (pugi::xml_node hero: spellsDoc.children())
	{
		for (pugi::xml_node spell: hero.children())
		{
			if (spell.name() == sname)
			{
				return pugi::as_wide(spell.attribute("descr").value());
			}
		}
	}
	return L"";
}

unsigned int Parser::getSpellLevel(std::string sname)
{
	for (pugi::xml_node hero: spellsDoc.children())
	{
		for (pugi::xml_node spell: hero.children())
		{
			if (spell.name() == sname)
			{
				return spell.attribute("level").as_int();
			}
		}
	}
	return 0;
}
