#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Forward Declaration
namespace sf {

	class Texture;
	class Font;
	class Text;
}

	class Textbox : public sf::Drawable {
	public:

		// Player default constructure, requires a playerTexture reference
		Textbox(const sf::Font& font, sf::Sound& bleep, const sf::Texture& imagePath, int width_box, int height_box, bool block = false, int font_size = 20, int padding = 25);
		// Player destructor (virtual -> destroy derived, then destroy base class)
		virtual ~Textbox();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// player positioning functions
		void setPosition(const sf::Vector2f position);
		void message(std::string to_display, std::string name = "", float elapsedTime = 0);
		void setSpeed(int speed);
		void setFontSize(int size);
		bool if_endMessage();
		void reset();

	private:

		sf::Sprite faceSprite;
		sf::Text actorName;
		sf::Text displayText;
		sf::RectangleShape rectText;
		std::string temp_string = "";
		sf::Sound& bleep;
		bool displayingText = false;
		bool end_message = false;
		bool block_draw;
		int width;
		int height;
		int padding;
		int count = 0;
		int lines = 0;
		int length_counter = 0;
		int end_length = 0;
		int string_length = 0;
		int aniCounter = 0;
		int aniFrameDuration = 25;
	};
#endif