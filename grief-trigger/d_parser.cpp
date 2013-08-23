#include "d_parser.h"
#include "d_gamedata.H"

#include <zlib.h>

unsigned long file_size(char *filename)
{
	FILE *pFile = fopen(filename, "rb");
	fseek (pFile, 0, SEEK_END);
	unsigned long size = ftell(pFile);
	fclose (pFile);
	return size;
}

int decompress_one_file(char *infilename, char *outfilename)
{
	gzFile infile = gzopen(infilename, "rb");
	FILE *outfile = fopen(outfilename, "wb");
	if (!infile || !outfile) return -1;

	char buffer[128];
	int num_read = 0;
	while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
		fwrite(buffer, 1, num_read, outfile);
	}

	gzclose(infile);
	fclose(outfile);
}

int compress_one_file(char *infilename, char *outfilename)
{
	FILE *infile = fopen(infilename, "rb");
	gzFile outfile = gzopen(outfilename, "wb");
	if (!infile || !outfile) return -1;

	char inbuffer[128];
	int num_read = 0;
	unsigned long total_read = 0, total_wrote = 0;
	while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
		total_read += num_read;
		gzwrite(outfile, inbuffer, num_read);
	}
	fclose(infile);
	gzclose(outfile);

	printf("Read %ld bytes, Wrote %ld bytes,Compression factor %4.2f%%\n",
		total_read, file_size(outfilename),
		(1.0-file_size(outfilename)*1.0/total_read)*100.0);
}

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

int Parser::getSocialBonus(std::string name, std::string situation)
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

	return text.attribute("bonus").as_int();
}

int Parser::getIntBonus(std::string name, std::string situation)
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

	return text.attribute("int").as_int();
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

void Parser::saveGame()
{
	saveDoc.append_child("save");

	pugi::xml_node save = saveDoc.child("save");

	save.append_child("day");
	save.append_child("scene");
	save.append_child("player");
	save.append_child("ember");
	save.append_child("thunder");

	save.append_attribute("money");
	save.append_child("weapons");

	save.append_attribute("meds").set_value(GameData::instance().getMeds());
	save.append_attribute("shards").set_value(GameData::instance().getManas());
	save.append_attribute("res").set_value(GameData::instance().getShards());

	pugi::xml_attribute mattr = save.attribute("money");
	mattr.set_value(GameData::instance().getMoney());

	pugi::xml_node weapons = save.child("weapons");
	weapons.append_attribute("p").set_value(GameData::instance().getPlayer().getWeapon().getLevel());
	weapons.append_attribute("e").set_value(GameData::instance().getThunder().getWeapon().getLevel());
	weapons.append_attribute("t").set_value(GameData::instance().getThunder().getWeapon().getLevel());

	pugi::xml_node day = save.child("day");
	day.append_attribute("value");
	day.attribute("value").set_value(std::to_string(Level::instance().getDay()).c_str());

	pugi::xml_node scene = save.child("scene");
	scene.append_attribute("value");
	scene.attribute("value").set_value(std::to_string(Level::instance().getScene()).c_str());

	pugi::xml_node player = save.child("player");
	player.append_attribute("l").set_value(GameData::instance().getPlayer().getLevel());
	player.append_attribute("hp");
	player.attribute("hp").set_value(GameData::instance().getPlayer().getHP());
	player.append_attribute("maxhp");
	player.attribute("maxhp").set_value(GameData::instance().getPlayer().getMaxHP());
	player.append_attribute("mp");
	player.attribute("mp").set_value(GameData::instance().getPlayer().getMP());
	player.append_attribute("maxmp");
	player.attribute("maxmp").set_value(GameData::instance().getPlayer().getMaxMP());
	player.append_attribute("str");
	player.attribute("str").set_value(GameData::instance().getPlayer().getStrength());
	player.append_attribute("int");
	player.attribute("int").set_value(GameData::instance().getPlayer().getIntelligence());
	player.append_attribute("agi");
	player.attribute("agi").set_value(GameData::instance().getPlayer().getAgility());
	player.append_attribute("xp");
	player.attribute("xp").set_value(GameData::instance().getPlayer().getXP());
	player.append_attribute("maxxp");
	player.attribute("maxxp").set_value(GameData::instance().getPlayer().getNextLevelXP());
	player.append_attribute("social");
	player.attribute("social").set_value(GameData::instance().getPlayer().getSocial());

	pugi::xml_node ember = save.child("ember");
	ember.append_attribute("l").set_value(GameData::instance().getEmber().getLevel());
	ember.append_attribute("hp");
	ember.attribute("hp").set_value(GameData::instance().getEmber().getHP());
	ember.append_attribute("maxhp");
	ember.attribute("maxhp").set_value(GameData::instance().getEmber().getMaxHP());
	ember.append_attribute("mp");
	ember.attribute("mp").set_value(GameData::instance().getEmber().getMP());
	ember.append_attribute("maxmp");
	ember.attribute("maxmp").set_value(GameData::instance().getEmber().getMaxMP());
	ember.append_attribute("str");
	ember.attribute("str").set_value(GameData::instance().getEmber().getStrength());
	ember.append_attribute("int");
	ember.attribute("int").set_value(GameData::instance().getEmber().getIntelligence());
	ember.append_attribute("agi");
	ember.attribute("agi").set_value(GameData::instance().getEmber().getAgility());
	ember.append_attribute("xp");
	ember.attribute("xp").set_value(GameData::instance().getEmber().getXP());
	ember.append_attribute("maxxp");
	ember.attribute("maxxp").set_value(GameData::instance().getEmber().getNextLevelXP());
	ember.append_attribute("social");
	ember.attribute("social").set_value(GameData::instance().getEmber().getSocial());

	pugi::xml_node thunder = save.child("thunder");
	thunder.append_attribute("l").set_value(GameData::instance().getThunder().getLevel());
	thunder.append_attribute("hp");
	thunder.attribute("hp").set_value(GameData::instance().getThunder().getHP());
	thunder.append_attribute("maxhp");
	thunder.attribute("maxhp").set_value(GameData::instance().getThunder().getMaxHP());
	thunder.append_attribute("mp");
	thunder.attribute("mp").set_value(GameData::instance().getThunder().getMP());
	thunder.append_attribute("maxmp");
	thunder.attribute("maxmp").set_value(GameData::instance().getThunder().getMaxMP());
	thunder.append_attribute("str");
	thunder.attribute("str").set_value(GameData::instance().getThunder().getStrength());
	thunder.append_attribute("int");
	thunder.attribute("int").set_value(GameData::instance().getThunder().getIntelligence());
	thunder.append_attribute("agi");
	thunder.attribute("agi").set_value(GameData::instance().getThunder().getAgility());
	thunder.append_attribute("xp");
	thunder.attribute("xp").set_value(GameData::instance().getThunder().getXP());
	thunder.append_attribute("maxxp");
	thunder.attribute("maxxp").set_value(GameData::instance().getThunder().getNextLevelXP());
	thunder.append_attribute("social");
	thunder.attribute("social").set_value(GameData::instance().getThunder().getSocial());

	saveDoc.save_file("save.xml");
	saveDoc.reset();

	compress_one_file("save.xml", "save.dat");
	remove("save.xml");
}

void Parser::loadGame()
{
	decompress_one_file("save.dat", "save.xml");
	saveDoc.load_file("save.xml");

	pugi::xml_node save = saveDoc.child("save");

	GameData::instance().setManas(save.attribute("shards").as_int());
	GameData::instance().setShards(save.attribute("res").as_int());
	GameData::instance().setMeds(save.attribute("meds").as_int());

	Level::instance().setDay(save.child("day").attribute("value").as_uint());
	Level::instance().setScene(save.child("scene").attribute("value").as_uint());

	GameData::instance().addMoney(save.attribute("money").as_int());

	GameData::instance().getPlayer().getWeapon().setLevel(save.child("weapons").attribute("p").as_int());
	GameData::instance().getEmber().getWeapon().setLevel(save.child("weapons").attribute("e").as_int());
	GameData::instance().getThunder().getWeapon().setLevel(save.child("weapons").attribute("t").as_int());

	GameData::instance().getPlayer().setHP(save.child("player").attribute("hp").as_int());
	GameData::instance().getPlayer().setMaxHP(save.child("player").attribute("maxhp").as_int());
	GameData::instance().getPlayer().setMP(save.child("player").attribute("mp").as_int());
	GameData::instance().getPlayer().setMaxMP(save.child("player").attribute("maxmp").as_int());
	GameData::instance().getPlayer().setStrength(save.child("player").attribute("str").as_int());
	GameData::instance().getPlayer().setIntelligence(save.child("player").attribute("int").as_int());
	GameData::instance().getPlayer().setAgility(save.child("player").attribute("agi").as_int());
	GameData::instance().getPlayer().setXP(save.child("player").attribute("xp").as_int());
	GameData::instance().getPlayer().setNextLevelXP(save.child("player").attribute("maxxp").as_int());
	GameData::instance().getPlayer().setSocial(save.child("player").attribute("social").as_int());

	GameData::instance().getEmber().setHP(save.child("ember").attribute("hp").as_int());
	GameData::instance().getEmber().setMaxHP(save.child("ember").attribute("maxhp").as_int());
	GameData::instance().getEmber().setMP(save.child("ember").attribute("mp").as_int());
	GameData::instance().getEmber().setMaxMP(save.child("ember").attribute("maxmp").as_int());
	GameData::instance().getEmber().setStrength(save.child("ember").attribute("str").as_int());
	GameData::instance().getEmber().setIntelligence(save.child("ember").attribute("int").as_int());
	GameData::instance().getEmber().setAgility(save.child("ember").attribute("agi").as_int());
	GameData::instance().getEmber().setXP(save.child("ember").attribute("xp").as_int());
	GameData::instance().getEmber().setNextLevelXP(save.child("ember").attribute("maxxp").as_int());
	GameData::instance().getEmber().setSocial(save.child("ember").attribute("social").as_int());

	GameData::instance().getThunder().setHP(save.child("thunder").attribute("hp").as_int());
	GameData::instance().getThunder().setMaxHP(save.child("thunder").attribute("maxhp").as_int());
	GameData::instance().getThunder().setMP(save.child("thunder").attribute("mp").as_int());
	GameData::instance().getThunder().setMaxMP(save.child("thunder").attribute("maxmp").as_int());
	GameData::instance().getThunder().setStrength(save.child("thunder").attribute("str").as_int());
	GameData::instance().getThunder().setIntelligence(save.child("thunder").attribute("int").as_int());
	GameData::instance().getThunder().setAgility(save.child("thunder").attribute("agi").as_int());
	GameData::instance().getThunder().setXP(save.child("thunder").attribute("xp").as_int());
	GameData::instance().getThunder().setNextLevelXP(save.child("thunder").attribute("maxxp").as_int());
	GameData::instance().getThunder().setSocial(save.child("thunder").attribute("social").as_int());

	GameData::instance().getThunder().setLevel(save.child("thunder").attribute("l").as_int());
	GameData::instance().getEmber().setLevel(save.child("ember").attribute("l").as_int());
	GameData::instance().getPlayer().setLevel(save.child("player").attribute("l").as_int());

	saveDoc.reset();

	remove("save.xml");
}

std::string Parser::getMap(unsigned int day, unsigned int scene)
{
	pugi::xml_node sceneNode = scenesDoc.child(("day" + std::to_string(day)).c_str()).child(("scene" + std::to_string(scene)).c_str());

	return sceneNode.attribute("map").as_string();
}