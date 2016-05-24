#include "MapStep.h"

MapStep::MapStep(std::string &map_name, std::string map_to_be)
: mapName(map_name), mapToBe(map_to_be) {
}

bool MapStep::run(float elapsedTime, Actor & actor)
{
	mapName = mapToBe;
	return false;
}
