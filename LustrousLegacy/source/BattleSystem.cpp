#include "BattleSystem.h"
#include "Enums.h"
#include <iostream>
#include <random>

/*********************************************************************
\brief temp
*********************************************************************/
BattleSystem::BattleSystem(const sf::Font& font, const sf::Vector2u window_size, const sf::Texture& imagePath_title, sf::Sound& sfx):
		cursorBleep(sfx),
		window_size(window_size), 
		battleCursor(imagePath_title),
		enemy(imagePath_title){

		background.setSize(sf::Vector2f(window_size.x, window_size.y));
		background.setFillColor(sf::Color(0, 0, 0, 255));
		background.setOrigin(window_size.x*.5, window_size.y*.5);
		enemy.setPosition(background.getOrigin());

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
		target.draw(enemy, states);
}

/*********************************************************************
\brief temp
*********************************************************************/
void BattleSystem::change_selection(int key) {
	
	clearStyle(battle_options[selection]);

	if (key == sf::Keyboard::Right)
		selection += 1;
	else if (key == sf::Keyboard::Left)
		selection -= 1;
	else if (key == sf::Keyboard::Up)
		switch (selection) {
		case(Battle::Status) :
			selection = Battle::Fight;
			break;
		case(Battle::Fight) :
			selection = Battle::Status;
			break;
		case(Battle::Escape) :
			selection = Battle::Items;
			break;
		case(Battle::Items) :
			selection = Battle::Escape;
			break;
		}
	else if (key == sf::Keyboard::Down)
		switch (selection) {
		case(Battle::Status) :
			selection = Battle::Fight;
			break;
		case(Battle::Fight) :
			selection = Battle::Status;
			break;
		case(Battle::Escape) :
			selection = Battle::Items;
			break;
		case(Battle::Items) :
			selection = Battle::Escape;
			break;
		}
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
void BattleSystem::battle(bool ENTER_KEY, float elapsedTime) {
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0, 1);
	std::uniform_real_distribution<float> distribution_e(.3, 1);
	float proportion = distribution(generator);
	float proportion_e = distribution_e(generator);

	if (player_turn && ENTER_KEY) {
		if (selection == Battle::Fight) {
			enemy_health -= player_attack * proportion;
			if (enemy_health <= 0) {
				enemy_health = 0;
				std::cout << "Enemy has died" << std::endl;
			}
				
			std::cout << "Enemy Health: " << enemy_health << std::endl;
			player_turn = false;
		}
	}
	else if (!player_turn && enemy_health != 0) {
		player_health -= enemy_attack * proportion_e;
		if (player_health <= 0) {
			player_health = 0;
			std::cout << "Player has died" << std::endl;
		}
		std::cout << "Player Health: " << player_health << std::endl;
		player_turn = true;

	}
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