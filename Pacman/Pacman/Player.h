#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include "GameObject.h"

class Player : public GameObject{

	float speedMultiplier; //movement speed
	float effectTimer; //timer for the ghost freeze ability
	const float effectDuration = 10.0f;

	enum Direction { //direction enum is used for animation and movement systems
		RIGHT,
		UP,
		LEFT,
		DOWN,
		STILL
	};

	Direction direction; //creates a instance of the direction enum

	//sound control variables
	bool hasChompedSoundPlayed = false;
	bool deathSoundPlayed = false;
	bool powerUpActive = false;

	void Movement(int elapsedTime); //moves the player
	void ScreenWrap(); //allows the player to wrap through to the opposite side of the map

public:
	void Update(int elapsedTime, int frameCount) override; //overrides the base class's update method
	void Render(); //renders pacman
	void PlaySound(S2D::SoundEffect* sound); //plays a sound that is passed through to it
	void SetPowerUp(bool newValue) { powerUpActive = newValue; } //sets the power up variable to a new value
	bool GetPowerUp() { return powerUpActive; } //gets the powerup value
	bool dead; //is the player dead
	bool hasDeathAnimPlayed; //animation control variable
	Player(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect); //constructor
	
	//sound effect variables
	S2D::SoundEffect* pacmanChompSound;
	S2D::SoundEffect* pacmanEatFruitSound;
	S2D::SoundEffect* pacmanEatGhostSound;
	S2D::SoundEffect* pacmanExtraPacSound;
	S2D::SoundEffect* pacmanMunchSound;

	//invincibility timer variables
	bool invincible;
	float invincibilityTimer = 4.0f;

	//gets the players input
	void PollInput();

};
#endif