#pragma once
#include "Menu.h"

class BlockMenu : public Menu {
public:
	BlockMenu(int numCols);
	BlockMenu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound &bleep, sf::Texture backgroundTexture);
	~BlockMenu();

	virtual void setPosition(Tile pos);
	virtual void moveCursor(bool downRight, bool vertical, std::string type, int previousSelection);
	virtual void setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite *background, sf::Sound *bleep);

private:
	int numCols;
};
