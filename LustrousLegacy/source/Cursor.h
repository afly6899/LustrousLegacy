#pragma once
#include "Pawn.h"

class Cursor : public Pawn {
public:
	Cursor(const sf::Texture& playerTexture);
	void spriteAnimate(float elapsedTime);
};