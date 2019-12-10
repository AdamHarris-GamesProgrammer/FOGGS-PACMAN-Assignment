#pragma once
#ifndef _ENEMY_H
#define _ENEMY_H

#include "GameObject.h"
#include "S2D/S2D.h"

//needs to know where the player is for the chase AI system 
#include "Player.h"

class Enemy : public GameObject {
private:
	void ReverseDirection(); //reverses the direction when a ghost gets to the edge of the screen

public:
	enum AIType { //this holds the 3 types of AI that a ghost can be 
		SideToSide = 0,
		Chase,
		RandomDirection
	};

	Player* mPlayer; //needs to store a pointer to the players memory address so the ghost can track the player
	int direction; //the direction of the ghost, 0 = right, 1 = left, 2 = down, 3 = up
	float speed; //movement speed, different for each AI to make the game more balanced

	const float switchDirectionTimer = 7.0f; //switches the direction of the random direction AI type every 7 seconds
	float switchDirectionTime; //holds the current timer value
	bool frozen = false; //is used to decide if the ghost is frozen or not
	AIType ai; //holds the current ai type of the ghost
	void Update(int elapsedTime, int frameCount) override; //overrides the base class's update method


	void GenerateValues(); //this method generates the values for the ghost such as AI type, direction and position
	Enemy(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect, Player* pacman); 
};

#endif // !_ENEMY_H