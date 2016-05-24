#pragma once

#include <string>
#include "Step.h"

class MapStep : public Step {
public:
	MapStep(std::string &map_name, std::string map_to_be);
	virtual bool run(float elapsedTime, Actor& actor);

	// for debugging
	virtual std::string getType() { return type; }
private:
	std::string &mapName;
	std::string mapToBe;
	// for debugging
	std::string type = "Map";
};