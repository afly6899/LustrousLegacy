#include "Menu.h"
#include <iostream>

Menu::Menu(GeneralInfo info, OptionNames stroptions, sf::Font& font, sf::Texture &texture, sf::Sound &bleep, bool rightleft)
: windowSize(info.windowSize), separation(info.separation), cursorBleep(bleep), cursor(texture){

	changeBy = (rightleft) ? sf::Vector2f(separation, 0) : sf::Vector2f(0, separation);

	for (int i = 0; i < stroptions.size(); i++) {
		options[i] = createOption(stroptions[i], font);
		positions.push_back(sf::Vector2f(info.startPos.x + changeBy.x * i, info.startPos.y + changeBy.y * i));
		options[i].setPosition(positions[i]);
	}
	currentSelection = 0;
	cursor.setPosition(cursorAdjustment());
}

Menu::~Menu() {
}

void Menu::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(cursor, states);
	for (int i = 0; i < positions.size(); i++) {
		target.draw(options.at(i), states);
	}
}

void Menu::setPosition(sf::Vector2f pos) {
	if (pos != positions[0]) {
		cursor.setPosition(cursorAdjustment());
		for (int i = 0; i < positions.size(); i++) {
			positions[i] = sf::Vector2f(pos.x + changeBy.x * i, pos.y + changeBy.y * i);
			options[i].setPosition(positions[i]);
		}
	}
}

void Menu::update(sf::Vector2f pos, float elapsedTime) {
	setPosition(pos);
	cursor.spriteAnimate(elapsedTime);
}

void Menu::moveCursor(bool downRight) {
	if (downRight) {
		currentSelection = (currentSelection + 1) % positions.size();
	}
	else {
		if (currentSelection == 0) {
			currentSelection = positions.size() -1 ;
		}
		else {
			currentSelection--;
		}
	}
	cursor.setPosition(cursorAdjustment());
	cursorBleep.play();
}

sf::Vector2f Menu::cursorAdjustment()
{
	sf::Vector2f temp = positions[currentSelection];
	temp.x -= 30;
	temp.y += 10;
	return temp;
}

/*int separation;
	int numOptions;
	int currentSelection;
	sf::Vector2f changeBy;
	sf::Sound &cursorBleep;
	sf::Vector2u windowSize;
	std::map<int, sf::Text> options;
	std::vector<sf::Vector2f> positions; // original pos is the first one*/