#pragma once
#include <initializer_list>
#include <SFML/Audio.hpp>
#include <vector>
#include "Pawn.h"
class Cursor : public Pawn {
public:
	Cursor(sf::Texture &texture, sf::Sound &cursorBleep, sf::Vector2f startPos, int numOptions, int separation = 50, bool rightleft = true);
	void moveToNext(bool rightOrdown);
	std::string giveOption();
private:
	sf::Sound& bleep;
	int separation;
	int numOptions;
	int currentPosition;
	std::vector<sf::Vector2f> positions;
};