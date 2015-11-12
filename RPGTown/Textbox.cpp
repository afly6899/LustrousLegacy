#include "textbox.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

	// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
	Textbox::Textbox(const sf::Texture& imagePath, const sf::Font& font, const sf::Vector2f position, int width, int height, int font_size, sf::Sound& bleep) :
		faceSprite(imagePath), bleep(bleep) {
		faceSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
		faceSprite.setScale(1.0f, 1.0f);
		actorName.setFont(font);
		actorName.setColor(sf::Color::White);
		displayText.setFont(font);
		displayText.setCharacterSize(20);
		displayText.setColor(sf::Color::White);
		rectText.setSize(sf::Vector2f(width - 25, height*.3));
		rectText.setOrigin((width - 25)*.5, height*.5);
		rectText.setFillColor(sf::Color::Black);
		rectText.setOutlineColor(sf::Color::White);
		rectText.setOutlineThickness(2);
		bleep.setVolume(15);
		
	}

	// Player virtual destructor;
	Textbox::~Textbox() {

	}

	// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
	void Textbox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(rectText, states);
		target.draw(faceSprite, states);
		target.draw(actorName, states);
		target.draw(displayText, states);
	}

	void Textbox::setPosition(const sf::Vector2f position) {
		faceSprite.setPosition(position.x - 375, position.y + 150);
		rectText.setPosition(position.x, position.y + 410);
		displayText.setPosition(position.x - 225, position.y + 135);
		actorName.setPosition(position.x - 375, position.y + 115);
	}

	void Textbox::message(std::string to_display, std::string name, float elapsedTime)
	{
		if (!displayingText && !end_message) {
			end_length = to_display.length();
			actorName.setString(name);
			displayingText = true;
		}
		if (displayingText)
		{
			aniCounter += elapsedTime;

			if (aniCounter >= aniFrameDuration)
			{
				aniCounter -= aniFrameDuration;
				bleep.play();
				int difference_x;
				int difference_y;
				temp_string += to_display[count];
				length_counter += 1;
				difference_x = length_counter*displayText.getCharacterSize() - (rectText.getLocalBounds().width + faceSprite.getLocalBounds().width * 2 + 25);
				if (difference_x > 0) {
					int space_pos = temp_string.find_last_of(' ');
					std::string temp_word = temp_string.substr(space_pos + 1);
					temp_string = temp_string.substr(0, space_pos);
					temp_string.push_back('\n');
					temp_string.append(temp_word);
					length_counter = temp_word.length();
					lines += 1;
					displayText.setString(temp_string);			
				}
				else {
					displayText.setString(temp_string);
				}
				count += 1;
				difference_y = displayText.getCharacterSize()*lines - (rectText.getLocalBounds().height - 75);
				if (difference_y > 0) {
					temp_string = temp_string.substr(temp_string.find_first_of('\n') + 1);;
					displayText.setString(temp_string);
					lines -= 1;
				}

				if (count == end_length)
				{
					count = 0;
					end_length = 0;
					lines = 0;
					length_counter = 0;
					displayingText = false;
					end_message = true;
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			if (end_message)
			{
				temp_string = "";
				displayText.setString(temp_string);
				end_message = !end_message;
			}
		}
	}

	// setSpeed sets the speed of the text rendering; a larger value results in slower text rendering
	void Textbox::setSpeed(int speed)
	{	
		aniFrameDuration = speed;
	}

	void Textbox::setFontSize(int size) {
		displayText.setCharacterSize(size);
	}