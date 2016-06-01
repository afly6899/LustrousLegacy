#include <iostream>
#include <sstream>
#include <fstream>
#include "BattleScene.h"

BattleScene::BattleScene(const sf::Texture &background, const sf::Texture &player, StatPawn playerStats)
: fighterPlayer(player, playerStats) {
	battleBackground.setTexture(background);

	currentBattle = nullptr;

	status.setString(fighterPlayer.getHealthInfo());

	animationCounter = 0;
	animationDuration = 200;
	currentAnimation = 0;

}

BattleScene::~BattleScene()
{
	if (currentBattle != nullptr) {
		delete currentBattle;
	}
}

void BattleScene::loadBattle(bool &startBattle, const sf::Font &font, const sf::Texture &menubg, sf::Texture &cursor, sf::Sound &sfx)
{
	std::ifstream file("resources/battles/" + battleName);
	std::vector<StatPawn> stats;
	std::vector<sf::Texture*> textures;
	int health, attack, speed;
	std::string texture, name, filelines;
	while (std::getline(file, filelines)) {
		std::stringstream info(filelines);
		info >> texture >> health >> attack >> speed >> name;
		sf::Texture *temp = new sf::Texture();
		temp->loadFromFile("resources/textures/" + texture);
		StatPawn tempStats = { health, attack, speed, name };
		stats.push_back(tempStats);
		textures.push_back(temp);
	}
	currentBattle = new BattleSystem(font, menubg, cursor, textures, stats, sfx);
	startBattle = true;
	inBattle = true;
	battleReady = false;

	status = sf::Text(fighterPlayer.getHealthInfo(), font, 20);
}

void BattleScene::update(sf::Vector2f pos, float elapsedTime)
{
	status.setString(fighterPlayer.getHealthInfo());
	setPosition(pos);
	if (currentBattle != nullptr) {
		currentBattle->update(pos, elapsedTime);
		inBattle = currentBattle->battleIsHappening();
	} else {
		inBattle = false;
	}

	animationCounter += elapsedTime;
	if (animationCounter >= animationDuration) {
		currentAnimation++;
		if (currentAnimation == 2) {
			currentAnimation = 0;
		}
		animationCounter = 0;
		fighterPlayer.setTextureRect(sf::IntRect(currentAnimation * 200, 0, 200, 200));
	}
}

void BattleScene::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(battleBackground, states);
	target.draw(fighterPlayer);
	if (currentBattle != nullptr) {
		target.draw(*currentBattle, states);
	}
	target.draw(status, states);
}

void BattleScene::setPosition(sf::Vector2f pos)
{
	battleBackground.setPosition(pos - sf::Vector2f(400, 300));
	fighterPlayer.setPosition(pos + sf::Vector2f(200, 150));
	if (currentBattle != nullptr) {
		currentBattle->setPosition(pos);
	}
	status.setPosition(pos + sf::Vector2f(150, 150));
}

void BattleScene::handleInput(sf::Event event, std::map<int, bool> ui_kb, float elapsedTime)
{
	if (currentBattle != nullptr) {
		if (ui_kb[sf::Keyboard::Up] || ui_kb[sf::Keyboard::Down])
			currentBattle->change_selection(event.key.code);
		else if (ui_kb[sf::Keyboard::Return]) {
			switch (currentBattle->getSelection()) {
			case(Battle::Fight) :
				currentBattle->battle(ui_kb[sf::Keyboard::Return], elapsedTime, fighterPlayer);
				break;
			case(Battle::Escape) :
				delete currentBattle;
				currentBattle = nullptr;
				inBattle = false;
				break;
			case(Battle::Items) :
				std::cout << "There is no implementation of this option yet." << std::endl;
				break;
			case(Battle::Status) :
				std::cout << "There is no implementation of this option yet." << std::endl;
				break;
			}
		}
	}
}
