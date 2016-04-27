#include <iostream>
#include "BlockMenu.h"

BlockMenu::BlockMenu(int numCols)
:numCols(numCols) {
}

BlockMenu::BlockMenu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound & bleep, sf::Texture backgroundTexture)
: Menu(names, start, size, bleep, backgroundTexture) {
	numCols = 1;
	cpos[0] = size.x;
	cpos[1] = size.y / numOptions;
	setPosition(start);
}

BlockMenu::~BlockMenu() {}

void BlockMenu::setPosition(Tile pos)
{
	int numRows = numOptions / numCols;
	int heightOffset = (optionNames.at(0)->getLocalBounds().height / 2);
	int counter = 0;
	if (positions.empty()) {
		pos.y += 2 * heightOffset;
		for (int row = 0; row < numRows; row++) {
			for (int col = 0; col < numCols; col++) {
				if (counter + 1 > numOptions) {
					row = numRows;
					break;
				}
				positions.push_back(sf::Vector2f(pos.x + col*cpos[0] + cpos[0] / 6 + optionNames.at(counter)->getLocalBounds().width / 2, (pos.y + row * cpos[1] + cpos[1] / 2 - heightOffset)));
				optionNames[counter]->setPosition(positions[counter]);
				counter++;
			}
		}
	}
	else if (pos != positions.at(0)) {
		pos.y += 2 * heightOffset;
		for (int row = 0; row < numRows; row++) {
			for (int col = 0; col < numCols; col++) {
				if (counter + 1 > numOptions) {
					row = numRows;
					break;
				}
				positions.push_back(sf::Vector2f(pos.x + col*cpos[0] + cpos[0] / 6 + optionNames.at(counter)->getLocalBounds().width / 2, (pos.y + row * cpos[1] + cpos[1] / 2 - heightOffset)));
				optionNames[counter]->setPosition(positions[col]);
				counter++;
			}
		}
	}
}

void BlockMenu::moveCursor(bool downRight, bool vertical, std::string type, int previousSelection)
{
	previousSelection = currentSelection;
	int currentRow = currentSelection / numCols;
	if (vertical) {
		if (downRight) {
			currentSelection = (currentSelection + numCols) % numOptions;
		}
		else {
			if (currentRow == 0) {
				currentSelection = numOptions - (1 + (currentSelection + 1) % numCols);
			}
			else {
				currentSelection -= numCols;
			}
		}
	}
	else {
		
		if (downRight) {
			currentSelection++;
			if (currentSelection % numCols == 0 || currentSelection > numOptions) {
				currentSelection = currentRow * numCols;
			}
		}
		else {
			currentSelection--;
			if (currentSelection % numCols == 1 || currentSelection < 0) {
				currentSelection = currentRow * numCols + 1;
			}
		}
	}
	Menu::moveCursor(downRight, vertical, "BlockMenu", previousSelection);
}

void BlockMenu::setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite * background, sf::Sound * bleep)
{
	Menu::setInfo(names, start, size, background, bleep);
	cpos[0] = size.x / numCols;
	if (names.at(0)->getString() == "...") {
		cpos[1] = size.y / (30 / numCols);
		names[0]->setString(" ");
	}
	else {
		cpos[1] = size.y / std::max(1, numOptions / numCols);
	}
	setPosition(start);
}
