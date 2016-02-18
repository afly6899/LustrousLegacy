#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Pawn.h"
#include "UI.h"
#include <map>

// Forward Declaration
namespace sf {

	class Texture;
	class Font;
	class Text;
}

class BattleSystem : public UI {
public:

	BattleSystem(const sf::Font& font, const sf::Vector2u window_size, const sf::Texture& imagePath_title, sf::Sound& sfx);
	virtual ~BattleSystem();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void change_selection(int up_or_down);
	int getSelection();
	virtual void setPosition(sf::Vector2f pos);
	virtual void update(sf::Vector2f pos, float elapsedTime);
	std::string BattleSystem::getClass();

private:

	sf::Sound& cursorBleep;
	sf::Vector2u window_size;
	sf::Vector2f originalPos;
	sf::Vector2f option_points[4];
	Pawn* _enemy = nullptr;
	Pawn* _actor = nullptr;
	Pawn battleCursor;
	int num_of_selections;
	int selection = Battle::Fight;
	int aniCounter = 0;
	int aniFrameDuration = 300;
	int seperation = 100;
	sf::RectangleShape background;
	std::map<int, sf::Text> battle_options;
};
#endif
#pragma once
