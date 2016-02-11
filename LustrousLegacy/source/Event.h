#pragma once

#include <initializer_list>
#include <queue>
#include "Step.h"
// Want to have an event class that takes in a queue of steps and can process each step?

class Event
{
public:
	Event(std::queue<Step*> steps);
	Event(std::initializer_list<Step*> steps);
	~Event();

	bool startEvent();
	void runEvent(float elapsedTime, Actor & actor);
	void pauseEvent();
	void nextEvent();
	
	// Accessors
	bool eventIsRunning() { return isRunning; }
	bool finishedEvents() { return eventSteps.empty(); }

	// Just for now - temp
	void addEvents(Step* step) { eventSteps.push(step); }

private:
	std::queue<Step*> eventSteps;
	Step* currentEvent = nullptr;
	bool isRunning = false;
};