#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

//includes S2D
#include "S2D/S2D.h"

//includes custom object files
#include "GameObject.h"
#include "Player.h"
#include "Cherry.h"
#include "Munchies.h"
#include "Enemy.h"
#include "Collisions.h"
#include "BigMunchie.h"

//includes utility files
#include "Constants.h"
#include "ResourceLoaders.h"
#include "CommonOperations.h"
#include "ScoreManager.h"

//includes standard library classes
#include <iostream>
#include <vector>
#include <typeinfo>
#include <sstream>
#include <time.h>

//this struct allows me to hold the elements of the GUI in a singular location
struct GameGUI {
	S2D::Vector2* highScoreTitleStringPosition;
	S2D::Vector2* highScoreStringPosition;
	S2D::Vector2* scoreStringPosition;
};


class Pacman : public S2D::Game
{
private:
	//GameStates enum handles what state the game is currently in allowing the code to be more efficient then gigantic if blocks
	enum GameStates {
		MAIN_MENU = 0,
		HOW_TO_PLAY,
		GAME,
		PAUSED,
		GAME_OVER,
		GAME_WIN
	};

	//GameStates object used to control the game flow
	GameStates states;

	//score related variables
	int remainingMunchies;
	int playerScore = 0;
	int highScore = 0;

	//utility classes
	ResourceLoaders rl;
	CommonOperations co;
	ScoreManager scoreManager;
	Collisions collisionInstance;

	// Frame counter
	int frameCount;

	GameGUI* gameUI;

	//Textures, stops the need to load the same texture multiple times
	S2D::Texture2D* munchieTexture;
	S2D::Texture2D* bigMunchieTexture;
	S2D::Texture2D* ghostTexture;

	//UI related textures
	S2D::Texture2D* playspaceTexture;
	S2D::Texture2D* howToPlayTexture;
	S2D::Texture2D* gameWinTexture;
	S2D::Texture2D* gameOverTexture;
	S2D::Texture2D* startMenuLogo;
	S2D::Texture2D* pauseScreenBackground;

	//Game objects
	std::vector<GameObject*> gameObjects;
	Player* pacman;
	Cherry* cherry;
	Munchies* munchies[MUNCHIE_COUNT];
	BigMunchie* bigMunchies[BIG_MUNCHIE_COUNT];
	Enemy* ghosts[GHOSTCOUNT];

	//Sound effects
	S2D::SoundEffect* pacmanBeginningSound;
	S2D::SoundEffect* pacmanIntermissionSound;

	//sound control bools
	bool hasIntroMusicPlayed = false;
	bool hasIntermissionSoundPlayed = false;

	/// <summary> The collision check system compares the types of objects collided and then decides what to do based off that
	void CollisionCheck();
public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent() override;

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime) override;

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime) override;

	/// <summary> Draws the GUI </summary>
	void DrawGUI();

	/// <summary> Polls the input, called every cycle </summary>
	void PollInput();

	/// <summary> Spawns the game objects </summary>
	void SpawnObjects();

	/// <summary> Enables the game space to be reset </summary>
	void ResetValues();

	//space and p key toggle variables
	bool ispKeyDown = false;
	bool isSpaceKeydown = false;

	//enables the powerup mode (freezes ghosts)
	bool powerUpMode = false;
};

/* GRAPHICAL TODOS
add help menu prompt to the main menu screen
improve pause screen looks
*/
