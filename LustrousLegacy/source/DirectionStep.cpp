#include "DirectionStep.h"

DirectionStep::DirectionStep(Direction direction)
: faceDirection(direction) {
}

bool DirectionStep::run(float elapsedTime, Actor & actor)
{
	actor.setDirection(faceDirection);
	return false;
}
