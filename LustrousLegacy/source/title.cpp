#include "title.h"
#include "Enums.h"

Title::Title(const sf::Font& font, const sf::Vector2u window_size, const sf::Texture& imagePath_title, sf::Sound& sfx, sf::Music& titleMusic):
		cursorBleep(sfx),
		window_size(window_size), 
		titleLogo(imagePath_title),
		titleCursor(imagePath_title) {

		play_game.setFont(font);
		play_game.setString("Play Game");
		play_game.setColor(sf::Color::Yellow);
		play_game.setStyle(sf::Text::Italic);
		play_game.setCharacterSize(Font_Size::Default);
		play_game.setOrigin(play_game.getLocalBounds().width*.5, play_game.getLocalBounds().height*.5);
		load_game.setFont(font);
		load_game.setString("Load Game");
		load_game.setCharacterSize(Font_Size::Default);
		load_game.setOrigin(load_game.getLocalBounds().width *.5, load_game.getLocalBounds().height*.5);
		settings.setFont(font);
		settings.setString("Settings");
		settings.setCharacterSize(Font_Size::Default);
		settings.setOrigin(settings.getLocalBounds().width *.5, settings.getLocalBounds().height*.5);
		exit_game.setFont(font);
		exit_game.setString("Exit");
		exit_game.setCharacterSize(Font_Size::Default);
		exit_game.setOrigin(exit_game.getLocalBounds().width *.5, exit_game.getLocalBounds().height*.5);

		play_game.setPosition(100, 100);
		load_game.setPosition(play_game.getPosition().x, play_game.getPosition().y + seperation);
		settings.setPosition(load_game.getPosition().x, load_game.getPosition().y + seperation);
		exit_game.setPosition(settings.getPosition().x, settings.getPosition().y + seperation);
		
		titleCursor.setPosition(sf::Vector2f(play_game.getPosition().x, play_game.getPosition().y + seperation*.5));
		originalPos = sf::Vector2f(titleCursor.getPosition().x, titleCursor.getPosition().y);
	}

	Title::~Title() {

	}

	void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(play_game, states);
		target.draw(load_game, states);
		target.draw(settings, states);
		target.draw(exit_game, states);
	}

	void Title::change_selection(int num_of_selections, int up_or_down) {
		if (up_or_down == Cursor_Direction::Down)
		{
			selection += 1;
			if (selection > num_of_selections) {
				titleCursor.setPosition(originalPos);
				selection = 1;
			}
			else
				titleCursor.setPosition(sf::Vector2f(titleCursor.getPosition().x, titleCursor.getPosition().y + seperation));
			cursorBleep.play();
		}
		else {
			selection -= 1;
			if (selection < 1) {
				selection = num_of_selections;
				titleCursor.setPosition(sf::Vector2f(titleCursor.getPosition().x, titleCursor.getPosition().y + seperation * (num_of_selections - 1)));
			}
			else
				titleCursor.setPosition(sf::Vector2f(titleCursor.getPosition().x, titleCursor.getPosition().y - seperation));
			cursorBleep.play();
		}

		if (selection == Selection::Play_Game) {
			play_game.setColor(sf::Color::Yellow);
			play_game.setStyle(sf::Text::Italic);
		}
		else {
			play_game.setColor(sf::Color::White);
			play_game.setStyle(0);
		}

		if (selection == Selection::Load_Game) {
			load_game.setColor(sf::Color::Yellow);
			load_game.setStyle(sf::Text::Italic);
		}
		else {
			load_game.setColor(sf::Color::White);
			load_game.setStyle(0);
		}

		if (selection == Selection::Settings) {
			settings.setColor(sf::Color::Yellow);
			settings.setStyle(sf::Text::Italic);
		}
		else {
			settings.setColor(sf::Color::White);
			settings.setStyle(0);
		}

		if (selection == Selection::Exit) {
			exit_game.setColor(sf::Color::Yellow);
			exit_game.setStyle(sf::Text::Italic);
		}
		else {
			exit_game.setColor(sf::Color::White);
			exit_game.setStyle(0);
		}
	}

	int Title::getSelection() {
		return selection;
	}

	void Title::setPosition(sf::Vector2f pos) {
		sf::Vector2f temp = pos;
	}

	void Title::setVisible(bool visibility) {
		is_visible = visibility;
	}

	bool Title::isVisible() {
		return is_visible;
	}

	sf::Vector2f Title::getPosition() {
		return sf::Vector2f(100, 100);
	}