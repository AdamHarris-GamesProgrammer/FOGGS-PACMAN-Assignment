#pragma once
#ifndef _MUNCHIES_H
#define _MUNCHIES_H

#include "GameObject.h"
#include "Constants.h"

class Munchies : public GameObject{
public:
	Munchies(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect){} //passes through the arguments into the base class constructor

	void Update(int elapsedTime, int frameCount) override { //overrides the base class's update method
		if (frameCount < PREFFERRED_FPS / 2) //first frame
		{
			mSrcRect->X = 0;
		}
		else //second frame
		{
			mSrcRect->X = 8;
		}
	}
};
#endif