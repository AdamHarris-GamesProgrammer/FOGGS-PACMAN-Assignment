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
#include "BigMunchie.h"
#include "CommonOperations.h"
#include "ScoreManager.h"

struct PauseScreen {
	//pause screen data
	S2D::Texture2D* pauseScreenBackground;
	S2D::Rect* pauseScreenRectangle;
};

struct StartMenuScreen {
	//start menu data
	S2D::Texture2D* startMenuLogo;
	S2D::Rect* startMenuRectangle;
	S2D::Vector2* startMenuPosition;
};

struct GameGUI {
	//GUI Data
	S2D::Vector2* highScoreTitleStringPosition;
	S2D::Vector2* highScoreStringPosition;
	S2D::Vector2* scoreStringPosition;
};

struct UI {
	StartMenuScreen* startMenu;
	PauseScreen* pauseScreen;
	GameGUI* gameGUI;

};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public S2D::Game
{
private:
	enum GameStates {
		MAIN_MENU = 0,
		HOW_TO_PLAY,
		GAME,
		PAUSED,
		GAME_OVER,
		GAME_WIN
	};

	GameStates states;

	int remainingMunchies;
	int playerScore = 0;
	int highScore = 0;

	ResourceLoaders rl;
	ScoreManager scoreManager;

	// Frame counter
	int frameCount;

	UI* uiSystem;
	Player* pacman;
	Cherry* cherry;
	S2D::Texture2D* munchieTexture;
	S2D::Texture2D* bigMunchieTexture;
	S2D::Texture2D* ghostTexture;

	Enemy* ghosts[GHOSTCOUNT];

	std::vector<GameObject*> gameObjects;
	Munchies* munchies[MUNCHIE_COUNT];

	BigMunchie* bigMunchies[BIG_MUNCHIE_COUNT];

	//Sound effects
	S2D::SoundEffect* pacmanBeginningSound;
	S2D::SoundEffect* pacmanIntermissionSound;

	Collisions collisionInstance;

	//sound control bools
	bool hasIntroMusicPlayed = false;
	bool hasIntermissionSoundPlayed = false;

private:

	void CheckGhostCollisions();
	void CheckMunchieCollisions();
	void CheckCherryCollisions();
	void CheckBigMunchieCollisions();

	void CollisionCheck();

	S2D::Texture2D* playspaceTexture;
	S2D::Texture2D* howToPlayTexture;
	S2D::Texture2D* gameWinTexture;
	S2D::Texture2D* gameOverTexture;

	CommonOperations co;


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

	void PollInput();

	void SpawnObjects();
	void Respawn();

	//pause and start control bools
	bool isGameStarted = false;
	bool isPaused = false;
	bool ispKeyDown = false;
	bool isSpaceKeydown = false;

	bool deathSoundPlayed = false;
	bool powerUpMode = false;
};


/* WEDNESDAY 
make it so munchies cant overlap each other
*/

/* Thursday
add help menu prompt to the main menu screen
improve pause screen looks
Create some form of GUI class
*/

/* FRIDAY
Ghosts can bump into each other and bounce off one another
Implement all sounds available
Leader board screen
*/