#pragma once
#include "Step.h"

class TogetherStep : public Step
{
public:
	TogetherStep(std::vector<sf::Vector2f> positions, std::vector<Actor*> actors);
	~TogetherStep();

	virtual bool run(float elapsedTime, Actor& actor);

	// for debugging
	virtual std::string getType() { return type; }
private:
	std::vector<sf::Vector2f> positions;
	std::vector<Actor*> actors;

	// Debugging
	std::string type = "Together Move";
};

