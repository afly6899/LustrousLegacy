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
	virtual void move(float elapsedTime, sf::Vector2f pos);
	void setSpeed(int speed);
	void setCollision(bool collision);
	void setCollisionBox(int x, int y);

	sf::FloatRect getCollisionBox();
	sf::Vector2f getPastPosition();
	bool getCollision();
	bool is_moving();

private:
	int pastDirection;
	unsigned int tile_counter = 0;
	int actorSpeed = 4;
	bool tile_move_set = false;
	bool movement_complete = true;
	bool collision = false;
	sf::FloatRect collision_box;
	sf::Vector2f pastPosition;

};
#endif