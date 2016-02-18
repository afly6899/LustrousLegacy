#ifndef UI_H_
#define UI_H_

#include <SFML/Graphics.hpp>

class UI : public sf::Drawable {
public:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void setPosition(sf::Vector2f pos) = 0;
	virtual void setVisible(bool visibility) {
		visible = visibility;
	}
	virtual bool isVisible() {
		return visible;
	}
	virtual void update(sf::Vector2f pos, float elapsedTime) = 0;
	virtual std::string getClass() = 0;
	sf::Text createOption(std::string option_name, const sf::Font& font);
	void clearStyle(sf::Text& string);
	void selected(sf::Text& string);

private: 
	bool visible = false;
};
#endif