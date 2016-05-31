#include "FightingPawn.h"

#include <iostream>

FightingPawn::FightingPawn(const sf::Texture & playerTexture, StatPawn statBase)
: Pawn(playerTexture, 2), base(statBase){
	remainingHealth = base.health;
	criticalRate = (base.speed * RNG()) % 20;
	if (criticalRate <= 5) {
		criticalRate += 4;
	}
	alive = true;
	std::cout << "Created Fighting Pawn with HP: " << base.health << ", Attack: " << base.attack << "Speed: " << base.speed << ", and Crit: " << ((float)criticalRate / 100.0) << std::endl;
}


FightingPawn::~FightingPawn()
{
}

int FightingPawn::attack()
{
	int chance = RNG() % 100;
	std::cout << "Chance of Critical: " << ((float)chance / 100.0) << ", Dealing Damage: " << ((chance <= criticalRate)? ceil(base.attack * 1.5) : base.attack) << std::endl;
	if (chance <= criticalRate) {
		return ceil(base.attack * 1.5);
	} 
	return base.attack;
}

bool FightingPawn::takeDamage(FightingPawn *enemy)
{
	if (enemy->base.attack > remainingHealth) {
		alive = false;
	}
	else {
		remainingHealth -= enemy->attack();
		alive = (remainingHealth > 0);
	}
	std::cout << "After Enemy attack, have " << remainingHealth << " HP remaining (is alive? " << alive << ")" << std::endl;
	return alive;
}

void FightingPawn::respawn()
{
	remainingHealth = base.health;
	alive = true;
	criticalRate = (base.speed * RNG()) % 20;
}
