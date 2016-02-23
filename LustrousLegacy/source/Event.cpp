#include <iostream>
#include "Event.h"

Event::Event(std::queue<std::pair<Step*, Actor*>> steps)
: eventSteps(steps) {
	currentEvent.first = nullptr;
	currentEvent.second = nullptr;
}

Event::Event(std::initializer_list<Step*> steps, std::initializer_list<Actor*> actors)
{
	std::initializer_list<Step*>::iterator steps_it = steps.begin();
	std::initializer_list<Actor*>::iterator actors_it = actors.begin();
	
	eventSteps.push(std::make_pair(*steps_it, *actors_it));
}

Event::~Event() {
	// might not be useful
	Step* to_delete_step = nullptr;
	while (!finishedSteps.empty()) {
		to_delete_step = finishedSteps.front().first;
		finishedSteps.pop();
		delete to_delete_step;
	}

	// just in case there's something in eventSteps
	while (!eventSteps.empty()) {
		to_delete_step = eventSteps.front().first;
		eventSteps.pop();
		delete to_delete_step;
	}
}

bool Event::startEvent()
{
	isRunning = true;
	if (currentEvent.first == nullptr) {
		nextEvent();
	}
	return isRunning;
}

void Event::runEvent(float elapsedTime)
{
	if (isRunning) {
		// for debugging
		// Step.run returns true if the event is still happening and false if it's not!
		if (!currentEvent.first->run(elapsedTime, *currentEvent.second)) {
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
	Step* to_delete = currentEvent.first; // want to deallocate the previously done step!
	//// might not be useful
	//if (currentEvent != nullptr) {
	//	finishedSteps.push(currentEvent);
	//}
	if (!eventSteps.empty()) {
		currentEvent = eventSteps.front();
		eventSteps.pop();
		eventType = currentEvent.first->getType();
		isRunning = true;
	}
	else {
		isRunning = false;
		currentEvent.first = nullptr;
		currentEvent.second = nullptr;
	}
	if (to_delete != nullptr) {
		delete to_delete;
	}
}

void Event::addEvents(Step* step, Actor* actor) 
{ 
	eventSteps.push(std::make_pair(step, actor));
}

// might not be useful
void Event::reloadEvent() 
{ 
	for (int i = 0; i < finishedSteps.size(); i++) {
		eventSteps.push(finishedSteps.front());
		finishedSteps.pop();
	}
}


// Audrey Edit: Adding Event class functionalities //

// *************** End Audrey Edit *************** //

// Audrey Edit: Deleting Dynamic Allocations //

// ************ End Audrey Edit ************ //