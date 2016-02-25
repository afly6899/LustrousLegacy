#ifndef HEALTHBAR_H_
#define HEALTHBAR_H_

#include "Enums.h"
#include "Pawn.h"
#include <SFML/Graphics.hpp>


class HealthBar : public Pawn {
public:

	HealthBar(const sf::Texture& playerTexture);
	virtual ~HealthBar();
	
private:


};
#endif