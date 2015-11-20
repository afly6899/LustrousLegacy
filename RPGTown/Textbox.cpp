#include "textbox.h"
#include "player.h"

	Textbox::Textbox(const sf::Font& font, sf::Sound& bleep, const sf::Texture& imagePath, int width_box, int height_box, bool block, int font_size, int padding) :
		faceSprite(imagePath), width(width_box), height(height_box), bleep(bleep), block_draw(block) {
		
		displayText.setFont(font);
		displayText.setCharacterSize(font_size);
		displayText.setColor(sf::Color::White);
		
		if (!block_draw)
		{
			faceSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
			faceSprite.setScale(1.0f, 1.0f);
			actorName.setFont(font);
			actorName.setColor(sf::Color::White);
			rectText.setSize(sf::Vector2f(width - padding, height*.3));
			rectText.setOrigin((width - padding)*.5, height*.5);
			rectText.setFillColor(sf::Color::Black);
			rectText.setOutlineColor(sf::Color::White);
			rectText.setOutlineThickness(2);
		}
		else {
			rectText.setSize(sf::Vector2f(width, height));
			rectText.setOrigin(width*.5, height*.5);
		}
			
	}

	Textbox::~Textbox() {
	}

	void Textbox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		
		if (!block_draw)
		{
			target.draw(rectText, states);
			target.draw(faceSprite, states);
			target.draw(actorName, states);
		}
		target.draw(displayText, states);
	}

	void Textbox::setPosition(const sf::Vector2f position) {
		if (!block_draw)
		{
			faceSprite.setPosition(position.x - 375, position.y + 150);
			rectText.setPosition(position.x, position.y + 410);
			displayText.setPosition(position.x - 225, position.y + 135);
			actorName.setPosition(position.x - 375, position.y + 115);
		}
		else {
			displayText.setPosition(position.x - 225, position.y + 135);
		}
		
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

				difference_y = displayText.getCharacterSize()*lines - (rectText.getLocalBounds().height - 75);
				if (difference_y > 0) {
					temp_string = temp_string.substr(temp_string.find_first_of('\n') + 1);;
					displayText.setString(temp_string);
					lines -= 1;
				}

				count += 1;
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
	}

	bool Textbox::if_endMessage() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			return end_message;
		}
		return false;
	}

	void Textbox::reset() {
		end_message = false;
		temp_string = "";
		displayText.setString(temp_string);
	}

	// setSpeed sets the speed of the text rendering; a larger value results in slower text rendering in ms
	void Textbox::setSpeed(int frame_duration)
	{	
		aniFrameDuration = frame_duration;
	}

	void Textbox::setFontSize(int size) {
		displayText.setCharacterSize(size);
	}
