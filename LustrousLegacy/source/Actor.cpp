#include "Actor.h"
#include "sfMath.h"

/*********************************************************************
\brief temp
*********************************************************************/
Actor::Actor(const sf::Texture& imagePath) :
	Pawn(imagePath) {
}

/*********************************************************************
\brief temp
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
\brief temp
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
		unsigned int dir = sfmath::vecTrueDirection(vec_dir);

		pastPosition = getPosition();
		pastDirection = getDirection();

		if (dir != Direction::Null)
			setDirection(dir);

		getSprite().move(sfmath::mul(actorSpeed, vec_dir));

		if (dir != Direction::Null)
			spriteAnimate(elapsedTime);
		else
			resetTextureRect();
	}
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
sf::FloatRect Actor::getCollisionBox() {
	sf::Vector2f center = getPosition();
	int offset = getSprite().getTextureRect().width / 2;
	return sf::FloatRect(center.x + offset, center.y + offset, offset, offset);
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

void Actor::addEventDialogue(std::string dialogue)
{
	eventDialogues.push(dialogue);
}

std::string Actor::nextEventDialogue()
{
	std::string next = eventDialogues.front();
	eventDialogues.pop();
	return next;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::disableMovement() {
	stop = true;
	time_counter = 0;
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

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::cycleMovement(float elapsedTime) {
	if (!targetPositions.empty()) {
		move(elapsedTime, (targetPositions[0]));
		if (getPosition() == targetPositions[0]) {
			//collided();
			sf::Vector2f temp = targetPositions[0];
			targetPositions.erase(targetPositions.begin());
			targetPositions.push_back(temp);
		}
	}
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::addTargetPosition(sf::Vector2f pos) {
	targetPositions.push_back(pos);
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::setScene(std::string scene_name) {
	actorScene = scene_name;
}

/*********************************************************************
\brief temp
*********************************************************************/
std::string Actor::getScene() {
	return actorScene;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Actor::setPastPosition(sf::Vector2f pos) {
	pastPosition = pos;
}