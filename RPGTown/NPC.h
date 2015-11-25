#ifndef NPC_H_
#define NPC_H_

#include <SFML/Graphics.hpp>

// Forward Declaration
namespace sf {

	class Texture;
}

class NPC : public sf::Drawable {
public:

	// Cursor default constructur, requires a cursorTexture reference
	NPC(const sf::Texture& imagePath_object);
	// Cursor destructor (virtual -> destroy derived, then destroy base class)
	virtual ~NPC();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f pos);
	// void setDirection();
	void hover(float elapsedTime);

private:

	sf::Sprite objSprite;
	sf::Vector2f objSource;
	int aniCounter = 0;
	int aniFrameDuration = 300;
};

#endif