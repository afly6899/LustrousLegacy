#include "pause.h"

Pause::Pause(const sf::Font& font, int window_width, int window_height, int font_size) {
	pauseText.setFont(font);
	pauseText.setCharacterSize(font_size);
	pauseText.setString("Game is paused. Press 'esc' to resume.");
	pauseText.setOrigin(pauseText.getLocalBounds().width*.5, pauseText.getLocalBounds().height*.5);
	rectOverlay.setSize(sf::Vector2f(window_width, window_height));
	rectOverlay.setOrigin(rectOverlay.getLocalBounds().width*.5, rectOverlay.getLocalBounds().height*.5);
	rectOverlay.setFillColor(sf::Color(0, 0, 0, 150));
}

Pause::~Pause() {

}

void Pause::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(rectOverlay, states);
	target.draw(pauseText, states);

}

void Pause::setSize(sf::Vector2f pos)
{
	rectOverlay.setSize(pos);
	rectOverlay.setOrigin(rectOverlay.getLocalBounds().width*.5, rectOverlay.getLocalBounds().height*.5);
}

void Pause::setPosition(sf::Vector2f pos) {
	rectOverlay.setPosition(pos);
	pauseText.setPosition(pos);
}
