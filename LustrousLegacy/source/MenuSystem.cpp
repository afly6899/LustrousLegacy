#include <fstream>
#include <sstream>
#include <iostream>
#include "MenuSystem.h"
#include "MenuFunctions.h"

MenuSystem::MenuSystem(std::string filename, const sf::RenderWindow & window)
{
	std::ifstream file(filename);
	if (file) {
		std::string infostr;
		if (getline(file, infostr)) {
			std::stringstream essentials(infostr);
			std::string backgroundFile, bleepFile, fontFile;
			essentials >> backgroundFile >> bleepFile >> fontFile;
			sf::Font *font = new sf::Font;
			if (!font->loadFromFile("resources/font/" + fontFile)) {
				std::cout << "Cannot load Font" << std::endl;
			}
			sf::Texture *texture = new sf::Texture;
			if (backgroundFile == "None") { backgroundFile = "default_background.png"; }
			if (!texture->loadFromFile("resources/textures/" + backgroundFile)) {
				std::cout << "Cannot load Background Texture" << std::endl;
				getchar();
			}
			sf::Sound *bleep = new sf::Sound;
			sf::SoundBuffer *bleep1 = new sf::SoundBuffer;
			if (!bleep1->loadFromFile("resources/audio/" + bleepFile)) {
				std::cout << "Cannot load Sound" << std::endl;
			}
			bleep->setBuffer(*bleep1);
			toKeep = { bleep1, font, texture };

			while (getline(file, infostr)) {
				std::stringstream info(infostr);
				unsigned int menuType;
				float widthOffset, heightOffset, startX, startY;
				std::string menuName, option;
				std::vector<sf::Text*> optionNames;
				info >> menuType >> menuName >> widthOffset >> heightOffset >> startX >> startY;

				while (info >> option) {
					int index = option.find('_');
					if (index != std::string::npos) {
						functions[option.substr(0, index)] = option.substr(index + 1);
						option = option.substr(0, index);
					}
					previous[option] = menuName;
					sf::Text *text = new sf::Text(option, *font, 24);
					text->setOrigin(text->getLocalBounds().width * .5, text->getLocalBounds().height *.5);
					optionNames.push_back(text);
				}

				sf::Sprite *backgroundSprite = new sf::Sprite(*texture);

				switch (menuType) {
				case MenuType::Regular:
					menus[menuName] = (new VerticalMenu());
					break;
				case MenuType::Horizontal:
					menus[menuName] = (new HorizontalMenu());
					break;
				default:
					menus[menuName] = (new BlockMenu(menuType));
					break;
				}
				backgroundSprite->setColor(sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255));
				menus[menuName]->setInfo(optionNames, sf::Vector2f(startX, startY), sf::Vector2u(window.getSize().x * widthOffset, window.getSize().y * heightOffset), backgroundSprite, bleep);
			}
		}
		currentMenu = "Start";
		previous[currentMenu] = "None";
		file.close();
	}
	else {
		std::cout << "Could not open file" << std::endl;
	}
}

MenuSystem::~MenuSystem()
{
	if (toKeep.buffer == nullptr) {
		delete toKeep.buffer;
		delete toKeep.backgroundTexture;
		delete toKeep.font;
	}
	for (auto menu : menus) {
		if (menu.first != "None") {
			Menu *to_delete = menu.second;
			delete to_delete;
		}
	}
}

void MenuSystem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (showMenu) {
		std::cout << "current menu is " << currentMenu << std::endl;
		std::string tempPrev = previousMenu;
		while (tempPrev != "None") {
			target.draw(*menus.at(tempPrev));
			tempPrev = previous.at(tempPrev);
		}
		target.draw((*menus.at(currentMenu)));
	}
}

void MenuSystem::setPosition(Tile pos)
{
	for (auto menu : menus) {
		menu.second->setPosition(pos);
	}
}

void MenuSystem::update(sf::Vector2f position, float elapsedTime) {}

void MenuSystem::handleInput(sf::Event e)
{
 	if (showMenu && e.type == sf::Event::KeyPressed) {
		switch (e.key.code) {
		case sf::Keyboard::Return:
			switchMenu(getNextMenu());
			break;
		case sf::Keyboard::Space:
			if (previousMenu == "None") {
				showMenu = false;
				setVisible(false);
			}
			else {
				switchMenu(previousMenu);
			}
			break;
		case sf::Keyboard::Up:
			menus[currentMenu]->moveCursor(false, true);
			break;
		case sf::Keyboard::Left:
			menus[currentMenu]->moveCursor(false, false);
			break;
		case sf::Keyboard::Down:
			menus[currentMenu]->moveCursor(true, true);
			break;
		case sf::Keyboard::Right:
			menus[currentMenu]->moveCursor(true, false);
			break;
		}
	}
	else {
		if (e.key.code == sf::Keyboard::Space) {
			menus[currentMenu]->resetCursor();
			switchMenu("Start");
			setVisible(true);
			showMenu = true;
		}
	}
}

void MenuSystem::switchMenu(std::string newMenu)
{
	if (menus.find(newMenu) == menus.end()) {
		if (functions.find(newMenu) == functions.end()) {
			std::cout << "Option is not available" << std::endl;
		}
		else {
			menuFunctions[functions[newMenu]](newMenu);
		}
	}
	else {
		currentMenu = (menus.find(newMenu) == menus.end()) ? "Start" : newMenu;
		previousMenu = previous[currentMenu];
		toggleStyle(*menus[currentMenu]->getSelectedText());
	}
}

std::string MenuSystem::getNextMenu()
{
	return menus[currentMenu]->getSelected();
}
