#include "Character.h"

/*********************************************************************
Player class default constructor.
\brief Requires a texture for instantiation.
*********************************************************************/
Character::Character(const sf::Texture& imagePath) :
	Actor(imagePath) {
}

/*********************************************************************
Player class virtual destructor.
*********************************************************************/
Character::~Character() {
}

/*********************************************************************
draw; inherited from class drawable.
\brief Allows drawable class members to be drawn to the screen.
*********************************************************************/
void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(Pawn(*this), states);
}

PlayerController& Character::getPlayerController() {
	return movement;
}