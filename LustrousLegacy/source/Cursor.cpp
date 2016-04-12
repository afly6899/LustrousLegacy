#include "Cursor.h"

Cursor::Cursor(sf::Texture &texture, sf::Sound &cursorBleep, sf::Vector2f startPos, int numOptions, int separation, bool rightleft)
	: Pawn(texture), bleep(cursorBleep), numOptions(numOptions), separation(separation), rightleft(rightleft) {
	if (rightleft) {
		for (int i = 0; i < numOptions; i++) {
			positions.push_back(sf::Vector2f(startPos.x, startPos.y + separation*i));
		}
	}
	else {
		for (int i = 0; i < numOptions; i++) {
			positions.push_back(sf::Vector2f(startPos.x + separation*i, startPos.y));
		}
	}
}

void Cursor::moveToNext(bool rightOrdown) {
	if (rightOrdown) {
		if (currentPosition == numOptions - 1) {
			currentPosition = 0;
		}
		else {
			currentPosition++;
		}
	}
	else {
		if (currentPosition == 0) {
			currentPosition++;
		}
		else {
			currentPosition = numOptions - 1;
		}
	}
	setPosition(positions[currentPosition]);
}

int Cursor::giveOption() {

	return currentPosition;
}

void Cursor::setPosition(sf::Vector2f position) {
	for (int i = 0; i < numOptions; i++) {
		if (rightleft) {
			positions[i] = sf::Vector2f(position.x, position.y + separation*i);
		}
		else {
			positions[i] = sf::Vector2f(position.x + separation*i, position.y);
		}
	}
}
