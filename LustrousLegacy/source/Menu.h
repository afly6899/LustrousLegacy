#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include "UI.h"

class Menu : public UI {
public: 
	Menu(const std::vector<std::string> options, const sf::Font& font, const sf::Vector2u window_size, sf::Sound& sfx);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states);
	virtual void setPosition(sf::Vector2f pos);
	virtual void update(sf::Vector2f pos, float elapsedTime);


	virtual std::string getClass() { return "Menu"; };

private:
	std::map<int, sf::Text> options;
	sf::Sound& cursorBleep;
	sf::Vector2u window_size;
	sf::Vector2f originalPos;
	sf::Vector2f cursor_originalPos;
};