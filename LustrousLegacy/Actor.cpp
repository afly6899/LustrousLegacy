#include "Actor.h"
#include "sfMath.h"

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
\brief temp
*********************************************************************/
bool Actor::allowMovement(float elapsedTime) {
	time_counter += elapsedTime;
	if (time_counter >= stop_counter) {
		stop = false;
	}
	return !stop;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::setStopCounter(int new_counter) {
	stop_counter = new_counter;
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
void Actor::move(float elapsedTime, sf::Vector2f pos) {
	if (!stop || allowMovement(elapsedTime)) {
		sf::Vector2f vec_dir = sfmath::Normalize(pos - getPosition());
		unsigned int dir = sfmath::vecDirection(vec_dir);

		pastPosition = getPosition();
		pastDirection = getDirection();

		if (dir != Direction::Null)
			setDirection(dir);

		getSprite().move(vec_dir);

		if (dir != Direction::Null)
			spriteAnimate(elapsedTime);
		else
			resetTextureRect();
	}
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

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::collided() {
	resetTextureRect();
	setPosition(pastPosition);
	stop = true;
	time_counter = 0;
}

/*********************************************************************
\brief temp
*********************************************************************/
std::string Actor::getClass() {
	return "Actor";
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::disableMovement() {
	stop = true;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::enableMovement() {
	stop = false;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::faceActor(Actor& other_actor) {
	switch (other_actor.getDirection()) {
	case(Direction::North) :
		setDirection(Direction::South);
		break;
	case(Direction::West) :
		setDirection(Direction::East);
		break;
	case(Direction::East) :
		setDirection(Direction::West);
		break;
	case(Direction::South):
		setDirection(Direction::North);
		break;
	}
	resetTextureRect();
}


// Testing
void Actor::addTargetPosition(sf::Vector2f pos) {
	targetPositions.push(pos);
}

void Actor::addTargetPositions(std::queue<sf::Vector2f> pos) {
	while (!pos.empty()) {
		targetPositions.push(pos.front);
		pos.pop();
	}
}