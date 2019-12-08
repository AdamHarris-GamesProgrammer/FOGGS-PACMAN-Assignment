#pragma once
#ifndef _ENEMY_H
#define _ENEMY_H

#include "GameObject.h"
#include "S2D/S2D.h"
#include "ResourceLoaders.h"
#include "Constants.h"
#include "Player.h"



class Enemy : public GameObject {
private:
	void ReverseDirection();
	void ScreenWrap();

public:
	enum AIType {
		SideToSide = 0,
		Chase,
		RandomDirection,
		RunAway,
		Dead
	};
	S2D::Vector2* mPosition;
	S2D::Texture2D* mTexture;
	S2D::Rect* mSrcRect;
	Player* mPlayer;
	int direction;
	float speed;

	float switchDirectionTimer = 7.0f;
	float switchDirectionTime;
	bool frozen = false;
	AIType ai;
	void Update(int elapsedTime, int frameCount) override;


	void GenerateValues();
	Enemy(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect, Player* pacman);
};

#endif // !_ENEMY_H