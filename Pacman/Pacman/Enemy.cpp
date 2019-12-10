#include "Enemy.h"


void Enemy::GenerateValues()
{
	direction = rand() % 4; //generates a direction for the ghost
	int num = rand() % 3; //generates a number to be used for the ai type
	ai = (AIType)num; //casts the number to AIType to load the correct ai method
	switch (ai) //sets the src rect and the speed variable to the value for the corresponding AI type
	{
	case SideToSide:
		speed = 0.25f; 
		mSrcRect->Y = 0;
		break;
	case Chase:
		speed = 0.05f;
		mSrcRect->Y = 20;
		break;
	case RandomDirection:
		speed = 0.25f;
		mSrcRect->Y = 40;
		break;
	}
}

void Enemy::ReverseDirection() //switches the ghosts direction when they hit the edge of the playspace
{
	if (GetPosition()->X + GetSourceRect()->Width >= SCREEN_WIDTH - SCREENX_OFFSET) direction = 1; 
	else if (GetPosition()->X <= SCREENX_OFFSET) direction = 0;

	if (GetPosition()->Y + GetSourceRect()->Width >= SCREEN_HEIGHT - SCREENY_OFFSET) direction = 3;
	else if (GetPosition()->Y <= SCREENY_OFFSET) direction = 2;
}

void Enemy::Update(int elapsedTime, int frameCount)
{
	//gets the distance to the player
	float xDistanceToPlayer = mPlayer->GetPosition()->X - GetPosition()->X;
	float yDistanceToPlayer = mPlayer->GetPosition()->Y - GetPosition()->Y;

	if (!frozen) { //if not frozen
		switch (ai)
		{

		case Enemy::SideToSide: //0 = right, 1 = left, 2 = down, 3 = up
			if (direction == 0) {
				GetPosition()->X += speed * elapsedTime; //move right
				mSrcRect->X = 0; //sets the sprite to look in the direction its moving
			}
			else if (direction == 1) {
				GetPosition()->X -= speed * elapsedTime; //move left
				mSrcRect->X = 20;
			}
			else if (direction == 2) {
				GetPosition()->Y += speed * elapsedTime; //move down
				mSrcRect->X = 40;
			}
			else if (direction == 3) {
				GetPosition()->Y -= speed * elapsedTime; //move up
				mSrcRect->X = 60;
			}
			break;
		case Enemy::Chase:
			if (GetPosition()->X < mPlayer->GetPosition()->X) GetPosition()->X += speed * elapsedTime; //gets the players X position and moves closer to it
			else if (GetPosition()->X > mPlayer->GetPosition()->X)  GetPosition()->X -= speed * elapsedTime;

			if (GetPosition()->Y < mPlayer->GetPosition()->Y)  GetPosition()->Y += speed * elapsedTime; //gets the players Y position and moves closer to it
			else if (GetPosition()->Y > mPlayer->GetPosition()->Y)  GetPosition()->Y -= speed * elapsedTime;

			if (xDistanceToPlayer > yDistanceToPlayer) { //this block of code decides which direction sprite to use based on if the x distance to the player is greater than the y distance 
				if (GetPosition()->X < mPlayer->GetPosition()->X) mSrcRect->X = 0;
				else if (GetPosition()->X > mPlayer->GetPosition()->X) mSrcRect->X = 20;

			}
			else {
				if (GetPosition()->Y < mPlayer->GetPosition()->Y) mSrcRect->X = 40;
				else if (GetPosition()->Y > mPlayer->GetPosition()->Y) mSrcRect->X = 60;
			}

			break;
		case Enemy::RandomDirection:
			if (direction == 0) { //move right
				GetPosition()->X += speed * elapsedTime;
				mSrcRect->X = 0;
			}
			else if (direction == 1) { //move left
				GetPosition()->X -= speed * elapsedTime;
				mSrcRect->X = 20;
			}
			else if (direction == 2) { //move down
				GetPosition()->Y += speed * elapsedTime;
				mSrcRect->X = 40;
			}
			else if (direction == 3) { //move up
				GetPosition()->Y -= speed * elapsedTime;
				mSrcRect->X = 60;
			}

			//switches the ghosts direction
			switchDirectionTime -= 0.17;
			if (switchDirectionTime <= 0) {
				switchDirectionTime = switchDirectionTimer;
				direction = rand() % 4;
			}
			break;
		}

		ReverseDirection();
	}
}

Enemy::Enemy(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect, Player* pacman) : GameObject(texture, position, srcRect) { //passes parameters to the base class's constructor 
	mPlayer = pacman; //sets the mPlayer variable equal to the pacman pointer passed through
	GenerateValues();// generates the ghosts starting values

}
