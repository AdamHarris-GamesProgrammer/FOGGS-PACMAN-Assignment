#pragma once
#include "GameObject.h"
class BigMunchie : public GameObject
{
public:
	bool isActivated = false;

	void SetActive(bool newValue) { isActivated = newValue; }
	bool GetActive() { return isActivated; }

	void Update(int elapsedTime, int frameCount) override {
		if (frameCount < PREFFERRED_FPS / 2)
		{
			mSrcRect->X = 0;
		}
		else
		{
			mSrcRect->X = 10;
		}


	}

	BigMunchie(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect) {}

private:


};

