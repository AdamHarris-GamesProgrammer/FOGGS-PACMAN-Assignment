#pragma once

#include "S2D/S2D.h"
class CommonOperations {
public: 
	S2D::Vector2* GeneratePositionWithinGameBounds()
	{
		int randX = rand() % 992 + 48;
		int randY = rand() % 736 + 48;
		if (randX >= 944) {
			randX -= 128;
		}
		if (randY >= 688) {
			randY -= 96;
		}

		return new S2D::Vector2(randX, randY);
	}
};