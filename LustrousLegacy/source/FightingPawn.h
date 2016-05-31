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

	int attack();
	bool takeDamage(FightingPawn *enemy);
	void respawn();

	bool isAlive() { return alive; }
	std::string getHealthInfo() { return base.name + ": " + std::to_string(remainingHealth) + " / " + std::to_string(base.health); }

private:
	bool alive;
	StatPawn base;
	int criticalRate;
	int remainingHealth;
	std::random_device RNG;
};
