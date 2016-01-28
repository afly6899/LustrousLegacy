#include "fader.h"

/*********************************************************************
\brief temp
*********************************************************************/
Fader::Fader() {
	rectFade.setSize(sf::Vector2f(800, 600));
	rectBlack = rectFade;
	rectBlack.setFillColor(sf::Color(0, 0, 0, 255));
}

/*********************************************************************
\brief temp
*********************************************************************/
Fader::~Fader() {}

/*********************************************************************
\brief temp
*********************************************************************/
void Fader::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(rectFade, states);
}

/*********************************************************************
\brief temp
*********************************************************************/
void Fader::setPosition(const sf::Vector2f position) {
	rectFade.setPosition(position.x - 400, position.y - 300);
	rectBlack.setPosition(position.x - 400, position.y - 300);
}

/*********************************************************************
\brief temp
*********************************************************************/
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

/*********************************************************************
\brief temp
*********************************************************************/
void Fader::resetFader() {
	fading = false;
	complete = false;
}

/*********************************************************************
\brief temp
*********************************************************************/
sf::RectangleShape Fader::blackScreen() {
	return rectBlack;
}

/*********************************************************************
\brief temp
*********************************************************************/
bool Fader::isComplete() {
	return complete;
}
