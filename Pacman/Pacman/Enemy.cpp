#include "Enemy.h"

void Enemy::CheckGhostCollisions()
{
	//TODO Make this method into a collisions class

}

void Enemy::Update(int elapsedTime, int frameCount)
{
	if (direction == 0) GetPosition()->X += speed * elapsedTime;
	else if (direction == 1) GetPosition()->X -= speed * elapsedTime;
	else if (direction == 2) GetPosition()->Y += speed * elapsedTime;
	else if (direction == 3) GetPosition()->Y -= speed * elapsedTime;
	
	if (GetPosition()->X + GetSourceRect()->Width >= SCREEN_WIDTH - 32) direction = 1;
	else if (GetPosition()->X <= 32) direction = 0;

	if (GetPosition()->Y + GetSourceRect()->Width >= SCREEN_HEIGHT - 32) direction = 3;
	else if (GetPosition()->Y <= 32) direction = 2;
}

Enemy::Enemy(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect) {
	direction = rand() % 4;
	speed = 0.25f;
	if (GetPosition()->Y >= SCREEN_HEIGHT - 32) {
		SetPosition(new S2D::Vector2(GetPosition()->X, GetPosition()->Y - 32));
	}
	
}
