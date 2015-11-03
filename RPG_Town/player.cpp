#include "player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace actor {
	// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
	Player::Player(const sf::Texture& imagePath) :
		mSprite(imagePath),
		mSource(1, Player::South) {
		mSprite.setOrigin(32, 32);
		mSprite.setScale(1.0f, 1.0f);
		
	}

	// Player virtual destructor;
	Player::~Player() {

	}

	// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
	void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(mSprite, states);
	}

	// Player move function moves sprite and animates based on clock and speed
	void Player::move(int direction, float elapsedTime, int speed, bool collision) {
        std::cout << "Will move " << speed << " pixels" << std::endl;
		mSource.y = direction;
		playerDirection = direction;

//		if (collision == false)
//		{ 
//			pastPosition = mSprite.getPosition();
//			switch (direction) {
//			case Player::South: mSprite.move(0, speed * 1/elapsedTime);
//				break;
//			case Player::East: mSprite.move(speed * 1/elapsedTime, 0);
//				break;
//			case Player::West: mSprite.move(-speed * 1/elapsedTime, 0);
//				break;
//			case Player::North: mSprite.move(0, -speed * 1/elapsedTime);
//				break;
//			}
//		}
        
        if (collision == false)
        {
            pastPosition = mSprite.getPosition();
            switch (direction) {
                case Player::South: mSprite.move(0, int(speed/4));
                    break;
                case Player::East: mSprite.move(speed/4, 0);
                    break;
                case Player::West: mSprite.move(-speed/4, 0);
                    break;
                case Player::North: mSprite.move(0, -speed/4);
                    break;
            }
        }
        if (int(aniCounter)%3 == 0) {
            mSource.x++;
        if (mSource.x * 64 >= (int)mSprite.getTexture()->getSize().x) {
            mSource.x = 0;
        }
        }
//		aniCounter += elapsedTime;
//
//		if (aniCounter >= aniFrameDuration)
//		{
//			aniCounter -= aniFrameDuration;
//			mSource.x++;
//
//			if (mSource.x * 64 >= (int)mSprite.getTexture()->getSize().x) {
//				mSource.x = 0;
//			}
//		}

		mSprite.setTextureRect(sf::IntRect(mSource.x * 64, mSource.y * 64, 64, 64));
	}

	// Player idle sprite is loaded
	void Player::idle() {
		mSource.x = 1;
		mSprite.setTextureRect(sf::IntRect(mSource.x *64, mSource.y * 64, 64, 64));
	}

	void Player::setPosition(int x, int y) {
		mSprite.setPosition(x, y);
	}

	sf::Vector2f Player::getPosition() {
		return mSprite.getPosition();
	}

	sf::Vector2f Player::getPastPosition() {
		return pastPosition;
	}

	sf::FloatRect Player::getGlobalBounds() {
		return mSprite.getGlobalBounds();
	}

	int Player::getDirection() {
		return playerDirection;
	}
}

