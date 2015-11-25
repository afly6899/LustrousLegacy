#include "NPC.h"

// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
NPC::NPC(const sf::Texture& imagePath_object) :
	objSprite(imagePath_object),
	objSource(0, 0) {
	objSprite.setOrigin(32, 32);
	objSprite.setTextureRect(sf::IntRect(objSource.x * 64, objSource.y * 64, 64, 64));
}

// Player virtual destructor;
NPC::~NPC() {

}

sf::Vector2f NPC::getPosition() {
	return objSprite.getPosition();
}
// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
void NPC::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(objSprite, states);
}

void NPC::setPosition(sf::Vector2f pos) {
	objSprite.setPosition(pos);
}

void NPC::hover(float elapsedTime) {

	aniCounter += elapsedTime;

	if (aniCounter >= aniFrameDuration) {
		aniCounter -= aniFrameDuration;
		objSource.y++;
		if (objSource.y > 3)
			objSource.y = 0;
		objSprite.setTextureRect(sf::IntRect(objSource.x * 64, objSource.y * 64, 64, 64));
	}
}