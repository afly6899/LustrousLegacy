#ifndef UI_H_
#define UI_H_

class UI : public sf::Drawable {
public:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void setPosition(sf::Vector2f pos) = 0;
	virtual void setVisible(bool visibility) = 0;
	virtual bool isVisible() = 0;
	virtual sf::Vector2f getPosition() = 0;

};
#endif