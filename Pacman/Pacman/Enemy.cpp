#include "Enemy.h"


void Enemy::GenerateValues()
{
	direction = rand() % 4;
	ai = AIType::RandomDirection;
	int num = rand() % 3;
	ai = (AIType)num;
	switch (ai)
	{
	case Enemy::SideToSide:
		speed = 0.25f;
		mSrcRect->Y = 0;
		break;
	case Enemy::Chase:
		speed = 0.05f;
		mSrcRect->Y = 20;
		break;
	case Enemy::RandomDirection:
		speed = 0.25f;
		mSrcRect->Y = 40;
		break;
	default:
		break;
	}
}

void Enemy::CheckGhostCollisions()
{
	//TODO Make this method into a collisions class

}

void Enemy::Update(int elapsedTime, int frameCount)
{
	float xDistanceToPlayer = mPlayer->GetPosition()->X - GetPosition()->X;
	float yDistanceToPlayer = mPlayer->GetPosition()->Y - GetPosition()->Y;

	switch (ai)
	{
	case Enemy::SideToSide: //0 = right, 1 = left, 2 = down, 3 = up
		if (direction == 0) {
			GetPosition()->X += speed * elapsedTime;
			mSrcRect->X = 0;
		}
		else if (direction == 1) {
			GetPosition()->X -= speed * elapsedTime;
			mSrcRect->X = 20;
		}
		else if (direction == 2) {
			GetPosition()->Y += speed * elapsedTime;
			mSrcRect->X = 40;
		}
		else if (direction == 3) {
			GetPosition()->Y -= speed * elapsedTime;
			mSrcRect->X = 60;
		}

		if (GetPosition()->X + GetSourceRect()->Width >= SCREEN_WIDTH - 32) direction = 1;
		else if (GetPosition()->X <= 32) direction = 0;

		if (GetPosition()->Y + GetSourceRect()->Width >= SCREEN_HEIGHT - 32) direction = 3;
		else if (GetPosition()->Y <= 32) direction = 2;
		break;
	case Enemy::Chase:
		if (GetPosition()->X < mPlayer->GetPosition()->X) { GetPosition()->X += speed * elapsedTime; }
		else if (GetPosition()->X > mPlayer->GetPosition()->X) { GetPosition()->X -= speed * elapsedTime; }

		if (GetPosition()->Y < mPlayer->GetPosition()->Y) { GetPosition()->Y += speed * elapsedTime; }
		else if (GetPosition()->Y > mPlayer->GetPosition()->Y) {
			GetPosition()->Y -= speed * elapsedTime;
		}


		if (xDistanceToPlayer > yDistanceToPlayer) {
			if (GetPosition()->X < mPlayer->GetPosition()->X) {
				mSrcRect->X = 0;
			}
			else if (GetPosition()->X > mPlayer->GetPosition()->X) {
				mSrcRect->X = 20;
			}
		}
		else {
			if (GetPosition()->Y < mPlayer->GetPosition()->Y) {
				mSrcRect->X = 40;
			}
			else if (GetPosition()->Y > mPlayer->GetPosition()->Y) {
				mSrcRect->X = 60;
			}
		}
		break;
	case Enemy::RandomDirection:
		if (direction == 0) {
			GetPosition()->X += speed * elapsedTime;
			mSrcRect->X = 0;
		}
		else if (direction == 1) {
			GetPosition()->X -= speed * elapsedTime;
			mSrcRect->X = 20;
		}
		else if (direction == 2) {
			GetPosition()->Y += speed * elapsedTime;
			mSrcRect->X = 40;
		}
		else if (direction == 3) {
			GetPosition()->Y -= speed * elapsedTime;
			mSrcRect->X = 60;
		}

		if (GetPosition()->X + GetSourceRect()->Width >= SCREEN_WIDTH - 32) direction = 1;
		else if (GetPosition()->X <= 32) direction = 0;

		if (GetPosition()->Y + GetSourceRect()->Width >= SCREEN_HEIGHT - 32) direction = 3;
		else if (GetPosition()->Y <= 32) direction = 2;

		switchDirectionTime -= 0.17;
		if (switchDirectionTime <= 0) {
			switchDirectionTime = switchDirectionTimer;
			direction = rand() % 4;
		}
		break;
	default:
		break;
	}

}

Enemy::Enemy(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect, Player* pacman) : GameObject(texture, position, srcRect) {
	mPlayer = pacman;
	mSrcRect = srcRect;
	GenerateValues();

}
