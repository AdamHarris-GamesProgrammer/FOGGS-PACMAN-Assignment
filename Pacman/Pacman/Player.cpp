#include "Player.h"

void Player::Movement(int elapsedTime) //this method is responsible for moving the player
{
	float movementSpeed = speedMultiplier * elapsedTime; //gets the amount that the ghost should move this frame
	int directionValue = direction * 32; //gets the value that the source rectangle X parameter should be set to
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
	if (mPosition->X + mSrcRect->Width > SCREEN_WIDTH - SCREENX_OFFSET) mPosition->X = SCREENX_OFFSET + 0.01f; //if the player goes off the left hand side, teleport to the right
	else if (mPosition->X <= SCREENX_OFFSET) mPosition->X = SCREEN_WIDTH - mSrcRect->Width - SCREENX_OFFSET; //if the player goes off the right hand side, teleport to the left

	if (mPosition->Y + mSrcRect->Height > SCREEN_HEIGHT - SCREENY_OFFSET) mPosition->Y = SCREENY_OFFSET + 0.01f; //if the player goes off the top side, teleport to the bottom
	else if (mPosition->Y <= SCREENY_OFFSET) mPosition->Y = SCREEN_HEIGHT - mSrcRect->Height - SCREENY_OFFSET; //if the player goes off the bottom side, teleport to the top
}

void Player::PollInput()
{
	// Gets the current state of the keyboard
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	//traditional pacman movement system, as he can only move one direction at a time and the player isnt required to hold the buttons down
	if (keyboardState->IsKeyDown(S2D::Input::Keys::W)) direction = Direction::UP; //sets the direction value
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::A)) direction = Direction::LEFT;
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::S)) direction = Direction::DOWN;
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::D)) direction = Direction::RIGHT;
}

void Player::Update(int elapsedTime, int frameCount)
{
	if (!dead) { //if the player is not dead
		//this animation opens and closes pacman mouth as he moves
		if (frameCount < PREFFERRED_FPS / 8) { //first frame
			mSrcRect->X = 0;
			if (!hasChompedSoundPlayed) {
				PlaySound(pacmanChompSound);
				hasChompedSoundPlayed = true;
			}
		}
		else if (frameCount < PREFFERRED_FPS / 8 * 2) { //second frame
			mSrcRect->X = 32;
		}
		else if (frameCount < PREFFERRED_FPS / 8 * 3) { //third frame
			mSrcRect->X = 64;
		}
		else if (frameCount < PREFFERRED_FPS / 8 * 4) { //fourth frame
			mSrcRect->X = 96;
		}
		else if (frameCount < PREFFERRED_FPS / 8 * 5) { //fifth frame
			mSrcRect->X = 64;
		}
		else if (frameCount < PREFFERRED_FPS / 8 * 6) { //sixth frame
			mSrcRect->X = 32;
		}
		else if (frameCount < PREFFERRED_FPS / 8 * 7) { //seventh frame 
			mSrcRect->X = 0;
		}

		if (frameCount >= PREFFERRED_FPS) {
			hasChompedSoundPlayed = false; //stops the chomp sound from playing more than once a second
		}

		if (invincible) { //if invincible 
			invincibilityTimer -= 0.017; //start the timer
			if (invincibilityTimer <= 0.0f)
				invincible = false; //sets invincible to false when the timer has ran out
		}
		if (powerUpActive) { //if the powerup is active
			effectTimer -= 0.017;
			if (effectTimer <= 0.0f) {
				effectTimer = effectDuration;
				powerUpActive = false;
			}
		}
		PollInput(); //polls the input
		Movement(elapsedTime); //moves pacman
		ScreenWrap(); //wraps pacman
	}
	else { //if pacman is dead
		if (!deathSoundPlayed) {
			PlaySound(pacmanMunchSound);
			if (pacmanMunchSound->GetState() == S2D::SoundEffectState::PLAYING) {
				deathSoundPlayed = true;
			}
		}

		direction = Direction::STILL; //stops him from moving
		if (frameCount < PREFFERRED_FPS / 8) { //frame 1
			mSrcRect->Y = 128;
			mSrcRect->X = 0;
		}
		else if (frameCount < PREFFERRED_FPS / 8 * 2) mSrcRect->X = 32; //frame 2		 
		else if (frameCount < PREFFERRED_FPS / 8 * 3) mSrcRect->X = 64;	//frame 3
		else if (frameCount < PREFFERRED_FPS / 8 * 4) mSrcRect->X = 96; //frame 4
		else if (frameCount < PREFFERRED_FPS / 8 * 5) { //frame 5, switches to next row on the sprite sheet
			mSrcRect->Y = 160;
			mSrcRect->X = 0;
		}
		else if (frameCount < PREFFERRED_FPS / 8 * 6) mSrcRect->X = 32; //frame 6
		else if (frameCount < PREFFERRED_FPS / 8 * 7) mSrcRect->X = 64; //frame 7
		else if (frameCount < PREFFERRED_FPS / 8 * 8) mSrcRect->X = 96; //frame 8
		else hasDeathAnimPlayed = true; //once a second has passed then set this to true, a second has passed as preferred fps is greater than 60
	}
}

void Player::Render()
{
	if (!hasDeathAnimPlayed) //if the death animation has not played then render 
		S2D::SpriteBatch::Draw(mTexture, mPosition, mSrcRect);
}


void Player::PlaySound(S2D::SoundEffect* sound)
{
	S2D::Audio::Play(sound); //plays the passed through sound
}

Player::Player(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect)
{
	direction = Direction::STILL; //sets the direction to still until the player decides what direction to go in
	speedMultiplier = 0.16f; //sets the movement speed

	//initializes the sound variables
	pacmanChompSound = rl.LoadSound("Assets/Sounds/pacman_chomp.wav"); 
	pacmanEatFruitSound = rl.LoadSound("Assets/Sounds/pacman_eatfruit.wav");
	pacmanExtraPacSound = rl.LoadSound("Assets/Sounds/pacman_extrapac.wav"); 
	pacmanMunchSound = rl.LoadSound("Assets/Sounds/pacman_munch.wav"); 
	pacmanExtraPacSound = rl.LoadSound("Assets/Sounds/pacman_extrapac.wav");

	//sets up the first timer
	effectTimer = effectDuration;

	//sets bool variables
	dead = false;
	deathSoundPlayed = false;
	hasDeathAnimPlayed = false;
	invincible = true;
}