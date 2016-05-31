#include <iostream>
#include <sstream>
#include <fstream>
#include "BattleScene.h"

BattleScene::BattleScene(const sf::Texture &background, const sf::Texture &player, StatPawn playerStats)
: fighterPlayer(player, playerStats) {
	battleBackground.setTexture(background);

	battleName = "battle";
	numBattles = 2;
	battleNum = 1;

	currentBattle = nullptr;

	status.setString(fighterPlayer.getHealthInfo());
}

BattleScene::~BattleScene()
{
}

void BattleScene::loadBattle(bool &startBattle, const sf::Font &font, const sf::Texture &menubg, sf::Texture &cursor, sf::Sound &sfx)
{
	std::string filename = battleName + std::to_string(battleNum) + ".txt";
	std::ifstream file("resources/battles/" + filename);
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
	startBattle = inBattle = true;

	status = sf::Text(fighterPlayer.getHealthInfo(), font, 20);
}

void BattleScene::update(sf::Vector2f pos, float elapsedTime)
{
	status.setString(fighterPlayer.getHealthInfo());
	setPosition(pos);
	currentBattle->update(pos, elapsedTime);
	inBattle = currentBattle->battleIsHappening();
}

void BattleScene::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(battleBackground, states);
	target.draw(fighterPlayer);
	target.draw(*currentBattle, states);
	target.draw(status, states);
}

void BattleScene::setPosition(sf::Vector2f pos)
{
	battleBackground.setPosition(pos - sf::Vector2f(400, 300));
	fighterPlayer.setPosition(pos + sf::Vector2f(200, 150));
	currentBattle->setPosition(pos);
	status.setPosition(pos + sf::Vector2f(150, 150));
}
