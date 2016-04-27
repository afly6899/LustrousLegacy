#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include "Pawn.h"
#include "UI.h"

typedef std::vector<std::string> Names;
typedef sf::Vector2f Tile;

class Menu : public UI {
public:
	Menu();
	Menu(int number);
	Menu(Names names, sf::Vector2f start, sf::Vector2u size, sf::Sound &bleep, sf::Texture backgroundTexture);
	~Menu();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void update(sf::Vector2f position, float elapsedTime);
	virtual void setPosition(Tile pos) = 0;

	void resetCursor();
	virtual void moveCursor(bool downRight, bool vertical, std::string type = "Menu", int previousSelection = 0);
	std::string getSelected();
	sf::Text *getSelectedText();

	virtual void setInfo(std::vector<sf::Text*> names, sf::Vector2f start, sf::Vector2u size, sf::Sprite *background, sf::Sound *bleep);

protected:
	float cpos[2];
	int numOptions;
	sf::Vector2u size;
	int currentSelection;
	std::vector<sf::Text*> optionNames;
	std::vector<sf::Vector2f> positions;

private:
	sf::Sound *cursorBleep;
	sf::Sprite *background;
};
