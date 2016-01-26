#include "MoveStep.h"

/*********************************************************************
*********************************************************************/
MoveStep::MoveStep(std::vector<sf::Vector2f> positions):
	positions(positions),
	current_target(positions[0]){
}

/*********************************************************************
*********************************************************************/
MoveStep::~MoveStep() {
}

/*********************************************************************
*********************************************************************/
bool MoveStep::run(float elapsedTime, Actor& actor) {
	if (positions.empty())
		return false;
	if (actor.getPosition() == current_target) {
		positions.erase(positions.begin());
		current_target = positions[0];
	}
	actor.move(elapsedTime, positions[0]);
	return true;
}