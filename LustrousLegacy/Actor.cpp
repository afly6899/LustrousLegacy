#include <iostream> //for debugging
#include "Actor.h"

/*********************************************************************
Actor class default constructor.
\brief Requires a texture for instantiation.
*********************************************************************/
Actor::Actor(const sf::Texture& imagePath) :
	Pawn(imagePath) {
}

/*********************************************************************
Actor class virtual destructor.
*********************************************************************/
Actor::~Actor() {
}

/*********************************************************************
\brief Moves the player based on provided speed and direction.
\param Elapsed Time, Direction
*********************************************************************/
void Actor::move(float elapsedTime, int direction) {
	if (!stop || allowMovement(elapsedTime)) {
		pastPosition = getPosition();
		pastDirection = getDirection();

		if (direction != Direction::Null)
			setDirection(direction);

		switch (direction) {
		case Direction::South: getSprite().move(0, actorSpeed);
			break;
		case Direction::East: getSprite().move(actorSpeed, 0);
			break;
		case Direction::West: getSprite().move(-actorSpeed, 0);
			break;
		case Direction::North: getSprite().move(0, -actorSpeed);
			break;
		case Direction::SouthEast: getSprite().move(actorSpeed, actorSpeed);
			break;
		case Direction::NorthEast: getSprite().move(actorSpeed, -actorSpeed);
			break;
		case Direction::SouthWest: getSprite().move(-actorSpeed, actorSpeed);
			break;
		case Direction::NorthWest: getSprite().move(-actorSpeed, -actorSpeed);
			break;
		case Direction::Null:
			break;
		}

		if (direction != Direction::Null)
			spriteAnimate(elapsedTime);
		else
			resetTextureRect();
	}
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::move_tile(float elapsedTime, unsigned int number_of_tiles, int direction) {

	if (movement_complete) {
		tile_counter = number_of_tiles*Tilesize;
		movement_complete = false;
	}
		
	if (tile_counter != 0) {
		move(elapsedTime, direction);
		tile_counter--;
	}
	else if(tile_counter == 0)
		movement_complete = true;
}

/*********************************************************************
\brief temp
*********************************************************************/
bool Actor::is_moving() {
	return !movement_complete;
}

/*********************************************************************
\brief temp
*********************************************************************/
sf::Vector2f Actor::getPastPosition() {
	return pastPosition;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::setSpeed(int speed) {
	actorSpeed = speed;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::setCollision(bool col) {
	collision = col;
}

/*********************************************************************
\brief temp
*********************************************************************/
bool Actor::getCollision() {
	return collision;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::setCollisionBox(int x, int y) {
	sf::Vector2f center = getPosition();
	collision_box = sf::FloatRect(center.x + x, center.y + y, x, y);
}

/*********************************************************************
\brief temp
*********************************************************************/
sf::FloatRect Actor::getCollisionBox() {
	return collision_box;
}



////////////////////////////////////TESTINGGGGGGGGGGGGGGGGGGGGGGGGGG///////////////////////////////////////
void Actor::collided() {
	std::cout << "Collided!" << std::endl;
	setPosition(pastPosition);
	stop = true;
	time_counter = 0;
}

bool Actor::allowMovement(float elapsedTime) {
	time_counter += elapsedTime;
	if (time_counter >= stop_counter) {
		stop = false;
	}
	return !stop;
}
////////////////////////////////////TESTINGGGGGGGGGGGGGGGGGGGGGGGGGG///////////////////////////////////////