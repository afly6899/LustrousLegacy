#include "fader.h"

// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
Fader::Fader() {
	rectFade.setSize(sf::Vector2f(800, 600));
}

// Player virtual destructor;
Fader::~Fader() {

}

// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
void Fader::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(rectFade, states);
}

void Fader::setPosition(const sf::Vector2f position) {
	rectFade.setPosition(position.x - 400, position.y - 300);

}

void Fader::performFade(int fade_type, int speed) {
	
	
	if (!fading)
	{
		fader_type = fade_type;
		if (fader_type == 0) {
			fader = 255;
		}
		else if (fader_type == 1) {
			fader = 0;
		}

		rectFade.setFillColor(sf::Color(0, 0, 0, fader));
		fading = true;

	}
	else if (fading && !complete) {

		if (fader_type == 0) {
			fader -= speed;
			if (fader < 0)
				fader = 0;
		}
		else if(fader_type == 1) {
			fader += speed;
			if (fader > 255)
				fader = 255;
		}

		rectFade.setFillColor(sf::Color(0, 0, 0, fader));

		if (fader_type == 0 && fader == 0)
			complete = true;

		else if (fader_type == 1 && fader == 255)
			complete = true;
	}
}

void Fader::resetFader() {
	fading = false;
	complete = false;
}

bool Fader::isComplete() {
	return complete;
}
