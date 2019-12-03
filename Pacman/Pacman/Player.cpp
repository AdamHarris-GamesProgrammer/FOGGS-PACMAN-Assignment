#include "Player.h"

void Player::Movement(int elapsedTime)
{
	float movementSpeed = speedMultiplier * elapsedTime;
	int directionValue = direction * 32;
	if (movingRight) {
		mPosition->X += movementSpeed;
		mSrcRect->Y = directionValue;
	}
	else if (movingLeft) {
		mPosition->X -= movementSpeed;
		mSrcRect->Y = directionValue;
	}
	else if (movingUp) {
		mPosition->Y -= movementSpeed;
		mSrcRect->Y = directionValue;
	}
	else if (movingDown) {
		mPosition->Y += movementSpeed;
		mSrcRect->Y = directionValue;
	}
}

void Player::ScreenWrap()
{
	//screen edge collision system
	if (mPosition->X + mSrcRect->Width > SCREEN_WIDTH - 32) {
		mPosition->X = 32 + 0.01f;
	}
	else if (mPosition->X <= 32) {
		mPosition->X = SCREEN_WIDTH - mSrcRect->Width - 32;
	}

	if (mPosition->Y + mSrcRect->Height > SCREEN_HEIGHT - 32) {
		mPosition->Y = 64 + 0.01f;
	}
	else if (mPosition->Y <= 64) {
		mPosition->Y = SCREEN_HEIGHT - mSrcRect->Height - 32;
	}
}

void Player::PollInput()
{

	// Gets the current state of the keyboard
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	//traditional pacman movement system
	if (keyboardState->IsKeyDown(S2D::Input::Keys::W)) { //MOVE UP
		movingUp = true;
		movingLeft = false;
		movingRight = false;
		movingDown = false;
		direction = Direction::UP;
	}
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::A)) { //MOVE LEFT
		movingUp = false;
		movingLeft = true;
		movingRight = false;
		movingDown = false;
		direction = Direction::LEFT;
	}
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::S)) { //MOVE DOWN
		movingUp = false;
		movingLeft = false;
		movingRight = false;
		movingDown = true;
		direction = Direction::DOWN;
	}
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::D)) { //MOVE RIGHT
		movingUp = false;
		movingLeft = false;
		movingRight = true;
		movingDown = false;
		direction = Direction::RIGHT;
	}
	else {
		movingUp = movingUp;
		movingLeft = movingLeft;
		movingRight = movingRight;
		movingDown = movingDown;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::LEFTSHIFT)) {
		speedMultiplier = 1.0f;
	}
	else if (keyboardState->IsKeyUp(S2D::Input::Keys::LEFTSHIFT)) {
		speedMultiplier = 0.16f;
	}

}

void Player::Update(int elapsedTime, int frameCount)
{
	if (frameCount < PREFFERRED_FPS / 2) {
		mSrcRect->X = 32;
	}
	else {
		mSrcRect->X = 0;
	}
	if (invincible) {
		invincibilityTimer -= 0.017;
		if (invincibilityTimer <= 0.0f)
			invincible = false;
	}
	PollInput();
	Movement(elapsedTime);
	ScreenWrap();
}

void Player::Render()
{
	if(!dead)
		S2D::SpriteBatch::Draw(mTexture, mPosition, mSrcRect);
}


void Player::PlaySound(S2D::SoundEffect* sound)
{
	S2D::Audio::Play(sound);
}

Player::Player(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture,position,srcRect)
{
	direction = Direction::RIGHT;
	speedMultiplier = 0.16f;

	pacmanChompSound = rl.LoadSound("Assets/Sounds/pacman_chomp.wav"); //used
	pacmanEatFruitSound = rl.LoadSound("Assets/Sounds/pacman_eatfruit.wav");
	pacmanEatGhostSound = rl.LoadSound("Assets/Sounds/pacman_eatghost.wav");
	pacmanExtraPacSound = rl.LoadSound("Assets/Sounds/pacman_extrapac.wav");
	pacmanMunchSound = rl.LoadSound("Assets/Sounds/pacman_munch.wav");

	dead = false;
	invincible = true;
}


