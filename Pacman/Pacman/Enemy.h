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



	bool canBeKilled = false;
	bool dead = false;
	float respwanTimer;
	const float respawnDuration = 15.0f;
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

	AIType ai;
	void CheckGhostCollisions();
	void Update(int elapsedTime, int frameCount) override;

	bool GetCanbeKilled() { return canBeKilled; }
	void SetCanBeKilled(bool newValue) { canBeKilled = newValue; }

	void GenerateValues();
	Enemy(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect, Player* pacman);
};

#endif // !_ENEMY_H

//TODO: chase timer going down
//program run away ai
//program death and respawn methods