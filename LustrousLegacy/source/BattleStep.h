#include "Step.h"
#include "BattleSystem.h"

class BattleStep : public Step {
public:
	BattleStep(BattleSystem * battleScene, std::string battle);
	virtual bool run(float elapsedTime, Actor& actor);

	// for debugging
	virtual std::string getType() { return type; }
private:
	BattleSystem *battle;
	std::string battleName;
	bool battleStarted;

	// for debugging
	std::string type = "Battle";
};