#ifndef MAGIC_INCLUDE
#define MAGIC_INCLUDE

#include <string>
#include <iostream>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Spell
{
private:
	sf::String name;
	sf::String fileName;
	unsigned int mana;
	unsigned int level;

	std::vector<int> combo;

public:
	Spell();
	void init(std::string newFileName, std::wstring newName, std::string newCombo, unsigned int newMana);
	std::wstring getName() {return name.toWideString();};
	std::string getFileName(){return fileName.toAnsiString();};
	std::vector<int> getCombo() {return combo;};
	unsigned int getMana(){return mana;};
};

#endif