#ifndef TITLE_H_
#define TITLE_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "cursor.h"

// Forward Declaration
namespace sf {

	class Texture;
	class Font;
	class Text;
}

	class Title : public sf::Drawable {
	public:

		Title(const sf::Texture& imagePath_title, const sf::Texture& imagePath_bgtitle, const sf::Texture& imagePath_cursor, const sf::Font& font, sf::Sound& bleep, int window_width, int window_height);
		virtual ~Title();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void change_selection(int num_of_selections, int up_or_down);
		int getSelection();
		void setPosition(sf::Vector2f pos);
		sf::Vector2f Title::getPosition();
		void animate(float elapsedTime);

	private:

		sf::Text play_game, load_game, settings, exit_game;
		sf::Sprite titleSprite, bgtitleSprite;
		sf::Sound& cursorBleep;
		sf::Vector2f originalPos;
		Cursor titleCursor;
		int window_width, window_height;
		int selection = 1;
		int aniCounter = 0;
		int aniFrameDuration = 300;
		int seperation = 50;
	};
#endif