#pragma once
#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include "S2D/S2D.h"
#include "GameObject.h"

class Collisions {
public:
	Collisions() {

	}
	~Collisions() {

	}
	bool CheckCollisions(GameObject* object1, GameObject* object2) {
		int bottom1 = object1->GetPosition()->Y + object1->GetSourceRect()->Height;
		int bottom2 = object2->GetPosition()->Y + object2->GetSourceRect()->Height;

		int left1 = object1->GetPosition()->X;
		int left2 = object2->GetPosition()->X;

		int right1 = object1->GetPosition()->X + object1->GetSourceRect()->Width;
		int right2 = object2->GetPosition()->X + object2->GetSourceRect()->Width;

		int top1 = object1->GetPosition()->Y;
		int top2 = object2->GetPosition()->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2)) {
			return true;
		}
		else {
			return false;
		}
	}

	
};


#endif

