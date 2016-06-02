#pragma once
#include <random>
#include "Pawn.h"

struct BaseStats {
	int health;
	int attack;
	int speed;
	std::string name;
};

class FightingPawn : public Pawn {
public:

	FightingPawn(sf::Texture &skin, BaseStats stats, int slimit, int numPotions);

	void update(float time);

	int getSpeed();
	int getAttack();
	bool usePotion();
	void restoreHP(int hp);
	bool takeDamage(FightingPawn &enemy);
	bool isFaster(FightingPawn &enemy);

	std::string getClass();
	std::string getStatus();

	int getHealth();
	int getNumPotions();
	int getPrevAttack();
	std::string getName();

	void respawn(BaseStats stats);

private:
	int counter;
	int damlimit, showlimit;
	bool damaged, canshow;

	int currentHP;
	int prevAttack;
	BaseStats base;
	int criticalRate;
	std::random_device RNG;

	std::string front, end;

	int numPotions;
};
