#pragma once
#include <random>
#include "Pawn.h"

struct StatPawn {
	int health;
	int attack;
	int speed;
	std::string name;
};

class FightingPawn : public Pawn {
public:
	FightingPawn(const sf::Texture& playerTexture, StatPawn statBase);
	~FightingPawn();
	std::string getClass() { return "FightingPawn"; }

	int FightingPawn::attack();
	bool takeDamage(FightingPawn *enemy);
	void respawn();

private:
	bool alive;
	StatPawn base;
	int criticalRate;
	int remainingHealth;
	std::random_device RNG;
};
