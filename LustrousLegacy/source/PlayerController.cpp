#include "PlayerController.h"

/*********************************************************************
\brief temp
*********************************************************************/
PlayerController::PlayerController() {}

/*********************************************************************
\brief temp
*********************************************************************/
PlayerController::~PlayerController() {}

/*********************************************************************
\brief temp
*********************************************************************/
int PlayerController::get_input() {

	if (playerDirection != Direction::Null)
		pastPlayerDirection = playerDirection;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			playerDirection = Direction::NorthWest;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			playerDirection = Direction::NorthEast;
		else
			playerDirection = Direction::North;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			playerDirection = Direction::SouthWest;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			playerDirection = Direction::SouthEast;
		else
			playerDirection = Direction::South;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			playerDirection = Direction::NorthEast;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			playerDirection = Direction::SouthEast;
		else
			playerDirection = Direction::East;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			playerDirection = Direction::NorthWest;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			playerDirection = Direction::SouthWest;
		else
			playerDirection = Direction::West;
	}
	else
		playerDirection = Direction::Null;

	return playerDirection;
}

/*********************************************************************
\brief temp
*********************************************************************/
void PlayerController::allow_input() {
	allow_player_input = true;
}

/*********************************************************************
\brief temp
*********************************************************************/
void PlayerController::block_input() {
	allow_player_input = false;

}