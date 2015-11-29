#ifndef FADER_H_
#define FADER_H_

#include <SFML/Graphics.hpp>

class Fader : public sf::Drawable {
public:

	Fader();
	virtual ~Fader();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setPosition(const sf::Vector2f position);
	void performFade(int fade_type, int speed);
	void resetFader();
	sf::RectangleShape blackScreen();
	bool isComplete();

private:

	sf::RectangleShape rectFade;
	sf::RectangleShape rectBlack;
	bool fading = false;
	bool complete = false;
	int fader_type;
	int fader;
};

#endif