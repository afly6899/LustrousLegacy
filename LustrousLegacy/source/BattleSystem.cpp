#include "BattleSystem.h"
#include "Enums.h"
#include "FightingPawn.h"
#include <iostream>

/*********************************************************************
\brief temp
*********************************************************************/
BattleSystem::BattleSystem(const sf::Font& font, const sf::Texture &menubg, sf::Texture &cursorTexture, std::vector<sf::Texture*> textures, std::vector<StatPawn> stats, sf::Sound& sfx)
: cursorBleep(sfx), battleCursor(cursorTexture), spriteTextures(textures), background(menubg), enemyPawns(*textures[0], stats[0]) {

	//for (int i = 1; i < textures.size(); i++) {
	//	enemyPawns.push_back(FightingPawn(*textures[i], stats[i]));
	//}

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
BattleSystem::~BattleSystem() {
	for (int i = 0; i < spriteTextures.size(); i++) {
		delete spriteTextures[i];
	}
}

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
	
	//for (int i = 0; i < enemyPawns.size(); i++) {
	//	target.draw(enemyPawns[i], states);
	//}
	target.draw(enemyPawns, states);
}

/*********************************************************************
\brief temp
*********************************************************************/
void BattleSystem::change_selection(int key) {

	clearStyle(battle_options[selection]);

	if (key == sf::Keyboard::Up || key == sf::Keyboard::Down)
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
	battleCursor.setPosition(sf::Vector2f(pos.x, pos.y + 25));
	cursorBleep.play();
	selected(battle_options[selection]);
}

/*********************************************************************
\brief temp
*********************************************************************/
void BattleSystem::battle(bool ENTER_KEY, float elapsedTime, FightingPawn &player) {
	if (player.isAlive() && enemyPawns.isAlive()) {
		if (player_turn && ENTER_KEY) {
			if (selection == Battle::Fight) {
				if (!enemyPawns.takeDamage(&player)) {
					std::cout << "Enemy has died" << std::endl;
				}
				player_turn = false;
			}
		}
		else if (!player_turn) {
			if (!player.takeDamage(&enemyPawns)) {
				std::cout << "Player has died" << std::endl;
			}
			player_turn = true;

		}
	}
	else {
		inBattle = false;
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
		background.setPosition(pos - sf::Vector2f(400, -150));
		battle_options[Battle::Fight].setPosition(pos + sf::Vector2f(0, 170));
		sf::Vector2f nexty = battle_options[Battle::Fight].getPosition();
		battle_options[Battle::Items].setPosition(pos + sf::Vector2f(0, 205));
		battle_options[Battle::Status].setPosition(pos + sf::Vector2f(0, 240));
		battle_options[Battle::Escape].setPosition(pos + sf::Vector2f(0, 275));
		sf::Vector2f c_pos = battle_options[selection].getPosition();
		battleCursor.setPosition(sf::Vector2f(c_pos.x, c_pos.y + 25));

		
		enemyPawns.setPosition(pos - sf::Vector2f(200, -150));
		//int ypos = 50;
		//for (int i = 0; i < enemyPawns.size(); i++) {
		//	enemyPawns[i].setPosition(pos + sf::Vector2f(100, ypos));
		//	ypos += 100;
		//}
	}
}

/*********************************************************************
\brief temp
*********************************************************************/
std::string BattleSystem::getClass() {
	return "BattleSystem";
}