#pragma once
#include <initializer_list>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Pawn.h"

class Cursor : public Pawn {
public:
	Cursor(const sf::Texture& playerTexture);
	void spriteAnimate(float elapsedTime);
};