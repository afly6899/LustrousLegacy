#ifndef PAUSE_H_
#define PAUSE_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Forward Declaration
namespace sf {

	class Texture;
	class Font;
	class Text;
}

class Pause : public sf::Drawable {
public:

	// Cursor default constructur, requires a cursorTexture reference
	Pause(const sf::Texture& imagePath_title, const sf::Texture& imagePath_bgtitle, const sf::Texture& imagePath_cursor, const sf::Font& font, sf::Sound& bleep);
	// Cursor destructor (virtual -> destroy derived, then destroy base class)
	virtual ~Pause();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void move(int num_of_selections, int up_or_down);
	// returns an integer value representing the current selection
	int getSelection();
	void setPosition(int x, int y);
	void animate(float elapsedTime);

private:

	sf::Text textSelections;
	sf::Sprite cursorSprite;
	sf::Sprite titleSprite;
	sf::Sprite bgtitleSprite;
	sf::Vector2f cursorSource;
	sf::Vector2f originalPos;
	sf::Sound& cursorBleep;
	int selection = 1;
	int aniCounter = 0;
	int aniFrameDuration = 300;
};

#endif