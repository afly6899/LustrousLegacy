#include "BattleSystem.h"
#include <iostream>

BattleSystem::BattleSystem(bool &inBattle, sf::Font& font, T &p, T &e, T &c, T &bg, T &bgm, BaseStats ps, BaseStats es, sf::Sound& sfx, sf::Sound& fanfare)
	: inBattle(inBattle), player(p, ps, 20, 10), enemy(e, es, 20, 0), cursor(c), bleep(sfx), fanfare(fanfare), background(bg), topMenu(bgm), bottomMenu(bgm) {
	battle_options.push_back(createOption("Attack", font));
	battle_options.push_back(createOption("Potion", font));
	battle_options.push_back(createOption("Status", font));
	battle_options.push_back(createOption("Escape", font));

	selection = Battle::Attack;
	separation = 35;
	status = sf::Text(player.getStatus(), font, 24);

	counter = 100;
	waitlimit = 100;
	wholeTurnDone = true;
	halfTurnDone = false;

	toDisplay[Battle::Attack][false] = "[enemy] is defeated!";
	toDisplay[Battle::Attack][true] = "You dealt [attack] damage";
	toDisplay[Battle::Attack][2] = "Attack to deal physical damage";
	toDisplay[Battle::Potion][false] = "You have no more potions to heal with!";
	toDisplay[Battle::Potion][true] = "You healed 50 hp";
	toDisplay[Battle::Potion][2] = "Heal yourself with a potion (have [num] left)";
	toDisplay[Battle::Status][false] = "Scan failed";
	toDisplay[Battle::Status][true] = "[enemy] scanned: has [ehp] left";
	toDisplay[Battle::Status][2] = "Scan the enemy";
	toDisplay[Battle::Escape][false] = "You can't run away!";
	toDisplay[Battle::Escape][true] = "Successfully ran away";
	toDisplay[Battle::Escape][2] = "Run from the fight";
	toDisplay[Battle::Defend][false] = "You were defeated...";
	toDisplay[Battle::Defend][true] = "You took [eattack] damage";
	toDisplay[Battle::Defend][2] = "The battle has ended. [greet]";

	message = sf::Text("Attack to deal physical damage", font, 20);
}


BattleSystem::~BattleSystem() {
}

void BattleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(background, states);

	target.draw(player, states);
	target.draw(enemy, states);

	target.draw(bottomMenu, states);
	target.draw(topMenu, states);

	target.draw(cursor, states);
	for (int i = 0; i < battle_options.size(); i++)
		target.draw(battle_options[i], states);

	target.draw(status, states);
	target.draw(message, states);
}


void BattleSystem::change_selection(int key) {

	clearStyle(battle_options[selection]);

	if (key == sf::Keyboard::Up) {
		selection--;
	}
	else if (key == sf::Keyboard::Down) {
		selection++;
	}
	if (selection == battle_options.size())
		selection = 1;
	else if (selection < 0)
		selection = battle_options.size() - 1;

	sf::Vector2f pos = battle_options[selection].getPosition();
	cursor.setPosition(sf::Vector2f(pos.x, pos.y + separation));
	bleep.play();
	selected(battle_options[selection]);
	statusMessage(Battle(selection), 2);
}

void BattleSystem::setPosition(sf::Vector2f pos) {
	if (pos != originalPos) {

		originalPos = pos;

		topMenu.setPosition(pos - sf::Vector2f(400, 400));
		bottomMenu.setPosition(pos - sf::Vector2f(400, -150));
		background.setPosition(pos - sf::Vector2f(400, 300));

		int offset = 170;
		battle_options[Battle::Attack].setPosition(pos + sf::Vector2f(0, offset));
		battle_options[Battle::Potion].setPosition(pos + sf::Vector2f(0, offset += separation));
		battle_options[Battle::Status].setPosition(pos + sf::Vector2f(0, offset += separation));
		battle_options[Battle::Escape].setPosition(pos + sf::Vector2f(0, offset += separation));

		sf::Vector2f c_pos = battle_options[selection].getPosition();
		cursor.setPosition(sf::Vector2f(c_pos.x, c_pos.y + separation));

		player.setPosition(pos + sf::Vector2f(200, 150));
		enemy.setPosition(pos - sf::Vector2f(200, -150));

		status.setPosition(pos + sf::Vector2f(150, 150));
		message.setPosition(pos - sf::Vector2f(message.getCharacterSize() * message.getString().getSize() / 2, 250));
	}
}

void BattleSystem::update(sf::Vector2f pos, float elapsedTime) {
	setPosition(pos);
	player.update(elapsedTime);
	enemy.update(elapsedTime);
	cursor.spriteAnimate(elapsedTime);

	status.setString(player.getStatus());
	message.setPosition(pos - sf::Vector2f(message.getCharacterSize() * message.getString().getSize() / 3, 250));
	std::cout << "wholeTurnDone? " << wholeTurnDone << ", or halfTurnDone? " << halfTurnDone << ", counter = " << counter << std::endl;
	if (!wholeTurnDone || !halfTurnDone) {
		counter++;
		if (counter >= waitlimit) {
			if (inBattle) {
				if (halfTurnDone) {
					battle(elapsedTime);
					counter = 0;
				}
				else if (wholeTurnDone) {
					counter = waitlimit;
				}
			}
			else {
				counter = waitlimit;
			}
		}
	}
}





/*********************************************************************
\brief temp
*********************************************************************/
void BattleSystem::battle(float elapsedTime) {
	if (inBattle) {
		if (wholeTurnDone) {
			playerTurn = player.isFaster(enemy);
			halfTurnDone = true;
			wholeTurnDone = false;
			if (playerTurn) {
				statusMessage(Battle::Attack, enemy.takeDamage(player));
			}
			else {
				statusMessage(Battle::Defend, player.takeDamage(enemy));
			}
		}
		else if (halfTurnDone) {
			wholeTurnDone = true;
			halfTurnDone = false;
			playerTurn = !didOtherMove && !playerTurn;
			if (playerTurn) {
				statusMessage(Battle::Attack, enemy.takeDamage(player));
			}
			else {
				statusMessage(Battle::Defend, player.takeDamage(enemy));
			}
		}
	}
}
void BattleSystem::handleInput(sf::Event event, std::map<int, bool> ui_kb, float elapsedTime)
{
	if (inBattle) {
		if (counter >= waitlimit) {
			if (wholeTurnDone) {
				if (ui_kb[sf::Keyboard::Up] || ui_kb[sf::Keyboard::Down]) {
					change_selection(event.key.code);
				}
				else if (ui_kb[sf::Keyboard::Return]) {
					switch (selection) {
					case Battle::Attack:
						didOtherMove = false;
						battle(elapsedTime);
						break;
					case Battle::Potion:
						wholeTurnDone = false;
						didOtherMove = true;
						statusMessage(Battle::Potion, player.usePotion());
						break;
					case Battle::Status:
						wholeTurnDone = false;
						didOtherMove = true;
						statusMessage(Battle::Status, (int)elapsedTime % 2 == 0);
						break;
					case Battle::Escape:
						wholeTurnDone = false;
						didOtherMove = true;
						statusMessage(Battle::Escape, !inEvent && (int)elapsedTime % 3 < 2);
						break;
					}
					counter = 0;
					halfTurnDone = true;
				}
			}
		}
	}
}

void BattleSystem::statusMessage(Battle option, int status)
{
	switch (option) {
	case Battle::Attack:
		toFill["enemy"] = enemy.getName();
		toFill["attack"] = std::to_string(player.getPrevAttack());
		break;
	case Battle::Potion:
		toFill["num"] = std::to_string(player.getNumPotions());
		break;
	case Battle::Status:
		toFill["enemy"] = enemy.getName();
		toFill["ehp"] = std::to_string(enemy.getHealth());
		break;
	case Battle::Escape:

	case Battle::Defend:
		toFill["eattack"] = std::to_string(enemy.getPrevAttack());
		toFill["greet"] = (player.getHealth() > enemy.getHealth()) ? "You win!" : "You lose...";
		break;
	}
	std::string temp = toDisplay[option][status];
	std::string replacing;
	std::string endofstring;

	int begin1 = temp.find_first_of("[");
	if (begin1 != std::string::npos) {

		int stop1 = temp.find_first_of("]");
		int begin2 = temp.find_last_of("["), stop2 = temp.find_last_of("]");

		if (begin1 == begin2) {
			endofstring = temp.substr(stop1 + 1);
		}
		else {
			endofstring = temp.substr(stop1 + 1, begin2 - stop1 - 1) + toFill[temp.substr(begin2 + 1, stop2 - begin2 - 1)] + temp.substr(stop2 + 1);
		}

		replacing = temp.substr(begin1 + 1, stop1 - begin1 - 1);
		temp = temp.substr(0, begin1) + toFill[replacing] + endofstring;
	}
	message.setString(temp);
	if (((option == Battle::Attack || option == Battle::Defend) && !status) || (option == Battle::Escape && status == 1)) {
		stopBattle();
	}
}

void BattleSystem::stopBattle()
{
	inBattle = false;
	inEvent = false;
	counter = 0;
	statusMessage(Battle::Defend, 2);
	wholeTurnDone = false;
	halfTurnDone = false;
	fanfare.play();
}

Battle BattleSystem::getSelection() {
	return Battle(selection);
}

std::string BattleSystem::getClass() {
	return "BattleSystem";
}

void BattleSystem::setBattleBackground(const sf::Texture & bgTexture)
{
	background.setTexture(bgTexture);
}

void BattleSystem::startBattle()
{
	statusMessage(Battle::Attack, 2);
	enemy.restoreHP(1000);
	inBattle = true;
	counter = 100;
	waitlimit = 100;
	wholeTurnDone = true;
	halfTurnDone = false;
}
