#pragma once
#ifndef _CHERRY_H
#define _CHERRY_H

#include "GameObject.h"

class Cherry : public GameObject {
public:
	Cherry(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect) {}

	bool SetTimer = true;


	void Update(int elapsedTime, int frameCount) override {
		if (SetTimer) {
			timeToSpawn = spawnTimer;
			SetTimer = false;
		}

		if (!SetTimer)
		{
			timeToSpawn -= 0.017;
			if (timeToSpawn <= 0) {
				timeToSpawn = spawnTimer;
				mPosition = co.GeneratePositionWithinGameBounds();
			}
		}
	}

private:
	const float spawnTimer = 15.0f;
	float timeToSpawn;
};

#endif
