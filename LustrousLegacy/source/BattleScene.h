#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>
#include <string>

class BattleScene : sf::Drawable
{
public:
	BattleScene(std::string textureName, int numTextures, std::string location);
	~BattleScene();

private:
	bool inBattle = false;
	std::vector<sf::Texture*> battleSprite;
};

BattleScene::BattleScene(std::string textureName, int numTextures, std::string location)
{
}

BattleScene::~BattleScene()
{
}