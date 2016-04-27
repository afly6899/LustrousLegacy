#pragma once
#include "Menu.h"

class HorizontalMenu : public Menu {
public:
	HorizontalMenu();
	HorizontalMenu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound &bleep, sf::Texture backgroundTexture);
	~HorizontalMenu();

	virtual void setPosition(Tile pos);
	virtual void setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite *background, sf::Sound *bleep);

private:

};