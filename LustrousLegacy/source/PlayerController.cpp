#include "PlayerController.h"

/*********************************************************************
PlayerController default constructor.
*********************************************************************/
PlayerController::PlayerController() {}

/*********************************************************************
PlayerController destructor.
*********************************************************************/
PlayerController::~PlayerController() {}

/*********************************************************************
PlayerController get_input() functon.
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
PlayerController get_input() functon.
*********************************************************************/
void PlayerController::allow_input() {
	allow_player_input = true;
}

/*********************************************************************
PlayerController get_input() functon.
*********************************************************************/
void PlayerController::block_input() {
	allow_player_input = false;

}