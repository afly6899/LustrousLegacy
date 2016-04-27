#include "UI.h"
#include "Enums.h"

/*********************************************************************
\brief temp
*********************************************************************/
void UI::selected(sf::Text& string) {
	string.setColor(sf::Color::Yellow);
	string.setStyle(sf::Text::Italic);
}

/*********************************************************************
\brief temp
*********************************************************************/
void UI::clearStyle(sf::Text& string) {
	string.setColor(sf::Color::White);
	string.setStyle(sf::Text::Regular);
}

/*********************************************************************
\brief temp
*********************************************************************/
sf::Text UI::createOption(std::string option_name, const sf::Font& font) {
	sf::Text option;
	option.setFont(font);
	option.setString(option_name);
	option.setCharacterSize(Font_Size::Default);
	option.setOrigin(option.getLocalBounds().width *.5, option.getLocalBounds().height*.5);
	return option;
}

void UI::toggleStyle(sf::Text & current, sf::Text & old) {
	current.setColor(sf::Color::Yellow);
	current.setStyle(sf::Text::Italic | sf::Text::Bold);

	if (old.getString() != "") {
		old.setColor(sf::Color::White);
		old.setStyle(sf::Text::Regular);
	}
}