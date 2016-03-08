#pragma once
#ifndef MOVESTEP_H_
#define MOVESTEP_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include "Step.h"


class MoveStep : public Step{
public:
	MoveStep(std::vector<sf::Vector2f> positions);
	virtual ~MoveStep();
	virtual bool run(float elapsedTime, Actor& actor);

	// for debugging
	virtual std::string getType() { return type; }

private: 
	std::vector<sf::Vector2f> positions;
	sf::Vector2f current_target;
	
	// for debugging
	std::string type = "Move";
};
#endif

//std::map<int, bool> test;
//std::map<int, event> events;
//// key is ID
//if (test[0]) {
//	runEvent(events[0]);
//}
//
//std::map<int<std::vector<Events>> events
//int counter = 0
//if (outside_boolean_that_tells_event_to_happen) {
//	for event in events[counter]{
//	runEvent(event);
//	}
//	counter++;
//}