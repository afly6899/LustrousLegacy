#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>
#include <string>

#include "BattleSystem.h"
#include "FightingPawn.h"

class BattleScene : public sf::Drawable
{
public:
	BattleScene(const sf::Texture &background, const sf::Texture &player, BaseStats playerStats, sf::Music &music);
	~BattleScene();

	

private:
	sf::Text status;
	std::string battleName;
	bool battleReady = false;
	bool inBattle = false;
	FightingPawn fighterPlayer;
	sf::Sprite battleBackground;
	BattleSystem *currentBattle;
	sf::Music &music;

	int currentAnimation;
	int animationCounter;
	int animationDuration;

	int numPotions;
	bool wait;
	bool otherTurn;
};