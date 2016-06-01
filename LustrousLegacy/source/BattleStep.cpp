#include "BattleStep.h"

BattleStep::BattleStep(BattleScene * battleScene, std::string battle)
: battle(battleScene), battleName(battle) {
	battleStarted = false;
}

bool BattleStep::run(float elapsedTime, Actor & actor)
{
	if (!battleStarted && !battle->getBattleState()) {
		battle->setBattle(battleName);
		battle->prepareBattle();
		battleStarted = true;
	}
	return (battle->getBattleState() || battle->getStartBattle());
}
