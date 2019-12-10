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
	bool CheckCollisions(GameObject* object1, GameObject* object2) { //takes in two game objects
		int bottom1 = object1->GetPosition()->Y + object1->GetSourceRect()->Height; //the bottom of the first game object
		int bottom2 = object2->GetPosition()->Y + object2->GetSourceRect()->Height; //the bottom of the second game object

		int left1 = object1->GetPosition()->X; //the left side of the first game object
		int left2 = object2->GetPosition()->X; //the left side of the second game object

		int right1 = object1->GetPosition()->X + object1->GetSourceRect()->Width; //the right side of the first game object
		int right2 = object2->GetPosition()->X + object2->GetSourceRect()->Width; //the right side of the second game object

		int top1 = object1->GetPosition()->Y; //the top of the first game object
		int top2 = object2->GetPosition()->Y; //the top of the second game object

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2)) { //if there is a overlapping side
			return true;
		}
		else {
			return false;
		}
	}

	
};


#endif

