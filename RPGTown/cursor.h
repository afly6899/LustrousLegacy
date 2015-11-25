#ifndef CURSOR_H_
#define CURSOR_H_

#include <SFML/Graphics.hpp>

// Forward Declaration
namespace sf {

	class Texture;
}

class Cursor : public sf::Drawable {
public:

	// Cursor default constructur, requires a cursorTexture reference
	Cursor(const sf::Texture& imagePath_cursor);
	// Cursor destructor (virtual -> destroy derived, then destroy base class)
	virtual ~Cursor();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f pos);
	void animate(float elapsedTime);

private:

	sf::Sprite cursorSprite;
	sf::Vector2f cursorSource;
	int aniCounter = 0;
	int aniFrameDuration = 300;
};

#endif