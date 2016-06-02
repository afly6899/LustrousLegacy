#include "FightingPawn.h"

#include <iostream>

FightingPawn::FightingPawn(sf::Texture & skin, BaseStats stats, int dlimit, int numPotions)
: Pawn(skin, 2), base(stats), damlimit(dlimit), currentHP(stats.health), counter(0), numPotions(numPotions) {
	showlimit = damlimit / 5;
	canshow = damaged = false;

	criticalRate = (base.speed * RNG()) % 20;
	if (criticalRate <= 5) { criticalRate += 4; }

	front = base.name + ": ";
	end = " / " + std::to_string(base.health);
}

void FightingPawn::update(float time) {
	if (damaged) {
		counter++;
		if ((counter % (showlimit + 1) == showlimit)) {
			// for doing the black/texture damage animation
			if (!canshow) {
				pawnSprite.setColor(sf::Color::Black);
			}
			else {
				pawnSprite.setColor(sf::Color::White);
			}
			canshow = !canshow;
		}
		if (counter % (damlimit + 1) == damlimit) {
			// for telling how much time the damage animation runs
			damaged = !damaged;
			counter = 0;
		}
	}
}

int FightingPawn::getSpeed()
{
	int chance = RNG() % 100;
	if (chance <= criticalRate) { return ceil(base.speed * 1.2); }
	return base.speed;
}

int FightingPawn::getAttack()
{
	int chance = RNG() % 100;
	prevAttack = (chance <= criticalRate)?  ceil(base.attack * 1.5) :  base.attack;
	return prevAttack;
}

bool FightingPawn::usePotion()
{
	if (numPotions == 0) { return false; }
	restoreHP(50); // 50 is the amount of HP a potion restores?
	numPotions--;
	return true;
}

std::string FightingPawn::getName()
{
	return base.name;
}

int FightingPawn::getHealth()
{
	return currentHP;
}

int FightingPawn::getNumPotions()
{
	return numPotions;
}

int FightingPawn::getPrevAttack()
{
	return prevAttack;
}

void FightingPawn::restoreHP(int hp) {
	currentHP = std::min(base.health, currentHP + hp);
}


bool FightingPawn::takeDamage(FightingPawn & enemy)
{
	damaged = true;
	counter = 0;
	if (enemy.base.attack > currentHP) { currentHP = 0;  return false; }
	currentHP -= enemy.getAttack();
	return (currentHP > 0);
}


bool FightingPawn::isFaster(FightingPawn &enemy)
{
	return getSpeed() >= enemy.getSpeed();
}

std::string FightingPawn::getClass()
{
	return "FightingPawn";
}

std::string FightingPawn::getStatus()
{
	return front + std::to_string(currentHP) + end;
}

void FightingPawn::respawn(BaseStats stats)
{
	currentHP = stats.health;
	criticalRate = (base.speed * RNG()) % 20;
	if (criticalRate <= 5) { criticalRate += 4; }

	front = base.name + ": ";
	end = " / " + std::to_string(base.health);
}

