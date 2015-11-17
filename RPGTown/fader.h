#ifndef FADER_H_
#define FADER_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Fader : public sf::Drawable {
public:

	Fader();
	virtual ~Fader();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setPosition(const sf::Vector2f position);
	void performFade(int fade_type, int speed);
	void resetFader();
	bool isComplete();

private:

	sf::RectangleShape rectFade;
	bool fading = false;
	bool complete = false;
	int fader_type;
	int fader;
};

#endif