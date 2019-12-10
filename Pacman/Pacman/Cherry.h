#pragma once
#ifndef _CHERRY_H
#define _CHERRY_H

#include "GameObject.h"

class Cherry : public GameObject { //inherits from GameObject class
public:
	Cherry(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect) {}  //passes through the texture, position and srcRect to the base class's constructor

	bool SetTimer = true;


	void Update(int elapsedTime, int frameCount) override { //overrides the base classes update method 
		if (frameCount < PREFFERRED_FPS / 2) { //two frames of animation therefore only needs to be a division of two
			mSrcRect->X = 0;
		}
		else {
			mSrcRect->X = 32;
		}
		if (SetTimer) {
			timeToSpawn = spawnTimer;
			SetTimer = false;
		}

		if (!SetTimer) //sets the timer to go off after 15 seconds
		{
			timeToSpawn -= 0.017f;
			if (timeToSpawn <= 0) {
				timeToSpawn = spawnTimer;
				mPosition = co.GeneratePositionWithinGameBounds(); //generates a new position for the cherry
			}
		}
	}

private:
	const float spawnTimer = 15.0f;
	float timeToSpawn;
};

#endif
