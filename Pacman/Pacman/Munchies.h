#pragma once
#ifndef _MUNCHIES_H
#define _MUNCHIES_H

#include "GameObject.h"
#include "Constants.h"

class Munchies : public GameObject{
public:
	Munchies(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect){}

	void Update(int elapsedTime, int frameCount) override {
		if (frameCount < PREFFERRED_FPS / 2)
		{
			mSrcRect->X = 0;
		}
		else
		{
			mSrcRect->X = 8;
		}
	}
};
#endif