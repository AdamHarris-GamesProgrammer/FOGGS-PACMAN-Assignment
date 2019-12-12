#include "Pacman.h"

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	//sets default state to main menu
	states = GameStates::MAIN_MENU;

	//creates GUI object
	gameUI = new GameGUI();

	//Initialise important Game aspects
	S2D::Audio::Initialise(); 

	S2D::Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", PREFFERRED_FPS);
	S2D::Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	S2D::Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	//deletes pacman
	delete pacman;

	//deletes cherry
	delete cherry;

	//deletes all munchies
	delete[] munchies;

	//deletes all big munchies
	delete[] bigMunchies;

	//deletes all ghosts
	delete[] ghosts;

	//deletes textures
	{
		{ //deletes game object textures
			delete bigMunchieTexture;
			delete ghostTexture;
			delete munchieTexture;
		}
		{ //deletes menu screen textures
			delete playspaceTexture;
			delete howToPlayTexture;
			delete gameWinTexture;
			delete gameOverTexture;
			delete startMenuLogo;
			delete pauseScreenBackground;
		}
	}

	//deletes gameUI
	{ //deletes gameUI struct and contained objects
		delete gameUI->highScoreStringPosition;
		delete gameUI->highScoreTitleStringPosition;
		delete gameUI->scoreStringPosition;
		delete gameUI;
	}

	//deletes sound effects
	{ //deletes sounds
		delete pacmanBeginningSound;
		delete pacmanIntermissionSound;
	}

}

void Pacman::LoadContent()
{
	//calls the SpawnObjects method which positions all munchies and ghosts etc.
	SpawnObjects();

	//load pause screen background
	pauseScreenBackground = rl.LoadTexture("Assets/MenuScreens/PacmanPauseMenu.png");

	//load main menu screen
	startMenuLogo = rl.LoadTexture("Assets/MenuScreens/PacmanLogo.png");

	//load GUI;
	gameUI->highScoreTitleStringPosition = new S2D::Vector2((SCREEN_WIDTH / 2) - 64, 16);
	gameUI->highScoreStringPosition = new S2D::Vector2((SCREEN_WIDTH / 2), 32);
	gameUI->scoreStringPosition = new S2D::Vector2((SCREEN_WIDTH / 4), 32);
	playspaceTexture = rl.LoadTexture("Assets/Textures/Playspace.png");

	//loads UI related textures
	gameOverTexture = rl.LoadTexture("Assets/MenuScreens/GameOver.png");
	gameWinTexture = rl.LoadTexture("Assets/MenuScreens/GameWin.png");
	howToPlayTexture = rl.LoadTexture("Assets/MenuScreens/HowToPlay.png");

	//load sound effects music
	pacmanBeginningSound = rl.LoadSound("Assets/Sounds/pacman_beginning.wav"); //used
	pacmanIntermissionSound = rl.LoadSound("Assets/Sounds/pacman_intermission.wav", true); //used

	//sets the highscore to the current score saved in the file
	highScore = scoreManager.GetHighscore();
}

void Pacman::Update(int elapsedTime)
{
	PollInput(); //gets the user's input
	switch (states) {
	case GameStates::MAIN_MENU:
		if (!hasIntermissionSoundPlayed) { //plays the intermission music if it is not already playing (this avoids the song from starting a new instance each frame)
			S2D::Audio::Play(pacmanIntermissionSound);
			hasIntermissionSoundPlayed = true;
		}
		break;
	case GameStates::GAME:
		hasIntermissionSoundPlayed = false; //sets this to false so that it can play on the pause menu
		if (!hasIntroMusicPlayed) { //plays the intro music if it has not already been played
			S2D::Audio::Play(pacmanBeginningSound); //this sound effect does not loop so when it ends signifies the end of the immortality period
			hasIntroMusicPlayed = true;
		}

		frameCount++; //increments the frame count variable

		for each (GameObject * object in gameObjects) //cycles through game object in the gameObjects vector and calls their update method
		{
			if (object != nullptr) { //if the object exits
				object->Update(elapsedTime, frameCount); //calls objects update
			}
		}

		CollisionCheck(); //calls the collision check method

		if (pacman->GetPowerUp()) { //if pacman is currently in powerup mode then turn on the powerUpMode variables
			powerUpMode = true;

		}

		if (powerUpMode && !pacman->GetPowerUp()) { //unfreezes the ghosts and then sets powerUpMode back to false
			for (auto& ghost : ghosts) { //uses a range based for loop to loop through each ghost and unfreeze them
				ghost->frozen = false;
			}
			powerUpMode = false;
		}
		break;
	case GameStates::PAUSED:
		if (!hasIntermissionSoundPlayed) { //plays the intermission sound effect if it has not already played
			S2D::Audio::Play(pacmanIntermissionSound);
			hasIntermissionSoundPlayed = true;
		}
		break;
	}; //GAME_OVER and HOW_TO_PLAY are missing as they have no objects to update during the frame
}

void Pacman::Draw(int elapsedTime)
{
	S2D::SpriteBatch::BeginDraw(); // Starts Drawing

	switch (states) {
	case GameStates::MAIN_MENU:
		S2D::SpriteBatch::Draw(startMenuLogo,new S2D::Vector2(0,0), new S2D::Rect(0.0f,0.0f, SCREEN_WIDTH, SCREEN_HEIGHT)); //draws the main menu
		break;
	case GameStates::HOW_TO_PLAY:
		S2D::SpriteBatch::Draw(howToPlayTexture, new S2D::Vector2(0, 0), new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT)); //draws the how to play menu
		break;
	case GameStates::GAME:
		S2D::SpriteBatch::Draw(playspaceTexture, new S2D::Vector2(32, 32), new S2D::Rect(0.0f, 0.0f, 960, 704)); //draws the play space texture
		if (!pacman->dead) { //if pacman is not dead
			for each (GameObject * object in gameObjects) //render every object
			{
				object->Render();
			}
			if (frameCount >= 60) frameCount = 0;
		}
		else { //if pacman is dead
			pacman->Render();  //then only render pacman
		}
		if (pacman->dead && pacman->hasDeathAnimPlayed) {
			states = GameStates::GAME_OVER; //switches state when pacman and the death animation has played
		}

		DrawGUI();
		break;
	case GameStates::PAUSED:
		S2D::SpriteBatch::Draw(playspaceTexture, new S2D::Vector2(32, 32), new S2D::Rect(0.0f, 0.0f, 960, 704)); //draws the play space texture
		for each (GameObject * object in gameObjects) //render every object
		{
			object->Render();
		}
		S2D::SpriteBatch::Draw(pauseScreenBackground, new S2D::Vector2(0, 0), new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT)); //draws the pause m

		break;
	case GameStates::GAME_OVER:
		S2D::SpriteBatch::Draw(gameOverTexture, new S2D::Vector2(0, 0), new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT)); //draws the game over screen
		break;
	case GameStates::GAME_WIN:
		S2D::SpriteBatch::Draw(gameWinTexture, new S2D::Vector2(0, 0), new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT)); //draws the game win screen
		break;

	};
	S2D::SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::DrawGUI() { 
	std::stringstream highScoreTitleStream;  //creates a string stream for the title 
	highScoreTitleStream << "HIGH SCORE";

	if (playerScore >= highScore) { 
		highScore = playerScore; //only changes the highscore displayed if the player score is greater than the current highscore
	}

	std::stringstream highScoreStream; //highscore string stream, allows the highscore to be displayed
	highScoreStream << highScore;

	std::stringstream scoreStream; //score string stream, allows the players current score to be displayed
	scoreStream << playerScore;

	//draws the GUI elements at their needed position in the colour white
	S2D::SpriteBatch::DrawString(highScoreTitleStream.str().c_str(), gameUI->highScoreTitleStringPosition, S2D::Color::White);
	S2D::SpriteBatch::DrawString(highScoreStream.str().c_str(), gameUI->highScoreStringPosition, S2D::Color::White);
	S2D::SpriteBatch::DrawString(scoreStream.str().c_str(), gameUI->scoreStringPosition, S2D::Color::White);
}

void Pacman::PollInput()
{
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState(); //creates a keyboard state variable, this is used to hold the current events of the keyboard
	switch (states)
	{
	case Pacman::MAIN_MENU: 
		//isSpaceKeydown is used a toggle variable to stop the player from accidentally starting the game after looking at the help menu
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE) && !isSpaceKeydown) {
			isSpaceKeydown = true;
			hasIntermissionSoundPlayed = false;
			states = GameStates::GAME;
			S2D::Audio::Stop(pacmanIntermissionSound);
		} //if the player presses spaces and the space key is up then it will switch to the game states
		if (keyboardState->IsKeyUp(S2D::Input::Keys::SPACE)) { //allows space bar to toggle, stops the player from leaving the help menu and spawning straight into game
			isSpaceKeydown = false;
		} //sets is space key down to false

		if (keyboardState->IsKeyDown(S2D::Input::Keys::H)) { //if the user presses the h key then they will go to the how to play menu screen
			states = GameStates::HOW_TO_PLAY;
		}
		break;
	case Pacman::HOW_TO_PLAY:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE) && !isSpaceKeydown) { //returns to the main menu
			isSpaceKeydown = true;
			states = GameStates::MAIN_MENU;
		}

		if (keyboardState->IsKeyUp(S2D::Input::Keys::SPACE)) {
			isSpaceKeydown = false;
		}
		break;
	case Pacman::GAME:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::P) && !ispKeyDown) { //PAUSE
			ispKeyDown = true;
			states = GameStates::PAUSED;
		}

		if (keyboardState->IsKeyUp(S2D::Input::Keys::P)) { //allows pause to toggle 
			ispKeyDown = false;
		}
		break;
	case Pacman::PAUSED:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::P) && !ispKeyDown) { //PAUSE
			ispKeyDown = true;
			states = GameStates::GAME;
		}

		if (keyboardState->IsKeyUp(S2D::Input::Keys::P)) { //allows pause to toggle 
			ispKeyDown = false;
		}

		S2D::Audio::Stop(pacmanIntermissionSound);
		hasIntermissionSoundPlayed = true; //stops the intermission sound from continuous playing
		break;
	case Pacman::GAME_OVER:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::R)) { //restarts the game
			ResetValues(); //resets values
			states = GameStates::GAME; //sets the game to the game state
		}
		break;
	case Pacman::GAME_WIN:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE)) { //allows the player to play again and increase there score
			states = GameStates::GAME;
			SpawnObjects();
		}
		break;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::ESCAPE)) { //QUIT
		S2D::Graphics::Destroy();
	}
}

void Pacman::SpawnObjects()
{
	srand(time(nullptr)); //seeds the random

	frameCount = 0; //resets frame count

	collisionInstance = Collisions(); //creates the collisions instance

	remainingMunchies = MUNCHIE_COUNT; //sets the remaining munchie variable to the constant MUNCHIE_COUNT definition
	gameObjects.clear(); //cleats the gameObjects vector
	pacman = new Player(rl.LoadTexture("Assets/Textures/Pacman.tga"), co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 32, 32)); //creates pacman

	cherry = new Cherry(rl.LoadTexture("Assets/Textures/Cherry.png"), co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 32, 32)); //creates the cherry

	//adds pacman and the cherry to the gameObjects vector
	gameObjects.push_back(pacman);
	gameObjects.push_back(cherry);

	ghostTexture = rl.LoadTexture("Assets/Textures/GhostSheet.png"); //loads the texture for the ghosts
	for (auto& ghost : ghosts)
	{
		ghost = new Enemy(ghostTexture, co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 20, 20), pacman); //cycles through the ghosts array and instantiates a object for each element
		gameObjects.push_back(ghost); //pushes back the ghosts into the gameObjects vector
	}

	munchieTexture = rl.LoadTexture("Assets/Textures/Munchie.png"); //loads the munchie texture
	// Load Munchie
	for (auto& munchie : munchies) {
		munchie = new Munchies(munchieTexture, co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 8, 8)); //creates a munchie instance for each element in the munchies array
		gameObjects.push_back(munchie); //pushes back the munchies into the gameObjects vector
	}

	bigMunchieTexture = rl.LoadTexture("Assets/Textures/BigMunchie.png"); //loads the big munchie texture
	for (auto& bigMunchie : bigMunchies) {
		bigMunchie = new BigMunchie(bigMunchieTexture, co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 10, 10)); //cycles through the bigmuncheis array and creates a instance for each element 
		gameObjects.push_back(bigMunchie);
	}
}

void Pacman::ResetValues()
{
	//resets control variables
	hasIntermissionSoundPlayed = false;
	hasIntroMusicPlayed = false;

	//resets score
	playerScore = 0;

	//loads new sets of objects
	SpawnObjects();
}

void Pacman::CollisionCheck() {
	for (auto& object : gameObjects) { //cycles through every object in the gameObjects vector
		if (object != nullptr) { //if it is set to a object
			const std::type_info& type_info = typeid(*object); //gets the type of the object e.g. Ghost, Munchies, BigMunchie, Cherry
			if (type_info == typeid(Enemy)) { //if type equals enemy
				Enemy* enemy = (Enemy*)object; //casts the object to a enemy pointer allowing us to access the enemies functions
				if (!enemy->frozen && !pacman->invincible) { //if pacman is not frozen and pacman is not invincible 
					if (collisionInstance.CheckCollisions(pacman, enemy)) {  //the check for collisions
						//if a collision is found 
						//this is the player death area, here the score is saved if it is higher than the highscore
						if (playerScore > scoreManager.GetHighscore()) {
							scoreManager.SaveScore(playerScore);
						}
						if (!pacman->dead) {
							frameCount = 0; //sets the frame count to 0, allows pacman death animation to fully play
						}
						pacman->dead = true; //sets pacman->dead to true
					}
				}
			}
			else if (type_info == typeid(Munchies)) { //if object type is equal to munchies
				if (collisionInstance.CheckCollisions(pacman, (Munchies*)object)) { //if there is a collision
					playerScore += 10; //adds 10 to the players score
					pacman->PlaySound(pacman->pacmanEatFruitSound); //plays sound
					remainingMunchies--; //decrements remaining munchies
					if (remainingMunchies <= 0) { //tests for win condition
						pacman->PlaySound(pacman->pacmanExtraPacSound); //plays win sound effect
						states = GameStates::GAME_WIN; //sets the state to game win
					}
					object = nullptr; //sets object to nullptr, essentially taking it out of the logic for the game
				}
			}
			else if (type_info == typeid(BigMunchie)) { //if object type is equal to big munchies
				if (collisionInstance.CheckCollisions(pacman, (GameObject*)object)) { //if there is a collision
					pacman->SetPowerUp(true); //sets the powerup state on pacman
					object = nullptr; //sets this object to nullptr, taking it out of the logic for the game
					playerScore += 100; //adds 100 to the score
					pacman->PlaySound(pacman->pacmanEatFruitSound); //plays the eat fruit sound
					for (auto& ghost : ghosts) {
						ghost->frozen = true; //freezes all the ghosts
					}
				}
			}
			else if (type_info == typeid(Cherry)) { //if object type is equal to cherry
				if (collisionInstance.CheckCollisions(pacman, cherry)) { //if there is a collision
					playerScore += 100; //add 100 to score
					cherry->SetPosition(new S2D::Vector2(1100, 1100)); //move cherry off screen
					cherry->SetTimer = true; //sets the timer for the respawn
					pacman->PlaySound(pacman->pacmanExtraPacSound); //plays sound effect
				}
			}
		}

	}
}