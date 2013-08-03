#include "d_magic.h"

#include "h_config.h"

std::vector<int> parseCombo(std::string newCombo)
{
	std::vector<int> combo;

	while ( newCombo.find ("\n") != std::string::npos )
	{
		newCombo.replace( newCombo.find ("\n"), 1 ,"");
	}

	size_t pos = 0;
	std::string token;
	while ((pos = newCombo.find(' ')) != std::string::npos) {
		token = newCombo.substr(0, pos);
		switch (atoi(token.c_str()))
		{
		case 1:
			combo.push_back(UP);
			break;
		case 2:
			combo.push_back(RIGHT);
			break;
		case 3:
			combo.push_back(DOWN);
			break;
		case 4:
			combo.push_back(LEFT);
			break;
		case 5:
			combo.push_back(_Z);
			break;
		case 6:
			combo.push_back(_X);
			break;
		default:
			break;
		}
		newCombo.erase(0, pos + 1);
	}

	return combo;
}

Spell::Spell()
{

}

void Spell::init(std::string newFileName, std::wstring newName, std::string newCombo, unsigned int newMana)
{
	name = newName;
	fileName = newFileName;
	mana = newMana;

	//std::cout << newFileName << std::endl;
	//std::cout << fileName.toAnsiString() << std::endl;

	combo = parseCombo(newCombo);

	for (auto i = combo.begin(); i != combo.end(); i++)
	{
		int &a = *i;
		std::cout << std::to_string(a) << std::endl;
	}
}