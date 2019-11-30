#include "Enemy.h"

void Enemy::CheckGhostCollisions()
{
	//TODO Make this method into a collisions class

}

void Enemy::Update(int elapsedTime, int frameCount)
{
	switch (ai)
	{
	case Enemy::SideToSide:
		if (direction == 0) GetPosition()->X += speed * elapsedTime;
		else if (direction == 1) GetPosition()->X -= speed * elapsedTime;
		else if (direction == 2) GetPosition()->Y += speed * elapsedTime;
		else if (direction == 3) GetPosition()->Y -= speed * elapsedTime;

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
		break;
	case Enemy::RandomDirection:
		if (direction == 0) GetPosition()->X += speed * elapsedTime;
		else if (direction == 1) GetPosition()->X -= speed * elapsedTime;
		else if (direction == 2) GetPosition()->Y += speed * elapsedTime;
		else if (direction == 3) GetPosition()->Y -= speed * elapsedTime;

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
	direction = rand() % 4;
	ai = AIType::RandomDirection;
	mPlayer = pacman;

	int num = rand() % 3;
	ai = (AIType)num;
	switch (ai)
	{
	case Enemy::SideToSide:
		speed = 0.25f;
		break;
	case Enemy::Chase:
		speed = 0.05f;
		break;
	case Enemy::RandomDirection:
		speed = 0.25f;
		break;
	default:
		break;
	}

	if (GetPosition()->Y >= SCREEN_HEIGHT - 32) {
		SetPosition(new S2D::Vector2(GetPosition()->X, GetPosition()->Y - 32));
	}

}
