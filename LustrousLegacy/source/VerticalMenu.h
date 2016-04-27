#pragma once
#include "Menu.h"

class VerticalMenu : public Menu {
public:
	VerticalMenu();
	VerticalMenu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound &bleep, sf::Texture backgroundTexture);
	~VerticalMenu();

	virtual void setPosition(Tile pos);
	virtual void setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite *background, sf::Sound *bleep);

private:

};