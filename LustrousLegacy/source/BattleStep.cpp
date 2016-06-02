#include "BattleStep.h"

BattleStep::BattleStep(BattleSystem * battleScene, std::string battle)
: battle(battleScene), battleName(battle) {
	battleStarted = false;
}

bool BattleStep::run(float elapsedTime, Actor & actor)
{
	if (!battleStarted && !battle->isInBattle()) {
		battle->prepareForBattle();
		battleStarted = true;
	}
	return (battle->isInBattle() || battle->isStarting());
}
