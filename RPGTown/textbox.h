#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// Forward Declaration
namespace sf {

	class Texture;
	class Font;
	class Text;
}

	class Textbox : public sf::Drawable {
	public:

		// Player default constructure, requires a playerTexture reference
		Textbox(const sf::Texture& imagePath, const sf::Font& font, const sf::Vector2f position, int width, int height, int font_size, sf::Sound& bleep);
		// Player destructor (virtual -> destroy derived, then destroy base class)
		virtual ~Textbox();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// player positioning functions
		void setPosition(const sf::Vector2f position);
		void message(std::string to_display, std::string name, float elapsedTime);
		void setSpeed(int speed);
		void setFontSize(int size);

	private:

		sf::Sprite faceSprite;
		sf::Text actorName;
		sf::Text displayText;
		sf::RectangleShape rectText;
		std::string temp_string = "";
		sf::Sound& bleep;
		bool displayingText = false;
		bool end_message = false;
		int count = 0;
		int lines = 0;
		int length_counter = 0;
		int end_length = 0;
		int string_length = 0;
		int aniCounter = 0;
		int aniFrameDuration = 25;
	};

#endif