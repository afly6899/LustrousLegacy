#include "Cursor.h"

Cursor::Cursor(const sf::Texture & playerTexture)
: Pawn(playerTexture, 1) {
	spriteSource.x = Direction::South;
	spriteSource.y = Source::Idle;
	pawnSprite.setTextureRect(sf::IntRect(spriteSource.x * spriteSize, spriteSource.y * spriteSize, spriteSize, spriteSize));
}

void Cursor::spriteAnimate(float elapsedTime)
{
	aniCounter += elapsedTime;

	if (aniCounter >= aniFrameDuration)
	{
		aniCounter -= aniFrameDuration;
		spriteSource.y++;

		if (spriteSource.y * spriteSize >= pawnSprite.getTexture()->getSize().y) {
			spriteSource.y = 0;
		}
	}

	pawnSprite.setTextureRect(sf::IntRect(spriteSource.x * spriteSize, spriteSource.y * spriteSize, spriteSize, spriteSize));
}
