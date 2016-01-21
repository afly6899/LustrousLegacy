#ifndef PAUSE_H_
#define PAUSE_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "UI.h"

namespace sf {

	class Texture;
	class Font;
	class Text;
}

class Pause : public UI {
public:

	Pause(const sf::Font& font, sf::Vector2u window_size, int font_size = 18);
	virtual ~Pause();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setSize(sf::Vector2f pos);
	void setPosition(sf::Vector2f pos);
	void setVisible(bool visibility);;
	bool isVisible();
	virtual void update(sf::Vector2f pos, float elapsedTime);

private:

	sf::RectangleShape rectOverlay;
	sf::Vector2f originalPos;
	sf::Text pauseText;
	bool is_visible = false;
};
#endif