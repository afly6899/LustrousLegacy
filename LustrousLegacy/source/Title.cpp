#include "title.h"
#include "Enums.h"

/*********************************************************************
\brief temp
*********************************************************************/
Title::Title(const sf::Font& font, const sf::Vector2u window_size, const sf::Texture& imagePath_title, sf::Sound& sfx, sf::Music& titleMusic):
		cursorBleep(sfx),
		window_size(window_size), 
		titleLogo(imagePath_title),
		titleCursor(imagePath_title) {

		title_options[Selection::Play_Game] = createOption("Play Game", font);
		title_options[Selection::Load_Game] = createOption("Load Game", font);
		title_options[Selection::Settings] = createOption("Settings", font);
		title_options[Selection::Exit] = createOption("Exit", font);
	
		num_of_selections = title_options.size();

		selected(title_options[Selection::Play_Game]);
	}

/*********************************************************************
\brief temp
*********************************************************************/
Title::~Title() {}

/*********************************************************************
\brief temp
*********************************************************************/
void Title::update(sf::Vector2f pos, float elapsedTime) {
	setPosition(pos);
	titleCursor.spriteAnimate(elapsedTime);
}

/*********************************************************************
\brief temp
*********************************************************************/
void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(titleCursor, states);
	for (auto option = title_options.begin(); option != title_options.end(); option++)
		target.draw(option->second, states);
}

/*********************************************************************
\brief temp
*********************************************************************/
void Title::change_selection(int up_or_down) {
	
	clearStyle(title_options[selection]);

	if (up_or_down == Cursor_Direction::Down)
	{
		selection += 1;
		if (selection > num_of_selections) {
			titleCursor.setPosition(cursor_originalPos);
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

	selected(title_options[selection]);
}

/*********************************************************************
\brief temp
*********************************************************************/
int Title::getSelection() {
	return selection;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Title::setPosition(sf::Vector2f pos) {
	if (pos != originalPos) {
		originalPos = pos;
		title_options[Selection::Play_Game].setPosition(pos);
		title_options[Selection::Load_Game].setPosition(title_options[Selection::Play_Game].getPosition().x, title_options[Selection::Play_Game].getPosition().y + seperation);
		title_options[Selection::Settings].setPosition(title_options[Selection::Load_Game].getPosition().x, title_options[Selection::Load_Game].getPosition().y + seperation);
		title_options[Selection::Exit].setPosition(title_options[Selection::Settings].getPosition().x, title_options[Selection::Settings].getPosition().y + seperation);
		titleCursor.setPosition(sf::Vector2f(title_options[Selection::Play_Game].getPosition().x, title_options[Selection::Play_Game].getPosition().y + seperation*.5));
		cursor_originalPos = titleCursor.getPosition();
	}
}
