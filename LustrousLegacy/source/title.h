#ifndef TITLE_H_
#define TITLE_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Pawn.h"
#include "UI.h"

// Forward Declaration
namespace sf {

	class Texture;
	class Font;
	class Text;
}

	class Title : public UI {
	public:

		Title(const sf::Font& font, const sf::Vector2u window_size, const sf::Texture& imagePath_title, sf::Sound& sfx, sf::Music& titleMusic);
		virtual ~Title();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void change_selection(int num_of_selections, int up_or_down);
		int getSelection();
		void setPosition(sf::Vector2f pos);
		void setVisible(bool visibility);;
		bool isVisible();
		sf::Vector2f getPosition();

	private:

		sf::Text play_game, load_game, settings, exit_game;
		sf::Sound& cursorBleep;
		sf::Vector2u window_size;
		sf::Vector2f originalPos;
		Pawn titleCursor;
		Pawn titleLogo;
		int selection = 1;
		int aniCounter = 0;
		int aniFrameDuration = 300;
		int seperation = 50;
		bool is_visible = true;
	};
#endif