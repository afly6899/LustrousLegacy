#include "BattleSystem.h"
#include "Enums.h"

/*********************************************************************
\brief temp
*********************************************************************/
BattleSystem::BattleSystem(const sf::Font& font, const sf::Vector2u window_size, const sf::Texture& imagePath_title, sf::Sound& sfx):
		cursorBleep(sfx),
		window_size(window_size), 
		battleCursor(imagePath_title) {

		background.setSize(sf::Vector2f(window_size.x, window_size.y));
		background.setFillColor(sf::Color(0, 0, 0, 255));
		background.setOrigin(window_size.x*.5, window_size.y*.5);

		battle_options[Battle::Fight] = createOption("Fight", font);
		battle_options[Battle::Items] = createOption("Items", font);
		battle_options[Battle::Status] = createOption("Status", font);
		battle_options[Battle::Escape] = createOption("Escape", font);

		num_of_selections = battle_options.size();
		selected(battle_options[Battle::Fight]);
	}

/*********************************************************************
\brief temp
*********************************************************************/
BattleSystem::~BattleSystem() {}

/*********************************************************************
\brief temp
*********************************************************************/
void BattleSystem::update(sf::Vector2f pos, float elapsedTime) {
	setPosition(pos);
	battleCursor.spriteAnimate(elapsedTime);
}

/*********************************************************************
\brief temp
*********************************************************************/
void BattleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(background, states);
		target.draw(battleCursor, states);
	for (auto option = battle_options.begin(); option != battle_options.end(); option++)
		target.draw(option->second, states);	
}


/*********************************************************************
\brief temp
*********************************************************************/
void BattleSystem::change_selection(int up_or_down) {
	
	clearStyle(battle_options[selection]);

	if (up_or_down == sf::Keyboard::Down)
		selection += 1;
	else if (up_or_down == sf::Keyboard::Up)
		selection -= 1;

	if (selection > num_of_selections)
		selection = 1;
	else if (selection < 1)
		selection = num_of_selections;

	sf::Vector2f pos = battle_options[selection].getPosition();
	battleCursor.setPosition(sf::Vector2f(pos.x, pos.y + 50));
	cursorBleep.play();
	selected(battle_options[selection]);
}

/*********************************************************************
\brief temp
*********************************************************************/
int BattleSystem::getSelection() {
	return selection;
}

/*********************************************************************
\brief temp
*********************************************************************/
void BattleSystem::setPosition(sf::Vector2f pos) {
	if (pos != originalPos) {
		originalPos = pos;
		background.setPosition(pos);
		battle_options[Battle::Fight].setPosition(pos);
		battle_options[Battle::Items].setPosition(battle_options[Battle::Fight].getPosition().x + seperation, battle_options[Battle::Fight].getPosition().y);
		battle_options[Battle::Status].setPosition(battle_options[Battle::Items].getPosition().x - seperation, battle_options[Battle::Items].getPosition().y + seperation);
		battle_options[Battle::Escape].setPosition(battle_options[Battle::Status].getPosition().x + seperation, battle_options[Battle::Status].getPosition().y);
		sf::Vector2f pos = battle_options[selection].getPosition();
		battleCursor.setPosition(sf::Vector2f(pos.x, pos.y + 50));
	}
}

/*********************************************************************
\brief temp
*********************************************************************/
std::string BattleSystem::getClass() {
	return "BattleSystem";
}