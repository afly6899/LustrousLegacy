#pragma once
#ifndef STEP_H_
#define STEP_H_

#include "Enums.h"
#include "Event.h"
#include <SFML/Graphics.hpp>

namespace sf {

	class Texture;
}

class Step {
public:

	Step();
	virtual ~Step();
	virtual void run() = 0;
	virtual bool is_running();
	virtual bool set_running(bool flag);

private: 
	bool is_running = false;
	
};
#endif