#ifndef PLAYER_H_
#define PLAYER_H_

#include "Enums.h"
#include <SFML/Graphics.hpp>

namespace sf {

	class Texture;
}
	
class Player : public sf::Drawable {
	public:

		Player(const sf::Texture& playerTexture);
		virtual ~Player();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void move(int speed, float elapsedTime, bool& collision, bool& move_switch, int direction = -1);
        void faceDirection(int direction);
		void idle();
		void setDirection(int dir);
		int getDirection();
		int getPastDirection();
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition();
		sf::Vector2f getPastPosition();
		bool sysMovement();
		bool isMoving();
	
	private:

		sf::Sprite mSprite;
		sf::Vector2f mSource;
		sf::Vector2f pastPosition;
		int playerDirection;
		int pastDirection;
		int aniCounter = 0;
		int aniFrameDuration = 200;
		int playerSpeed = Speed::Normal;
		int distance_moved = 0;
		bool is_moving = false;
	};
#endif