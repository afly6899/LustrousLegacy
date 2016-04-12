#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include "Pawn.h"
#include "UI.h"

typedef std::vector<std::string> OptionNames;

struct GeneralInfo
{
	int separation;
	sf::Vector2f startPos;
	sf::Vector2u windowSize;
};

class Menu : public UI {
public: 
	Menu(GeneralInfo info, OptionNames stroptions, sf::Font& font, sf::Texture &texture, sf::Sound &bleep, bool rightleft);
	~Menu();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void setPosition(sf::Vector2f pos);
	virtual void update(sf::Vector2f pos, float elapsedTime);
	virtual std::string getClass() { return "Menu"; };

	void moveCursor(bool downRight);

private:
	sf::Vector2f cursorAdjustment();

	Pawn cursor;
	int separation;
	int numOptions;
	int currentSelection;
	sf::Vector2f changeBy;
	sf::Sound &cursorBleep;
	sf::Vector2u windowSize;
	std::map<int, sf::Text> options;
	std::vector<sf::Vector2f> positions; // original pos is the first one
};