#include "player.h"
#include "debug/debug.h"
#include "Enums.h"

/*********************************************************************
Player class default constructor.
\brief Requires a texture for instantiation.
*********************************************************************/
Player::Player(const sf::Texture& imagePath) :
	mSprite(imagePath),
	mSource(Source::Idle, Direction::South) {
	mSprite.setOrigin(32, 32);
	mSprite.setTextureRect(sf::IntRect(mSource.x * 64, mSource.y * 64, 64, 64));
}

/*********************************************************************
Player class virtual destructor.
*********************************************************************/
Player::~Player() {
}

/*********************************************************************
draw; inherited from class drawable.
\brief Allows drawable class members to be drawn to the screen.
*********************************************************************/
void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

/*********************************************************************
\brief Moves the player based on provided speed and direction.
\param Speed, Elapsed Time, Collision Switch, System Control Switch, Direction
*********************************************************************/
void Player::move(int speed, float elapsedTime, bool& collision, bool& move_flag, int direction) {
	if (!is_moving || move_flag) {
		if (move_flag)
		{
			is_moving = true;
		}
		else {
			if (direction == -1)
				is_moving = sysMovement();
			else {
				is_moving = true;
				move(speed, elapsedTime, collision, move_flag, direction);
			}
		}
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

/*********************************************************************
\brief Forces the player texture at the idle stance.
*********************************************************************/
void Player::idle() {
	mSource.x = 1;
	mSprite.setTextureRect(sf::IntRect(mSource.x * 64, mSource.y * 64, 64, 64));
}

/*********************************************************************
\brief Sets the player at the provided position.
\param sf::Vector2f that defines the position.
*********************************************************************/
void Player::setPosition(sf::Vector2f pos) {
	mSprite.setPosition(pos);
}

/*********************************************************************
\brief Returns the current position of the player.
*********************************************************************/
sf::Vector2f Player::getPosition() {
	return mSprite.getPosition();
}

/*********************************************************************
\brief Returns the previous cycle's position of the player.
*********************************************************************/
sf::Vector2f Player::getPastPosition() {
	return pastPosition;
}

/*********************************************************************
\brief Returns the the current direction the player is facing.
*********************************************************************/
int Player::getDirection() {
	return playerDirection;
}

/*********************************************************************
\brief Sets the direction that the player is facing.
\param Direction
*********************************************************************/
void Player::setDirection(int dir) {
	playerDirection = dir;
	mSource.y = playerDirection;
	mSource.x = 1;
	mSprite.setTextureRect(sf::IntRect(mSource.x * 64, mSource.y * 64, 64, 64));
}

/*********************************************************************
\brief Returns the direction the player was facing in the last cycle.
*********************************************************************/
int Player::getPastDirection() {
	return pastDirection;
}

/*********************************************************************
\brief Checks the player's input for movement. 
sysMovement checks the input from the keyboard and then returns
true if the player has pressed keys that should perform movement.
sysMovement returns false otherwise.
*********************************************************************/
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

/*********************************************************************
\brief Returns the current status of player movement.
isMoving returns true if the player is moving.
isMoving returns false if the player is not moving.
*********************************************************************/
bool Player::isMoving() {
	return is_moving;
}
