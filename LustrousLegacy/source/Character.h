#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Enums.h"
#include "Actor.h"
#include "PlayerController.h"
#include <SFML/Graphics.hpp>

namespace sf {

	class Texture;
}

class Character : public Actor {
public:

	Character(const sf::Texture& playerTexture);
	virtual ~Character();
	sf::Vector2f getViewArm();
	virtual std::string getClass();
	void collided();

	PlayerController controller;
};
#endif