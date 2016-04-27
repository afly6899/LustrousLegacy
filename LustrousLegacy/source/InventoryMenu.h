//#pragma once
//#include "BlockMenu.h"
//
//class InventoryMenu : public Menu
//{
//public:
//	InventoryMenu(int numCols);
//	InventoryMenu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound &bleep, sf::Texture backgroundTexture);
//	~InventoryMenu();
//
//	virtual void setPosition(Tile pos);
//	virtual void moveCursor(bool downRight, bool vertical, std::string type, int previousSelection);
//	virtual void setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite *background, sf::Sound *bleep);
//
//private:
//	int numCols;
//	float scpos[2];
//};