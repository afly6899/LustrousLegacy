#ifndef PAWN_H_
#define PAWN_H_

#include "Enums.h"
#include <SFML/Graphics.hpp>

namespace sf {

	class Texture;
}

class Pawn : public sf::Drawable {

public:

	//Pawn(const sf::Texture& playerTexture);
	Pawn(const sf::Texture& playerTexture, int numColumns = 3);
	virtual ~Pawn();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	unsigned int getDirection();
	int getDepth();
	void setDirection(unsigned int dir);
	void setPosition(sf::Vector2f pos);
	void setFrameDuration(unsigned int frame_duration);
	void spriteAnimate(float elapsedTime);
	void changeSprite(const sf::Texture& playerTexture);
	void resetTextureRect();
	sf::Sprite& getSprite();
	sf::Vector2f getPosition();
	virtual std::string getClass();

protected:

	sf::Sprite pawnSprite;
	sf::Vector2u spriteSource;
	unsigned int spriteSize;
	unsigned int spriteDirection;
	unsigned int aniCounter = 0;
	unsigned int aniFrameDuration = 200;
};
#endif