#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include "UI.h"
#include "Menu.h"
#include "VerticalMenu.h"
#include "HorizontalMenu.h"
#include "BlockMenu.h"

enum MenuType {
	Regular = 0,
	Horizontal,
	Block
};

struct MenuExtraEssentials
{
	sf::SoundBuffer *buffer;
	sf::Font *font;
	sf::Texture *backgroundTexture;
};

class MenuSystem : public UI {
public:
	MenuSystem(std::string filename, const sf::RenderWindow &window);
	~MenuSystem();

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	virtual void setPosition(Tile pos);
	virtual void update(sf::Vector2f position, float elapsedTime);

	void handleInput(sf::Event e);
	void switchMenu(std::string newMenu);

private:
	std::string getNextMenu();

	bool showMenu;
	std::string currentMenu;
	std::string previousMenu;
	MenuExtraEssentials toKeep;
	std::map<std::string, Menu*> menus;
	std::map<std::string, std::string> previous;
	std::map<std::string, std::string> functions;
};

