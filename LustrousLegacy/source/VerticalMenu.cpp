#include "VerticalMenu.h"
#include <iostream>

VerticalMenu::VerticalMenu() {}

VerticalMenu::VerticalMenu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound & bleep, sf::Texture backgroundTexture)
: Menu(names, start, size, bleep, backgroundTexture) {
	cpos[0] = (size.y / numOptions);
	cpos[1] = (size.y / (numOptions * 2)) - (optionNames.at(0)->getLocalBounds().height / 2);
	setPosition(start);
}

VerticalMenu::~VerticalMenu() {}

void VerticalMenu::setPosition(Tile pos)
{
	int xStart = pos.x + size.x / 6;
	if (positions.empty()) {
		for (int i = 0; i < numOptions; i++) {
			positions.push_back(sf::Vector2f(xStart + optionNames.at(i)->getLocalBounds().width/2, pos.y + i * cpos[0] + cpos[1]));
			optionNames[i]->setPosition(positions[i]);
		}
	}
	else if (pos != positions.at(0)) {
		for (int i = 0; i < numOptions; i++) {
			positions.push_back(sf::Vector2f(xStart + optionNames.at(i)->getLocalBounds().width / 2, pos.y + i * cpos[0] + cpos[1]));
			optionNames[i]->setPosition(positions[i]);
		}
	}
}

void VerticalMenu::setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite * background, sf::Sound * bleep)
{
	Menu::setInfo(names, start, size, background, bleep);
	cpos[0] = size.y / numOptions;
	cpos[1] = (size.y / (numOptions * 2)) - (optionNames.at(0)->getLocalBounds().height / 2);
	setPosition(start);
}
