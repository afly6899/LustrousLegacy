#ifndef ACTOR_H_
#define ACTOR_H_

#include "Enums.h"
#include "Pawn.h"
#include <SFML/Graphics.hpp>

namespace sf {

	class Texture;
}

class Actor : public Pawn {
public:

	Actor(const sf::Texture& playerTexture);
	virtual ~Actor();
	virtual void move(float elapsedTime, int direction);
	void move_tile(float elapsedTime, unsigned int number_of_tiles, int direction);
	void setSpeed(int speed);
	void setCollision(bool collision);
	void setCollisionBox(int x, int y);
	void setStopCounter(int new_counter);

	sf::FloatRect getCollisionBox();
	sf::Vector2f getPastPosition();
	bool getCollision();
	bool is_moving();
	void collided();

private:
	int pastDirection;
	unsigned int tile_counter = 0;
	int actorSpeed = 4;
	int time_counter = 0;
	int stop_counter = 300;
	bool tile_move_set = false;
	bool movement_complete = true;
	bool collision = false;
	bool stop = false;
	sf::FloatRect collision_box;
	sf::Vector2f pastPosition;
	
	bool allowMovement(float elapsedTime);
};
#endif