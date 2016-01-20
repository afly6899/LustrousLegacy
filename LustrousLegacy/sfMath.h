#include <SFML/Graphics.hpp>
#include "Enums.h"
#include <iostream>

namespace sfmath {

	/***********************************************
	eturns a vector normal
	*********************************************************************/
	template<typename T>
	T Normalize(T vector) {
		int mag_x = 1;
		int mag_y = 1;

		if (vector.x != 0)
			mag_x = sqrt(vector.x*vector.x);
		if (vector.y != 0)
			mag_y = sqrt(vector.y*vector.y);

		return T(vector.x/mag_x, vector.y/mag_y);
	}

	template<typename T>
	unsigned int vecDirection(T vector) {
		T temp = Normalize(vector);
		
		if (temp == T(1, 0)) {
			return Direction::East;
		}
		else if (temp == T(-1, 0)) {
			return Direction::West;
		}
		else if (temp == T(0, 1) || temp == T(1, 1) || temp == T(-1, 1)) {
			return Direction::South;
		}
		else if (temp == T(0, -1) || temp == T(1, -1) || temp == T(-1, -1)) {
			return Direction::North;
		}

		return Direction::Null;
	}
}