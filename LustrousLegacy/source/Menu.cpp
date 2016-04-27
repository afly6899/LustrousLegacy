#include <iostream>
#include "Menu.h"

Menu::Menu() {
}

Menu::Menu(int numRows)
{
}

Menu::Menu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound &bleep, sf::Texture backgroundTexture)
: numOptions(names.size()), currentSelection(0), size(size) {
	cursorBleep = new sf::Sound(bleep);
	background = new sf::Sprite(backgroundTexture);
	background->setTextureRect(sf::IntRect(start.x, start.y, size.x, size.y));
	background->setPosition(start);
	cpos[0] = 0.0; cpos[1] = 0.0;
}

Menu::~Menu() {
	if (optionNames.size() > 0) {
		delete background;
		//delete cursorBleep;
		for (int i = 0; i < optionNames.size(); i++) {
			delete optionNames[i];
		}
	}
}

void Menu::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(*background, states);
	for (int i = 0; i < numOptions; i++) {
		target.draw(*(optionNames.at(i)),states);
	}
}

void Menu::update(sf::Vector2f position, float elapsedTime)
{
	setPosition(position);
}

void Menu::resetCursor()
{
	if (currentSelection == 0) {
		toggleStyle(*optionNames[currentSelection]);
	}
	else {
		int previousSelection = currentSelection;
		toggleStyle(*optionNames[currentSelection], *optionNames[previousSelection]);
		currentSelection = 0;
	}
}

void Menu::moveCursor(bool downRight, bool vertical, std::string type, int previousSelection)
{
	if (type == "Menu") {
		previousSelection = currentSelection;
		if (downRight) {
			currentSelection = (currentSelection + 1) % numOptions;
		}
		else {
			if (currentSelection == 0) {
				currentSelection = numOptions - 1;
			}
			else {
				currentSelection--;
			}
		}
	}
	toggleStyle(*optionNames[currentSelection], *optionNames[previousSelection]);
	cursorBleep->play();
}

std::string Menu::getSelected()
{
	return optionNames.at(currentSelection)->getString();
}

sf::Text * Menu::getSelectedText()
{
	return optionNames.at(currentSelection);
}

void Menu::setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite *background, sf::Sound *bleep)
{
	this->size = size;
	numOptions = names.size();
	currentSelection = 0;
	cursorBleep = bleep;
	this->background = background;
	this->background->setTextureRect(sf::IntRect(start.x, start.y, size.x, size.y));
	this->background->setPosition(start);
	optionNames = names;
	cpos[0] = 0.0; cpos[1] = 0.0;
}



