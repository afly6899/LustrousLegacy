#ifndef NPC_H_
#define NPC_H_

#include <SFML/Graphics.hpp>
#include "Enums.h"

namespace sf {

	class Texture;
}

class NPC : public sf::Drawable {
public:

	NPC(const sf::Texture& imagePath_object);
	virtual ~NPC();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Vector2f getPosition();
	void move(int num_tiles, int direction, float elapsedTime);
	void reset_move();
	void setPosition(sf::Vector2f pos);
	void hover(float elapsedTime);
	bool NPC::moveComplete();

private:

	sf::Sprite objSprite;
	sf::Vector2f objSource;
	int aniCounter = 0;
	int aniFrameDuration = 300;
	bool is_moving = true;
	bool movement_complete = false;
	bool restart = false;
	int distance_moved = 0;
	int npcDirection;
};
#endif