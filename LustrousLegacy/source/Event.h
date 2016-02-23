#pragma once

#include <initializer_list>
#include <queue>
#include "Step.h"

class Event
{
public:
	Event(std::queue<std::pair<Step*, Actor*>> steps);
	Event(std::initializer_list<Step*> steps, std::initializer_list<Actor*> actors);
	~Event();

	bool startEvent();
	void runEvent(float elapsedTime);
	//void runEvent(float elapsedTime, Actor * actors);
	void pauseEvent();
	void nextEvent();
	
	// Accessors
	bool eventIsRunning() { return isRunning; }
	bool finishedEvents() { return eventSteps.empty(); }

	// Just for now - temp
	void addEvents(Step* step, Actor* actor);
	// might not be useful
	void reloadEvent();

	// for debugging
	std::string getEventType() { return eventType; }

private:
	// might not be useful
	std::queue<std::pair<Step*, Actor*>> finishedSteps;
	std::queue<std::pair<Step*, Actor*>> eventSteps;
	std::pair<Step*, Actor*> currentEvent;
	bool isRunning = false;

	// for debugging
	std::string eventType = "None";
};