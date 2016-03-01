#include "TogetherStep.h"

TogetherStep::TogetherStep(std::vector<sf::Vector2f> positions, std::vector<Actor*> actors)
: positions(positions), actors(actors) {
}

TogetherStep::~TogetherStep()
{
	while (positions.size() > 0) {
		positions.erase(positions.begin());
	}
}

bool TogetherStep::run(float elapsedTime, Actor & actor)
{
	bool isDone = true;
	for (int i = 0; i < actors.size(); i++) {
		if (actors[i]->getPosition() == positions[i]) {
			isDone = isDone && true;
		}
		else
			isDone = false;
		actors[i]->move(elapsedTime, positions[i]);
	}
	return !isDone;
}
