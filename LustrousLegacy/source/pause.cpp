#include "Pause.h"

/*********************************************************************
\brief temp
*********************************************************************/
Pause::Pause(const sf::Font& font, sf::Vector2u window_size, int font_size) {
	
	pauseText.setCharacterSize(font_size);
	pauseText = createOption("Game is paused. Press 'esc' to resume.", font);

	rectOverlay.setSize(sf::Vector2f(window_size.x, window_size.y));
	rectOverlay.setOrigin(rectOverlay.getLocalBounds().width*.5, rectOverlay.getLocalBounds().height*.5);
	rectOverlay.setFillColor(sf::Color(0, 0, 0, 150));
}

/*********************************************************************
\brief temp
*********************************************************************/
Pause::~Pause() {

}

/*********************************************************************
\brief temp
*********************************************************************/
void Pause::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(rectOverlay, states);
	target.draw(pauseText, states);

}

/*********************************************************************
\brief temp
*********************************************************************/
void Pause::setSize(sf::Vector2f pos)
{
	rectOverlay.setSize(pos);
	rectOverlay.setOrigin(rectOverlay.getLocalBounds().width*.5, rectOverlay.getLocalBounds().height*.5);
}

/*********************************************************************
\brief temp
*********************************************************************/
void Pause::setPosition(sf::Vector2f pos) {
	rectOverlay.setPosition(pos);
	pauseText.setPosition(pos);
}

/*********************************************************************
\brief temp
*********************************************************************/
void Pause::setVisible(bool visibility) {
	is_visible = visibility;
}

/*********************************************************************
\brief temp
*********************************************************************/
bool Pause::isVisible() {
	return is_visible;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Pause::update(sf::Vector2f pos, float elapsedTime) {
	if (pos != originalPos) {
		setPosition(pos);
		originalPos = pos;
	}
}