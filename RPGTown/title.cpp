#include "title.h"

	// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
	Title::Title(const sf::Texture& imagePath_title, const sf::Texture& imagePath_cursor, const sf::Font& font, sf::Sound& bleep):
		titleSprite(imagePath_title), 
		cursorSprite(imagePath_cursor),
		cursorSource(0,0), 
		cursorBleep(bleep) {
		
		textTitle.setFont(font);
		textTitle.setCharacterSize(52);
		textTitle.setColor(sf::Color(255, 255, 255, 255));
		textTitle.setString("Lustrous Legacy\n    (RPGTown)");
		textSelections.setFont(font);
		textSelections.setString(" Play Game\n\nLoad Game\n\n   Settings\n\n Exit Game");
		textSelections.setCharacterSize(20);
		textSelections.setPosition(350, 350);
		textTitle.setPosition(200, 150);
		titleSprite.setOrigin(0, 0);
		cursorSprite.setOrigin(8, 8);
		cursorSprite.setPosition(400, 386);
		originalPos = cursorSprite.getPosition();
	}

	// Player virtual destructor;
	Title::~Title() {

	}

	// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
	void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(titleSprite, states);
		target.draw(textSelections, states);
		target.draw(textTitle, states);
		target.draw(cursorSprite, states);
	}

	// Player move function moves sprite and animates based on clock and speed
	void Title::move(int num_of_selections, int up_or_down) {
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

	int Title::getSelection() {
		return selection;
	}

	void Title::setPosition(int x, int y) {
		textTitle.setPosition(x - 200, y - 150);
		textSelections.setPosition(x - 50, y + 50);
		cursorSprite.setPosition(x, y + 86);
		titleSprite.setPosition(x - 400, y - 300);
		originalPos = cursorSprite.getPosition();
	}

	void Title::animate(float elapsedTime) {

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