#ifndef PLAYER_H_
#define PLAYER_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

// Forward Declaration
namespace sf {

	class Texture;
}

namespace actor {
	
	class Player : public sf::Drawable {
	public:

		enum Direction {
			South, West, East, North
		};

		enum States {
			Idle, Walking
		};

		enum Speed {
			Slow = 1,
			Normal = 4,
			Fast = 6,
			Fastest = 8
		};

		// Player default constructure, requires a playerTexture reference
		Player(const sf::Texture& playerTexture);
		// Player destructor (virtual -> destroy derived, then destroy base class)
		virtual ~Player();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void move(int speed, float elapsedTime, bool& collision, bool& move_switch, int direction = -1);
		void idle();
		bool sysMovement();

		// player positioning functions
		void setPosition(int x, int y);
		sf::Vector2f getPosition();
		sf::Vector2f getPastPosition();
		sf::FloatRect getGlobalBounds();
		int getPastDirection();
		int getDirection();

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
}

#endif