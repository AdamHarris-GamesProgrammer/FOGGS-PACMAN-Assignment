#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include "Constants.h"
#include "Texture.h"
#include "ResourceLoaders.h"
#include "GameObject.h"
#include "Player.h"
#include "Cherry.h"
#include <vector>
#include "Munchies.h"
#include "Enemy.h"
#include "Collisions.h"
#include <iostream>
#include <fstream>



// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;


struct PauseScreen {
	//pause screen data
	Texture2D* pauseScreenBackground;
	Rect* pauseScreenRectangle;
};

struct StartMenuScreen {
	//start menu data
	Texture2D* startMenuLogo;
	Rect* startMenuRectangle;
	Vector2* startMenuPosition;
};

struct GameGUI {
	//GUI Data
	Vector2* highScoreTitleStringPosition;
	Vector2* highScoreStringPosition;
	Vector2* scoreStringPosition;

	Texture2D* extraPacmanSprite;
	Vector2* extraPacmanSpritePosition;
	Rect* extraPacmanSpriteRect;
};

struct UI {
	StartMenuScreen* startMenu;
	PauseScreen* pauseScreen;
	GameGUI* gameGUI;

};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:


	int playerScore = 0;
	int highScore = 0;

	ResourceLoaders rl;
	// Frame counter
	int frameCount;

	UI* uiSystem;
	Player* pacman;
	Cherry* cherry;
	Texture2D* munchieTexture;
	Texture2D* ghostTexture;

	Enemy* ghosts[GHOSTCOUNT];

	std::vector<GameObject*> gameObjects;
	Munchies* munchies[MUNCHIE_COUNT];

	//Sound effects
	SoundEffect* pacmanBeginningSound;
	SoundEffect* pacmanIntermissionSound;

	Collisions collisionInstance;

	//Texture2D* playspaceTexture;

	//sound control bools
	bool hasIntroMusicPlayed = false;
	bool hasChompedSoundPlayed = false;
	bool hasIntermissionSoundPlayed = false;

	std::fstream statsFile;

private:
	Vector2* GeneratePositionWithinGameBounds();
	void CheckGhostCollisions();
	void CheckMunchieCollisions();

	void SaveScore();
	void LoadScore();
	void DeleteScore();

	Texture2D* playspaceTexture;


public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	/// <summary> Draws the GUI </summary>
	void DrawGUI();

	void PollInput();

	//pause and start control bools
	bool isGameStarted = false;
	bool isPaused = false;
	bool ispKeyDown = false;

	bool deathSoundPlayed = false;
};
//TODO: each ai type has a different sprite allowing players to distinguish them
//TODO: make it so ghosts cant bump into each other
//TODO: Implement all sounds
//TODO: Eat ghosts power for 5 seconds, this causes them to run to the exit and then respawn 10 seconds later with a new ai type
//TODO: Implement multiple lives
//TODO: Add in different ghost sprites and animations for them
//TODO: add game over screen
//TODO: reload the game when all munchies have been picked up
//TODO: Add in powerup munchies
//TODO: Add in ghost AI
//TODO: make cherry spawn after random amount of time
//TODO: Add in highscore functionality
//TOOD: Different munchies
//TODO: Add in a high score leader board
