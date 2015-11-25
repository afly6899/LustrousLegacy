#include "pause.h"

// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
Pause::Pause(const sf::Texture& imagePath_title, const sf::Texture& imagePath_bgtitle, const sf::Texture& imagePath_cursor, const sf::Font& font, sf::Sound& bleep) :
	titleSprite(imagePath_title),
	bgtitleSprite(imagePath_bgtitle),
	cursorSprite(imagePath_cursor),
	cursorSource(0, 0),
	cursorBleep(bleep) {

	textSelections.setFont(font);
	textSelections.setString(" Play Game\n\nLoad Game\n\n   Settings\n\n Exit Game");
	textSelections.setCharacterSize(20);
	textSelections.setPosition(350, 350);
	titleSprite.setPosition(275, 100);
	cursorSprite.setOrigin(8, 8);
	cursorSprite.setPosition(400, 386);
	originalPos = cursorSprite.getPosition();
}

// Player virtual destructor;
Pause::~Pause() {

}

// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
void Pause::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(bgtitleSprite, states);
	target.draw(textSelections, states);
	target.draw(titleSprite, states);
	target.draw(cursorSprite, states);
}

// Player move function moves sprite and animates based on clock and speed
void Pause::move(int num_of_selections, int up_or_down) {
	if (up_or_down == 0)
	{
		selection += 1;
		if (selection > num_of_selections) {
			cursorSprite.setPosition(originalPos);
			selection = 1;
		}
		else
			cursorSprite.setPosition(cursorSprite.getPosition().x, cursorSprite.getPosition().y + 46);
		cursorBleep.play();
	}
	else {
		selection -= 1;
		if (selection < 1) {
			selection = num_of_selections;
			cursorSprite.setPosition(cursorSprite.getPosition().x, cursorSprite.getPosition().y + 46 * (num_of_selections - 1));
		}
		else
			cursorSprite.setPosition(cursorSprite.getPosition().x, cursorSprite.getPosition().y - 46);
		cursorBleep.play();
	}

}

int Pause::getSelection() {
	return selection;
}

void Pause::setPosition(int x, int y) {
	titleSprite.setPosition(x - 125, y - 200);
	textSelections.setPosition(x - 50, y + 50);
	cursorSprite.setPosition(x, y + 86);
	bgtitleSprite.setPosition(x - 400, y - 300);
	originalPos = cursorSprite.getPosition();
}

void Pause::animate(float elapsedTime) {

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