#include "Pawn.h"

/*********************************************************************
Pawn class default constructor.
\brief Requires a texture for instantiation. The sprite_sheet
*********************************************************************/
//Pawn::Pawn(const sf::Texture& imagePath):
//	pawnSprite(imagePath),
//	spriteSource(Source::Idle, Direction:: South) {
//	spriteSize = pawnSprite.getTexture()->getSize().x / 3;
//	pawnSprite.setOrigin(spriteSize*.5, spriteSize);
//	pawnSprite.setTextureRect(sf::IntRect(spriteSource.x * spriteSize, spriteSource.y * spriteSize, spriteSize, spriteSize));
//}

Pawn::Pawn(const sf::Texture& imagePath, int numColumns) :
	pawnSprite(imagePath),
	spriteSource(Source::Idle, Direction::South) {
	spriteSize = pawnSprite.getTexture()->getSize().x / numColumns;
	pawnSprite.setOrigin(spriteSize*.5, spriteSize);
	pawnSprite.setTextureRect(sf::IntRect(spriteSource.x * spriteSize, spriteSource.y * spriteSize, spriteSize, spriteSize));
}

/*********************************************************************
Pawn class virtual destructor.
*********************************************************************/
Pawn::~Pawn() {
}

/*********************************************************************
draw; inherited from class drawable.
\brief Allows drawable class members to be drawn to the screen.
*********************************************************************/
void Pawn::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(pawnSprite, states);
}

/*********************************************************************
\brief Sets the Pawn at the provided position.
\param sf::Vector2f that defines the position.
*********************************************************************/
void Pawn::setPosition(sf::Vector2f pos) {
	pawnSprite.setPosition(pos);
}

/*********************************************************************
\brief Returns the current position of the Pawn.
*********************************************************************/
sf::Vector2f Pawn::getPosition() {
	return pawnSprite.getPosition();
}

/*********************************************************************
\brief Returns the the current direction the Pawn is facing.
*********************************************************************/
unsigned int Pawn::getDirection() {
	return spriteDirection;
}

/*********************************************************************
\brief Sets the direction that the Pawn is facing.
\param Direction
*********************************************************************/
void Pawn::setDirection(unsigned int dir) {
	spriteDirection = dir;
	if (spriteDirection == Direction::NorthEast || spriteDirection == Direction::NorthWest)
		spriteSource.y = Direction::North;
	else if (spriteDirection == Direction::SouthEast || spriteDirection == Direction::SouthWest)
		spriteSource.y = Direction::South;
	else
		spriteSource.y = spriteDirection;
	pawnSprite.setTextureRect(sf::IntRect(spriteSource.x * spriteSize, spriteSource.y * spriteSize, spriteSize, spriteSize));
}

/*********************************************************************
\brief Sets the frame duration for each animation.
\param Frame duration (larger = slower)
*********************************************************************/
void Pawn::setFrameDuration(unsigned int frame_duration) {
	aniFrameDuration = frame_duration;
}


/*********************************************************************
\brief Performs the animation of the Pawn's sprite. 
\param elapsedTime
\other Dependant on the value of aniFrameDuration for speed.
*********************************************************************/
void Pawn::spriteAnimate(float elapsedTime) {
	aniCounter += elapsedTime;

	if (aniCounter >= aniFrameDuration)
	{
		aniCounter -= aniFrameDuration;
		spriteSource.x++;

		if (spriteSource.x * spriteSize >= pawnSprite.getTexture()->getSize().x) {
			spriteSource.x = 0;
		}
	}

	pawnSprite.setTextureRect(sf::IntRect(spriteSource.x * spriteSize, spriteSource.y * spriteSize, spriteSize, spriteSize));
}

/*********************************************************************
\brief temp
*********************************************************************/
void Pawn::changeSprite(const sf::Texture& playerTexture) {
	pawnSprite.setTexture(playerTexture);
}

/*********************************************************************
\brief temp
*********************************************************************/
sf::Sprite& Pawn::getSprite() {
	return pawnSprite;
}

/*********************************************************************
\brief temp
*********************************************************************/
void Pawn::resetTextureRect() {
	spriteSource.x = 1;
	pawnSprite.setTextureRect(sf::IntRect(spriteSource.x * spriteSize, spriteSource.y * spriteSize, spriteSize, spriteSize));
}

/*********************************************************************
\brief temp
*********************************************************************/
int Pawn::getDepth() {
	return -getPosition().y;
}

/*********************************************************************
\brief temp
*********************************************************************/
std::string Pawn::getClass() {
	return "Pawn";
}
