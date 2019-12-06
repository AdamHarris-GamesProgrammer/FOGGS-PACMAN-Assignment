#include "Player.h"

void Player::Movement(int elapsedTime)
{
	float movementSpeed = speedMultiplier * elapsedTime;
	int directionValue = direction * 32;
	if (direction == Direction::RIGHT) {
		mPosition->X += movementSpeed;
		mSrcRect->Y = directionValue;
	}
	else if (direction == Direction::LEFT) {
		mPosition->X -= movementSpeed;
		mSrcRect->Y = directionValue;
	}
	else if (direction == Direction::UP) {
		mPosition->Y -= movementSpeed;
		mSrcRect->Y = directionValue;
	}
	else if (direction == Direction::DOWN) {
		mPosition->Y += movementSpeed;
		mSrcRect->Y = directionValue;
	}
}

void Player::ScreenWrap()
{
	//screen edge collision system
	if (mPosition->X + mSrcRect->Width > SCREEN_WIDTH - SCREENX_OFFSET) mPosition->X = SCREENX_OFFSET + 0.01f;
	else if (mPosition->X <= SCREENX_OFFSET) mPosition->X = SCREEN_WIDTH - mSrcRect->Width - SCREENX_OFFSET;

	if (mPosition->Y + mSrcRect->Height > SCREEN_HEIGHT - SCREENY_OFFSET) mPosition->Y = SCREENY_OFFSET + 0.01f;
	else if (mPosition->Y <= SCREENY_OFFSET) mPosition->Y = SCREEN_HEIGHT - mSrcRect->Height - SCREENY_OFFSET;
}

void Player::PollInput()
{
	// Gets the current state of the keyboard
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	//traditional pacman movement system
	if (keyboardState->IsKeyDown(S2D::Input::Keys::W)) direction = Direction::UP;
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::A)) direction = Direction::LEFT;
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::S)) direction = Direction::DOWN;
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::D)) direction = Direction::RIGHT;
}

void Player::Update(int elapsedTime, int frameCount)
{
	if (frameCount < PREFFERRED_FPS / 2) {
		mSrcRect->X = 32;
		if (!hasChompedSoundPlayed) {
			PlaySound(pacmanChompSound);
			hasChompedSoundPlayed = true;
		}
	}
	else {
		mSrcRect->X = 0;
	}
	if (frameCount >= PREFFERRED_FPS) {
		hasChompedSoundPlayed = false;
	}

	if (invincible) {
		invincibilityTimer -= 0.017;
		if (invincibilityTimer <= 0.0f)
			invincible = false;
	}
	if (powerUpActive) {
		effectTimer -= 0.017;
		if (effectTimer <= 0.0f) {
			std::cout << effectTimer << std::endl;
			effectTimer = effectDuration;
			powerUpActive = false;
		}
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
	direction = Direction::STILL;
	speedMultiplier = 0.16f;

	pacmanChompSound = rl.LoadSound("Assets/Sounds/pacman_chomp.wav"); //used
	pacmanEatFruitSound = rl.LoadSound("Assets/Sounds/pacman_eatfruit.wav"); //used
	pacmanEatGhostSound = rl.LoadSound("Assets/Sounds/pacman_eatghost.wav");
	pacmanExtraPacSound = rl.LoadSound("Assets/Sounds/pacman_extrapac.wav");
	pacmanMunchSound = rl.LoadSound("Assets/Sounds/pacman_munch.wav"); //used
	effectTimer = effectDuration;
	dead = false;
	invincible = true;
}