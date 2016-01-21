#ifndef ACTOR_H_
#define ACTOR_H_

#include "Enums.h"
#include "Pawn.h"
#include <SFML/Graphics.hpp>

// For testing
#include <queue>

namespace sf {

	class Texture;
}

class Actor : public Pawn {
public:

	Actor(const sf::Texture& playerTexture);
	virtual ~Actor();
	virtual void move(float elapsedTime, int direction);
	virtual void move(float elapsedTime, sf::Vector2f pos);
	void setSpeed(int speed);
	void setCollision(bool collision);
	void setCollisionBox(int x, int y);
	void setStopCounter(int new_counter);
	void faceActor(Actor& other_actor);
	sf::FloatRect getCollisionBox();
	sf::Vector2f getPastPosition();
	bool allowMovement(float elapsedTime);
	bool getCollision();
	bool is_moving();
	virtual void collided();
	void enableMovement();
	void disableMovement();
	virtual std::string getClass();

private:
	int pastDirection;
	unsigned int tile_counter = 0;
	int actorSpeed = 4;
	int time_counter = 0;
	int stop_counter = 3000;
	bool tile_move_set = false;
	bool movement_complete = true;
	bool collision = false;
	bool stop = false;
	sf::FloatRect collision_box;
	sf::Vector2f pastPosition;

	std::queue<sf::Vector2f> targetPositions;

};
#endif