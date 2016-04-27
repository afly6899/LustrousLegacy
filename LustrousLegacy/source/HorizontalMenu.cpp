#include "HorizontalMenu.h"
#include <iostream>

HorizontalMenu::HorizontalMenu() : Menu() {}

HorizontalMenu::HorizontalMenu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound & bleep, sf::Texture backgroundTexture)
: Menu(names, start, size, bleep, backgroundTexture) {
	cpos[0] = size.x / numOptions;
	cpos[1] = (size.x / (numOptions*6));
	setPosition(start);
}

HorizontalMenu::~HorizontalMenu() {}

void HorizontalMenu::setPosition(Tile pos)
{
	int yStart = (pos.y + size.y) / 2 - (optionNames.at(0)->getLocalBounds().height / 2);
	if (positions.empty()) {
		for (int i = 0; i < numOptions; i++) {
			positions.push_back(sf::Vector2f(pos.x + i*cpos[0] + cpos[1] + optionNames.at(i)->getLocalBounds().width / 2, yStart));
			optionNames[i]->setPosition(positions[i]);
		}
	}
	else if (pos != positions.at(0)) {
		for (int i = 0; i < numOptions; i++) {
			positions.push_back(sf::Vector2f(pos.x + i*cpos[0] + cpos[1], yStart));
			optionNames[i]->setPosition(positions[i]);
		}
	}
}

void HorizontalMenu::setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite * background, sf::Sound * bleep)
{
	Menu::setInfo(names, start, size, background, bleep);
	cpos[0] = size.x / numOptions;
	cpos[1] = (size.x / (numOptions * 6));
	setPosition(start);
}
