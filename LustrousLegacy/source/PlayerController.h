#pragma once
#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H__H_

#include "Enums.h"
#include <SFML/Graphics.hpp>

namespace sf {

	class Keyboard;
}

class PlayerController {
public:

	PlayerController();
	~PlayerController();
	int get_input();
	void allow_input();
	void block_input();

private:
	unsigned int playerSpeed;
	unsigned int playerDirection;
	unsigned int pastPlayerDirection;
	bool allow_player_input;
};
#endif