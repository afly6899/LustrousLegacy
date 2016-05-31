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
	BattleScene(const sf::Texture &background, const sf::Texture &player, StatPawn playerStats);
	~BattleScene();

	void loadBattle(bool &startBattle, const sf::Font &font, const sf::Texture &menubg, sf::Texture &cursor, sf::Sound &sfx);
	bool getBattleState() { return inBattle; }

	void update(sf::Vector2f pos, float elapsedTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setPosition(sf::Vector2f pos);

private:
	int battleNum;
	int numBattles;
	std::string battleName;
	bool inBattle = false;
	FightingPawn fighterPlayer;
	sf::Sprite battleBackground;

	BattleSystem *currentBattle;

	sf::Text status;
};