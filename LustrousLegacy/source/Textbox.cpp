#include <iostream>
#include "Textbox.h"

	/*********************************************************************
	\brief temp
	*********************************************************************/
	Textbox::Textbox(std::map<std::string, sf::Sprite>& faceMap, const sf::Font& font, sf::Sound& bleep, sf::Vector2f window_size, bool block, int font_size, int padding) :
		faceMap(faceMap), window_size(window_size), bleep(bleep), block_draw(block), padding(padding) {
		
		displayText.setFont(font);
		displayText.setCharacterSize(font_size);
		displayText.setColor(sf::Color::White);
		
		bleep.setPitch(2);
		if (!block_draw)
		{
			rectText.setSize(sf::Vector2f(window_size.x - padding, window_size.y*.3));
			rectText.setOrigin((window_size.x - padding)*.5, window_size.y*.5);
			rectText.setFillColor(sf::Color::Black);
			rectText.setOutlineColor(sf::Color::White);
			rectText.setOutlineThickness(2);
			actorName.setColor(sf::Color::Yellow);
			actorName.setFont(font);
		}
		else {
			rectText.setSize(window_size);
			rectText.setOrigin(window_size.x*.5, window_size.y*.5);
		}	
	}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	Textbox::~Textbox() {}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	void Textbox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		
		if (!block_draw)
		{
			target.draw(faceMap[actorName.getString()], states);
			target.draw(rectText, states);
			target.draw(actorName, states);
		}
		target.draw(displayText, states);
	}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	void Textbox::setPosition(const sf::Vector2f position) {
		if (!block_draw)
		{
			rectText.setPosition(position.x, position.y + 410);
			if (actorName.getString() == "Warren")
				faceMap[actorName.getString()].setPosition(position.x - 200, position.y - 84);
			else
				faceMap[actorName.getString()].setPosition(position.x + 200, position.y - 84);
			rectText.setPosition(position.x, position.y + 410);
			displayText.setPosition(position.x - 350, position.y + 150);
			actorName.setPosition(position.x - 350, position.y + 115);
		}
		else {
			displayText.setPosition(position.x - 350, position.y + 135);
		}	
	}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	void Textbox::message(std::string to_display, std::string name, float elapsedTime)
	{
		if (!processingText && !end_message) {
			end_length = to_display.length();
			if (name.substr(0, 3) == "NPC") {
				name = name.substr(3);
			}
			actorName.setString((name == "System")? " " : name);
			processingText = true;
		}
		if (processingText)
		{
			aniCounter += elapsedTime;

			if (aniCounter >= aniFrameDuration)
			{
				aniCounter -= aniFrameDuration;
				if (bleep.getStatus() == sf::Sound::Stopped)
					bleep.play();
				
				int difference_x;
				int difference_y;
				temp_string += to_display[count];
				length_counter += 1;

				difference_x = length_counter*displayText.getCharacterSize() - (window_size.x*2 - (padding*3)*4);
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

				difference_y = displayText.getCharacterSize()*lines - (rectText.getLocalBounds().height - 100);
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
					processingText = false;
					end_message = true;
				}
			}
		}
	}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	bool Textbox::if_endMessage(bool& ENTER_KEY) {
		if (ENTER_KEY) {
			return end_message;
		}
		return false;
	}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	void Textbox::reset() {
		end_message = false;
		temp_string = "";
		displayText.setString(temp_string);
	}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	void Textbox::setSpeed(int frame_duration)
	{	
		aniFrameDuration = frame_duration;
	}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	void Textbox::setFontSize(int size) {
		displayText.setCharacterSize(size);
	}

	/*********************************************************************
	\brief temp
	*********************************************************************/
	bool Textbox::display_message(std::string scene_arr[], Character& player, float elapsedTime, bool& ENTER_KEY) {
		setPosition(player.getViewArm());
		if (reader == nullptr) {
			reader = new SceneReader(scene_arr[0], scene_arr[1]);
		}
		if (!if_endMessage(ENTER_KEY))
			ProcessMessage(reader->currentMessage().second, reader->currentMessage().first, elapsedTime, ENTER_KEY);
		else
		{
			reset();
			if (!reader->isEmpty())
				reader->nextMessage();
			if (reader->isEmpty()) {
				delete reader;
				reader = nullptr;
				return false;
			}
		}
		return true;
	}

	void Textbox::ProcessMessage(std::string to_display, std::string name, float elapsedTime, bool& ENTER_KEY) {

		if (ENTER_KEY) {
			while (!end_message) {
				message(to_display, name, elapsedTime);
			}
		}
		else
			message(to_display, name, elapsedTime);
	}