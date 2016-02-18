#ifndef TITLE_H_
#define TITLE_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Pawn.h"
#include "UI.h"
#include <map>

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
		void change_selection(int up_or_down);
		int getSelection();
		virtual void setPosition(sf::Vector2f pos);
		virtual void update(sf::Vector2f pos, float elapsedTime);
		virtual std::string getClass();
	private:

		sf::Sound& cursorBleep;
		sf::Vector2u window_size;
		sf::Vector2f originalPos;
		sf::Vector2f cursor_originalPos;
		Pawn titleCursor;
		Pawn titleLogo;
		int num_of_selections;
		int selection = 1;
		int aniCounter = 0;
		int aniFrameDuration = 300;
		int seperation = 50;
		std::map<int, sf::Text> title_options;
	};
#endif
