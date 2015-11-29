#ifndef PAUSE_H_
#define PAUSE_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace sf {

	class Texture;
	class Font;
	class Text;
}

class Pause : public sf::Drawable {
public:

	Pause(const sf::Font& font, int window_width, int window_height, int font_size = 18);
	virtual ~Pause();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setSize(sf::Vector2f pos);
	void setPosition(sf::Vector2f pos);

private:

	sf::RectangleShape rectOverlay;
	sf::Text pauseText;
};
#endif