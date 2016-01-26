#pragma once
#ifndef MOVESTEP_H_
#define MOVESTEP_H_

#include <vector>
#include "Step.h"
#include "SFML\Graphics.hpp"


class MoveStep : public Step{
public:

	MoveStep(std::vector<sf::Vector2f> positions);
	virtual ~MoveStep();
	virtual bool run(float elapsedTime, Actor& actor);

private: 
	std::vector<sf::Vector2f> positions;
	sf::Vector2f current_target;
	
};
#endif