#include "Step.h"

class DirectionStep : public Step {
public:
	DirectionStep(Direction direction);
	virtual bool run(float elapsedTime, Actor& actor);

	// for debugging
	virtual std::string getType() { return type; }
private:
	Direction faceDirection;

	// for debugging
	std::string type = "Direction";
};