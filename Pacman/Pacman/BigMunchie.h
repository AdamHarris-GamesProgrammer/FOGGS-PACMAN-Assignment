#pragma once
#include "GameObject.h"

class BigMunchie : public GameObject //inherits from game object
{
public:
	void Update(int elapsedTime, int frameCount) override { //overrides the base class's update method
		if (frameCount < PREFFERRED_FPS / 2) //since the big munchies only has two frames a simple division of 2 is needed for both frames
		{
			mSrcRect->X = 0; //first frame
		}
		else
		{ 
			mSrcRect->X = 10; //second frame
		}


	}

	BigMunchie(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect) {} //passes through the texture, position and srcRect to the base class's constructor
};

