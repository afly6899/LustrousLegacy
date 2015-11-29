#include "NPC.h"

NPC::NPC(const sf::Texture& imagePath_object) :
	objSprite(imagePath_object),
	objSource(0, 0) {
	objSprite.setOrigin(32, 32);
	objSprite.setTextureRect(sf::IntRect(objSource.x * 64, objSource.y * 64, 64, 64));
}

NPC::~NPC() {

}

sf::Vector2f NPC::getPosition() {
	return objSprite.getPosition();
}

void NPC::move(int num_tiles, int direction, float elapsedTime) {
	if (distance_moved == 64*num_tiles) {
		if (movement_complete) {
			movement_complete = false;
		}
		else
			movement_complete = true;
		is_moving = false;
		distance_moved = 0;
		objSource.x = 1;
		objSprite.setTextureRect(sf::IntRect(objSource.x * 64, objSource.y * 64, 64, 64));
	}
	else if (is_moving){
		npcDirection = direction;
		objSource.y = npcDirection;
		switch (npcDirection) {
		case Direction::South: objSprite.move(0, 1);
			break;
		case Direction::East: objSprite.move(1, 0);
			break;
		case Direction::West: objSprite.move(-1, 0);
			break;
		case Direction::North: objSprite.move(0, -1);
			break;
		}

		distance_moved += 1;

		aniCounter += elapsedTime;

		if (aniCounter >= aniFrameDuration)
		{
			aniCounter -= aniFrameDuration;
			objSource.x++;

			if (objSource.x * 64 >= (int)objSprite.getTexture()->getSize().x) {
				objSource.x = 0;
			}
		}

		objSprite.setTextureRect(sf::IntRect(objSource.x * 64, objSource.y * 64, 64, 64));
	}
}

void NPC::reset_move() {
	if (is_moving != true)
	{
		is_moving = true;
	}
}

bool NPC::moveComplete() {
	return movement_complete;
}
void NPC::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(objSprite, states);
}

void NPC::setPosition(sf::Vector2f pos) {
	objSprite.setPosition(pos);
}

void NPC::hover(float elapsedTime) {

	aniCounter += elapsedTime;

	if (aniCounter >= aniFrameDuration) {
		aniCounter -= aniFrameDuration;
		objSource.y++;
		if (objSource.y > 3)
			objSource.y = 0;
		objSprite.setTextureRect(sf::IntRect(objSource.x * 64, objSource.y * 64, 64, 64));
	}
}