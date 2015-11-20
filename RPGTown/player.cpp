#include "player.h"
#include "debug.h"
#include "enums.h"

// Player default constructor; Loads: character sprite from texture and sets position to frame 1 and south
Player::Player(const sf::Texture& imagePath) :
	mSprite(imagePath),
	mSource(1, Direction::South) {
	mSprite.setOrigin(32, 32);
	mSprite.setScale(1.0f, 1.0f);
	mSprite.setTextureRect(sf::IntRect(mSource.x * 64, mSource.y * 64, 64, 64));
}

// Player virtual destructor;
Player::~Player() {
}

// Derived from the sf::drawable class; Allows to be Player object to be drawn to screen
void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

// Player move function moves sprite and animates based on clock and speed
void Player::move(int speed, float elapsedTime, bool& collision, bool& move_flag, int direction) {
	if (!is_moving || move_flag) {
		if (move_flag)
		{
			is_moving = true;
		}
		else
			is_moving = sysMovement();
		move_flag = false;
	}
	else if (collision) {
		aniCounter += elapsedTime;

		if (aniCounter >= aniFrameDuration) {
			aniCounter -= aniFrameDuration;
			mSource.x++;

			if (mSource.x * 64 >= (int)mSprite.getTexture()->getSize().x) {
				mSource.x = 0;
			}
		}

		is_moving = false;
		collision = false;
		distance_moved = 0;
	}
	else if (distance_moved == 64) {
		is_moving = false;
		distance_moved = 0;
	}
	else {
		if (direction > -1)
		{
			playerDirection = direction;
		}
		mSource.y = playerDirection;
		pastDirection = playerDirection;
		pastPosition = getPosition();
		switch (playerDirection) {
		case Direction::South: mSprite.move(0, playerSpeed);
		break;
		case Direction::East: mSprite.move(playerSpeed, 0);
			break;
		case Direction::West: mSprite.move(-playerSpeed, 0);
			break;
		case Direction::North: mSprite.move(0, -playerSpeed);
			break;
		}

		distance_moved += playerSpeed;

		aniCounter += elapsedTime;

		if (aniCounter >= aniFrameDuration)
		{
			aniCounter -= aniFrameDuration;
			mSource.x++;

			if (mSource.x * 64 >= (int)mSprite.getTexture()->getSize().x) {
				mSource.x = 0;
			}
		}

		mSprite.setTextureRect(sf::IntRect(mSource.x * 64, mSource.y * 64, 64, 64));
	}
}

// Player idle sprite is loaded
void Player::idle() {
	mSource.x = 1;
	mSprite.setTextureRect(sf::IntRect(mSource.x * 64, mSource.y * 64, 64, 64));
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

void Player::setDirection(int dir) {
	playerDirection = dir;
	mSource.y = playerDirection;
	mSource.x = 1;
	mSprite.setTextureRect(sf::IntRect(mSource.x * 64, mSource.y * 64, 64, 64));
}

int Player::getPastDirection() {
	return pastDirection;
}

bool Player::sysMovement()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
		playerSpeed = Speed::Fast;
	}
	else
		playerSpeed = Speed::Normal;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		playerDirection = Direction::North;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		playerDirection = Direction::South;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		playerDirection = Direction::East;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		playerDirection = Direction::West;
	}
	else
	{
		idle();
		return false;
	}
	return true;
}

bool Player::isMoving() {
	return is_moving;
}
