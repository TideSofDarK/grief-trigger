#ifndef MAGIC_INCLUDE
#define MAGIC_INCLUDE

#include <string>
#include <iostream>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#define UP sf::Keyboard::Key::Up
#define RIGHT sf::Keyboard::Key::Right
#define DOWN sf::Keyboard::Key::Down
#define LEFT sf::Keyboard::Key::Left
#define _Z sf::Keyboard::Key::Z
#define _X sf::Keyboard::Key::X

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