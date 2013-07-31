#ifndef MAGIC_INCLUDE
#define MAGIC_INCLUDE

#include <string>
#include <iostream>
#include <vector>

#include <SFML/System.hpp>

#define TOP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

class Spell
{
private:
	sf::String name;
	sf::String fileName;
	unsigned int mana;

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