//#include "InventoryMenu.h"
//
//InventoryMenu::InventoryMenu(int numCols)
//: Menu(numCols), numCols(numCols) {
//}
//
//InventoryMenu::InventoryMenu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound & bleep, sf::Texture backgroundTexture)
//: Menu(names, start, size, bleep, backgroundTexture) {
//	numCols = 1;
//	cpos[0] = size.x; // for the col with the items
//	cpos[1] = size.y / numOptions;
//	scpos[0] = size.x / 6; // for the col with the numbers
//	scpos[1] = size.y / 2;
//	setPosition(start);
//}
//
//InventoryMenu::~InventoryMenu() {}
//
//void InventoryMenu::setPosition(Tile pos)
//{
//	int numRows = numOptions / numCols;
//	int heightOffset = (optionNames.at(0)->getLocalBounds().height / 2);
//	int counter = 0;
//	if (positions.empty()) {
//		for (int row = 0; row < numRows; row++) {
//			for (int col = 0; col < numCols; col+=2) {
//				float column1x = pos.x + col*cpos[0] + cpos[0] / 6 + optionNames.at(counter)->getLocalBounds().width / 2;
//				float columny = (pos.y + row * cpos[1] + cpos[1] / 2 - heightOffset);
//				positions.push_back(sf::Vector2f(column1x, columny));
//				positions.push_back(sf::Vector2f(column1x + col*scpos[0] + scpos[0] / 6 + optionNames.at(counter)->getLocalBounds().width / 2, columny));
//				optionNames[counter]->setPosition(positions[counter]);
//				counter++;
//			}
//		}
//	}
//	else if (pos != positions.at(0)) {
//		for (int row = 0; row < numRows / numCols; row++) {
//			for (int col = 0; col < numCols; col+=2) {
//				float column1x = pos.x + col*cpos[0] + cpos[0] / 6 + optionNames.at(counter)->getLocalBounds().width / 2;
//				float columny = (pos.y + row * cpos[1] + cpos[1] / 2 - heightOffset);
//				positions.push_back(sf::Vector2f(column1x, columny));
//				positions.push_back(sf::Vector2f(column1x + cpos[0]/6 + optionNames.at(counter)->getLocalBounds().width / 2, columny));
//				optionNames[counter]->setPosition(positions[col]);
//				counter++;
//			}
//		}
//	}
//}
//
//void InventoryMenu::moveCursor(bool downRight, bool vertical, std::string type, int previousSelection)
//{
//	previousSelection = currentSelection;
//	int currentRow = currentSelection / numCols;
//	if (vertical) {
//		if (downRight) {
//			currentSelection = (currentSelection + numCols) % numOptions;
//		}
//		else {
//			if (currentRow == 0) {
//				currentSelection = numOptions - (1 + (currentSelection + 1) % numCols);
//			}
//			else {
//				currentSelection -= numCols;
//			}
//		}
//	}
//	else {
//
//		if (downRight) {
//			currentSelection += 2;
//			if (currentSelection % numCols == 0 || currentSelection > numOptions) {
//				currentSelection = currentRow * numCols;
//			}
//		}
//		else {
//			currentSelection -= 2;
//			if (currentSelection % numCols == 1 || currentSelection < 0) {
//				currentSelection = currentRow * numCols + 1;
//			}
//		}
//	}
//	Menu::moveCursor(downRight, vertical, "BlockMenu", previousSelection);
//}
//
//void InventoryMenu::setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite *background, sf::Sound *bleep) {
//	Menu::setInfo(names, start, size, background, bleep);
//	Menu::setInfo(names, start, size, background, bleep);
//	cpos[0] = size.x / numCols;
//	cpos[1] = size.y / (numOptions / numCols);
//	scpos[0];
//	scpos[1];
//	setPosition(start);
//}