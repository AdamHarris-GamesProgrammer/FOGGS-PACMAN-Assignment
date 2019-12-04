#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include "S2D/S2D.h"
#include "Constants.h"
#include "Texture.h"
#include "ResourceLoaders.h"
#include "GameObject.h"

class Player : public GameObject{

	float speedMultiplier; // = 0.16f; = 1.0f when sprinting
	float effectTimer;
	const float effectDuration = 10.0f;

	enum Direction {
		RIGHT,
		UP,
		LEFT,
		DOWN
	};

	Direction direction;

	bool movingRight = false;
	bool movingLeft = false;
	bool movingUp = false;
	bool movingDown = false;

	bool hasChompedSoundPlayed = false;

	bool powerUpActive = false;


	void Movement(int elapsedTime);
	void ScreenWrap();

public:
	void Update(int elapsedTime, int frameCount) override;
	void Render();
	void PlaySound(S2D::SoundEffect* sound);
	void SetPowerUp(bool newValue) { powerUpActive = newValue; }
	bool GetPowerUp() { return powerUpActive; }
	bool dead;
	Player(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect);
	
	S2D::SoundEffect* pacmanChompSound;
	S2D::SoundEffect* pacmanEatFruitSound;
	S2D::SoundEffect* pacmanEatGhostSound;
	S2D::SoundEffect* pacmanExtraPacSound;
	S2D::SoundEffect* pacmanMunchSound;

	bool invincible;
	float invincibilityTimer = 4.0f;

	void PollInput();

};
#endif