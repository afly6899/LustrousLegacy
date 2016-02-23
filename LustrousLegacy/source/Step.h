#pragma once
#ifndef STEP_H_
#define STEP_H_

#include "Actor.h"

class Step {
public:

	Step() {};
	virtual ~Step() {};
	virtual bool run(float elapsedTime, Actor& actor) = 0;
	
	virtual std::string getType() { return ""; }
};
#endif