#ifndef PLAYER_H_
#define PLAYER_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

// Forward Declaration
namespace sf {

	class Texture;
	class Clock;
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

		// Player default constructure, requires a playerTexture reference
		Player(const sf::Texture& playerTexture);
		// Player destructor (virtual -> destroy derived, then destroy base class)
		virtual ~Player();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void move(int direction, sf::Clock& clock, double speed = 0.2);
		void idle(sf::Clock& clock);

	private:

		sf::Sprite mSprite;
		sf::Vector2i mSource; // x is the animation step, y is the direction character is going
		int aniCounter = 0;
		int aniFrameDuration = 200;
	};
}

#endif