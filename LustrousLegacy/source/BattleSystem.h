#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "FightingPawn.h"
#include "Cursor.h"
#include "UI.h"
#include <map>
#include <vector>

typedef sf::Texture T;
typedef std::map<Battle, std::string[3]> Messages;
typedef std::map<std::string, std::string> Description;

class BattleSystem : public UI {
public:

	BattleSystem(bool &inBattle, sf::Font& font, T &p, T &e, T &c, T &bg, T &bgm, BaseStats ps, BaseStats es, sf::Sound& sfx, sf::Sound& fanfare);
	~BattleSystem();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void change_selection(int up_or_down);
	void setPosition(sf::Vector2f pos);
	void update(sf::Vector2f pos, float elapsedTime);
	void battle(float elapsedTime);

	void handleInput(sf::Event event, std::map<int, bool> ui_kb, float elapsedTime);
	void statusMessage(Battle option, int status);

	void stopBattle();
	Battle getSelection();
	std::string BattleSystem::getClass();
	void setBattleBackground(const sf::Texture &background);

	void startBattle();

	void prepareForBattle() { inEvent = true; }
	bool isInBattle() { return inBattle; }
	bool isStarting() { return inEvent; }
	bool finallyEnd() { return counter >= waitlimit; }

private:
	bool &inBattle;
	bool inEvent = false;

	int counter;
	int waitlimit;
	bool playerTurn;
	bool didOtherMove;
	bool halfTurnDone;
	bool wholeTurnDone;

	int separation;
	sf::Vector2f originalPos;

	sf::Text status;
	sf::Text message;
	Messages toDisplay;
	Description toFill;

	FightingPawn enemy;
	FightingPawn player;

	int selection;
	Cursor cursor;
	sf::Sound& bleep;
	sf::Sound& fanfare;
	sf::Sprite topMenu;
	sf::Sprite bottomMenu;
	sf::Sprite background;
	std::vector<sf::Text> battle_options;
};
