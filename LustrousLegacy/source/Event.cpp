#include "Event.h"

Event::Event(std::queue<Step*> steps)
: eventSteps(steps) {
}

Event::Event(std::initializer_list<Step*> steps)
{
	for (Step* step : steps) {
		eventSteps.push(step);
	}
}

Event::~Event()
{
}

bool Event::startEvent()
{
	isRunning = true;
	if (currentEvent == nullptr) {
		nextEvent();
	}
	return isRunning;
}

void Event::runEvent(float elapsedTime, Actor & actor)
{
	if (isRunning) {
		// Step.run returns true if the event is still happening and false if it's not!
		if (!currentEvent->run(elapsedTime, actor)) {
			nextEvent();
		}
	}
}

void Event::pauseEvent() // is this needed? How would we do things such that the thing isn't
{
	isRunning = false;
}

void Event::nextEvent()
{
	Step* to_delete = currentEvent; // want to deallocate the previously done step!
	if (!eventSteps.empty()) {
		currentEvent = eventSteps.front();
		eventSteps.pop();
		isRunning = true;
	}
	else {
		isRunning = false;
	}
	if (to_delete != nullptr) {
		delete to_delete;
	}
}


// Audrey Edit: Adding Event class functionalities //

// *************** End Audrey Edit *************** //