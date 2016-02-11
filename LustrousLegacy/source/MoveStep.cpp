#include "MoveStep.h"
// Audrey Edit: Adding Event class functionalities //
#include <iostream>
// *************** End Audrey Edit *************** //

/*********************************************************************
\brief temp
*********************************************************************/
MoveStep::MoveStep(std::vector<sf::Vector2f> positions):
	positions(positions){
	if (!positions.empty()) {
		current_target = positions[0];
		positions.erase(positions.begin());
	}
}

/*********************************************************************
\brief temp
*********************************************************************/
MoveStep::~MoveStep() {
}

/*********************************************************************
\brief temp
*********************************************************************/
bool MoveStep::run(float elapsedTime, Actor& actor) {
	if (actor.getPosition() == current_target) {
		if (positions.empty()) {
			return false;
		}
		current_target = positions[0];
		positions.erase(positions.begin());
	}
	actor.move(elapsedTime, current_target);
	return true;
}