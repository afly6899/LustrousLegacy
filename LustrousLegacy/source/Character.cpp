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
\brief temp
*********************************************************************/
std::string Character::getClass() {
	return "Character";
}

/*********************************************************************
\brief temp
*********************************************************************/
void Character::collided() {
	resetTextureRect();
	setPosition(getPastPosition());
}

/*********************************************************************
\brief temp
*********************************************************************/
sf::Vector2f Character::getViewArm() {
	return sf::Vector2f(getPosition().x, getPosition().y - 32);
}
