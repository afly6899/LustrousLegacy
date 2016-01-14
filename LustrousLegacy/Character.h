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
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	PlayerController& getPlayerController();
private:
	PlayerController movement;
};
#endif