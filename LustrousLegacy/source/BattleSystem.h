#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "FightingPawn.h"
#include "Cursor.h"
#include "UI.h"
#include <map>
#include <vector>
#include <random>

// Forward Declaration
namespace sf {

	class Texture;
	class Font;
	class Text;
}

class BattleSystem : public UI {
public:

	BattleSystem(const sf::Font& font, const sf::Texture &menubg, sf::Texture &cursorTexture, std::vector<sf::Texture*> textures, std::vector<StatPawn> stats, sf::Sound& sfx);
	virtual ~BattleSystem();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void change_selection(int up_or_down);
	int getSelection();
	virtual void setPosition(sf::Vector2f pos);
	virtual void update(sf::Vector2f pos, float elapsedTime);
	void battle(bool ENTER_KEY, float elapsedTime, FightingPawn &player);
	std::string BattleSystem::getClass();

	bool battleIsHappening() { return inBattle; }

private:
	bool inBattle;
	sf::Sound& cursorBleep;
	sf::Vector2f originalPos;
	Cursor battleCursor;
	int num_of_selections;
	int selection = Battle::Fight;
	int separation = 20;
	bool player_turn = true;
	std::map<int, sf::Text> battle_options;
	sf::Sprite background;
	//std::vector<FightingPawn> enemyPawns;
	FightingPawn enemyPawns;
	std::vector<sf::Texture*> spriteTextures;
};
#endif
#pragma once
