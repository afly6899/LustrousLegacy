#include "Cursor.h"

Cursor::Cursor(sf::Texture &texture, sf::Sound &cursorBleep, sf::Vector2f startPos, int numOptions, int separation, bool rightleft)
: Pawn(texture), bleep(cursorBleep), numOptions(numOptions), separation(separation) {
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
