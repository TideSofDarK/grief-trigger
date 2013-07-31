#include "d_magic.h"

Spell::Spell()
{

}

void Spell::init(std::string newFileName, std::wstring newName, std::string newCombo, unsigned int newMana)
{
	name = newName;
	fileName = newFileName;
	mana = newMana;
	std::cout << newFileName << std::endl;
	std::cout << fileName.toAnsiString() << std::endl;
}