#ifndef UI_H_
#define UI_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Pawn.h"

// Forward Declaration
namespace sf {

	class Texture;
	class Font;
	class Text;
}

class UI : public sf::Drawable {
public:

	UI();
	virtual ~UI();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setPosition(sf::Vector2f pos);
	void setVisible(bool visibility);
	bool isVisible();
	sf::Vector2f getPosition();

private:
	bool is_visible;
};
#endif