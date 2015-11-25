#include "title.h"

	// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
	Title::Title(const sf::Texture& imagePath_title, const sf::Texture& imagePath_bgtitle, const sf::Texture& imagePath_cursor, const sf::Font& font, sf::Sound& bleep):
		titleSprite(imagePath_title),
		bgtitleSprite(imagePath_bgtitle), 
		titleCursor(imagePath_cursor),
		cursorBleep(bleep) {
	
		textSelections.setFont(font);
		textSelections.setString(" Play Game\n\nLoad Game\n\n   Settings\n\n Exit Game");
		textSelections.setCharacterSize(20);
		textSelections.setPosition(350, 350);
		titleSprite.setPosition(275, 100);
		titleCursor.setPosition(sf::Vector2f(400, 386));
		originalPos = titleCursor.getPosition();
	}

	// Player virtual destructor;
	Title::~Title() {

	}

	// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
	void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(bgtitleSprite, states);
		target.draw(textSelections, states);
		target.draw(titleSprite, states);
		target.draw(titleCursor, states);
	}

	// Player move function moves sprite and animates based on clock and speed
	void Title::move(int num_of_selections, int up_or_down) {
		if (up_or_down == 0)
		{
			selection += 1;
			if (selection > num_of_selections) {
				titleCursor.setPosition(originalPos);
				selection = 1;
			}
			else
				titleCursor.setPosition(sf::Vector2f(titleCursor.getPosition().x, titleCursor.getPosition().y + 46));
			cursorBleep.play();
		}
		else {
			selection -= 1;
			if (selection < 1) {
				selection = num_of_selections;
				titleCursor.setPosition(sf::Vector2f(titleCursor.getPosition().x, titleCursor.getPosition().y + 46 * (num_of_selections - 1)));
			}
			else
				titleCursor.setPosition(sf::Vector2f(titleCursor.getPosition().x, titleCursor.getPosition().y - 46));
			cursorBleep.play();
		}

	}

	int Title::getSelection() {
		return selection;
	}

	void Title::setPosition(int x, int y) {
		titleSprite.setPosition(x - 125, y - 200);
		textSelections.setPosition(x - 50, y + 50);
		titleCursor.setPosition(sf::Vector2f(x, y + 86));
		bgtitleSprite.setPosition(x - 400, y - 300);
		originalPos = titleCursor.getPosition();
	}

	void Title::animate(float elapsedTime) {

		titleCursor.animate(elapsedTime);
	}