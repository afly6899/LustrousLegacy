#include "cursor.h"

// Cursor default constructor
Cursor::Cursor(const sf::Texture& imagePath_cursor) :
	cursorSprite(imagePath_cursor),
	cursorSource(0, 0){
	cursorSprite.setOrigin(8, 8);
}

// Cursor virtual destructor
Cursor::~Cursor() {

}

sf::Vector2f Cursor::getPosition() {
	return cursorSprite.getPosition();
}

void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(cursorSprite, states);
}

void Cursor::setPosition(sf::Vector2f pos) {
	cursorSprite.setPosition(pos);
}

void Cursor::animate(float elapsedTime) {

	aniCounter += elapsedTime;

	if (aniCounter >= aniFrameDuration)
	{
		aniCounter -= aniFrameDuration;
		cursorSource.y++;

		if (cursorSource.y * 16 >= (int)cursorSprite.getTexture()->getSize().y) {
			cursorSource.y = 0;
		}
		cursorSprite.setTextureRect(sf::IntRect(cursorSource.x, cursorSource.y * 16, 16, 16));
	}
}