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

		Textbox(std::map<std::string, sf::Sprite>& faceMap, const sf::Font& font, sf::Sound& bleep, int width_box, int height_box, bool block = false, int font_size = 20, int padding = 25);
		virtual ~Textbox();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void setPosition(const sf::Vector2f position);
		void message(std::string to_display, std::string name = "", float elapsedTime = 0);
		void setSpeed(int speed);
		void setFontSize(int size);
		bool if_endMessage();
		void reset();

	private:

		std::map<std::string, sf::Sprite>& faceMap;
		sf::Sprite faceSprite;
		sf::Sprite faceSprite2;
		sf::Text actorName;
		sf::Text displayText;
		sf::RectangleShape rectText;
		std::string temp_string = "";
		std::string previous_name = "";
		sf::Sound& bleep;
		sf::Texture faceTemp;
		bool displayingText = false;
		bool newActor = false;
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