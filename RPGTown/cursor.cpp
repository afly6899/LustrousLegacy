#include "cursor.h"

// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
Cursor::Cursor(const sf::Texture& imagePath_cursor) :
	cursorSprite(imagePath_cursor),
	cursorSource(0, 0){
	cursorSprite.setOrigin(8, 8);
}

// Player virtual destructor;
Cursor::~Cursor() {

}

sf::Vector2f Cursor::getPosition() {
	return cursorSprite.getPosition();
}
// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
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