#include "Character.h"
#include "sfMath.h"

/*********************************************************************
\brief temp
*********************************************************************/
Character::Character(const sf::Texture& imagePath) :
	Actor(imagePath) {
}

/*********************************************************************
\brief temp
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

/*********************************************************************
\brief temp
*********************************************************************/
bool Character::check_Interact(Actor& actor) {
	if (actor.getSprite().getGlobalBounds().intersects(getSprite().getGlobalBounds())) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			unsigned int dir_player_from_actor = sfmath::vecTrueDirection(sfmath::Normalize(getPosition() - actor.getPosition()));
			if (dir_player_from_actor != getDirection()) {
				actor.faceActor(*this);
				actor.collided();
				resetTextureRect();
				return true;
				}
			}
		}
	return false;
}