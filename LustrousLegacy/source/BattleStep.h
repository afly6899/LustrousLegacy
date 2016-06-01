#include "Step.h"
#include "BattleScene.h"

class BattleStep : public Step {
public:
	BattleStep(BattleScene * battleScene, std::string battle);
	virtual bool run(float elapsedTime, Actor& actor);

	// for debugging
	virtual std::string getType() { return type; }
private:
	BattleScene *battle;
	std::string battleName;
	bool battleStarted;

	// for debugging
	std::string type = "Battle";
};