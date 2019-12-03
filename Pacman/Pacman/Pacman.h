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
	void CheckCherryCollisions();

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


/* WEDNESDAY 
Implement new munchies into game
Program big munchie functionality (add additional AI type to enemy for running away from player)
Implement cherry pickup collider and ability to spawn every 15 seconds
make it so munchies cant overlap each other
*/

/* Thursday
Create all GUI screens (Level Fail, Level Win, How to play, highscore table)
Implement these
Add in game states enum to control this system
Add in a reset and reload method for the game allowing a actual end and infitite replayability
(maybe add a ghost each time the map is cleared
*/

/* FRIDAY
Ghosts can bump into each other and bounce off one another
Implement all sounds available
Highscore system
*/