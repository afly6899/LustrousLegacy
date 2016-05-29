#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Pawn.h"
#include "UI.h"
#include <map>
#include <random>

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
	void battle(bool ENTER_KEY, float elapsedTime); 
	std::string BattleSystem::getClass();

private:

	std::default_random_engine gen;
	std::uniform_real_distribution<float> play_dis{ .6,1 };
	std::uniform_real_distribution<float> enemy_dis{ .3,.9 };

	sf::Sound& cursorBleep;
	sf::Vector2u window_size;
	sf::Vector2f originalPos;
	sf::Vector2f option_points[4];
	Pawn enemy;
	Pawn battleCursor;
	int num_of_selections;
	int selection = Battle::Fight;
	int aniCounter = 0;
	int aniFrameDuration = 300;
	int seperation = 100;
	bool player_turn = true;
	int player_health = 100;
	int player_attack = 20;
	int enemy_attack = 10;
	int enemy_health = 150;
	sf::RectangleShape background;
	std::map<int, sf::Text> battle_options;
};
#endif
#pragma once
