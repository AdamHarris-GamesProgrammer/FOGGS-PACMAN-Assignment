#include "Pacman.h"
#include "Player.h"
#include "ResourceLoaders.h"
#include "GameObject.h"

#include <sstream>
#include <iostream>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	srand(time(nullptr));

	frameCount = 0;

	uiSystem = new UI();
	uiSystem->startMenu = new StartMenuScreen();
	uiSystem->pauseScreen = new PauseScreen();
	uiSystem->gameGUI = new GameGUI();
	collisionInstance = Collisions();

	states = GameStates::MAIN_MENU;

	//Initialise important Game aspects
	S2D::Audio::Initialise(); //TODO: Uncomment this to enable sounds

	S2D::Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", PREFFERRED_FPS);
	S2D::Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	S2D::Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete pacman;

	delete cherry;

	delete munchieTexture;

	delete[] ghosts;
	delete munchieTexture;

	delete playspaceTexture;

	delete uiSystem->startMenu->startMenuLogo;
	delete uiSystem->startMenu->startMenuPosition;
	delete uiSystem->startMenu->startMenuRectangle;
	delete uiSystem->startMenu;

	delete uiSystem->pauseScreen->pauseScreenBackground;
	delete uiSystem->pauseScreen->pauseScreenRectangle;
	delete uiSystem->pauseScreen;

	delete uiSystem->gameGUI->highScoreStringPosition;
	delete uiSystem->gameGUI->highScoreTitleStringPosition;
	delete uiSystem->gameGUI->scoreStringPosition;
	delete uiSystem->gameGUI;

	delete uiSystem;

	delete pacmanBeginningSound;
}

void Pacman::LoadContent()
{
	SpawnObjects();

	//load menu background
	uiSystem->pauseScreen->pauseScreenBackground = rl.LoadTexture("Assets/Textures/PacmanPauseMenu.png");
	uiSystem->pauseScreen->pauseScreenRectangle = new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

	uiSystem->startMenu->startMenuLogo = rl.LoadTexture("Assets/Textures/PacmanLogo.png");
	uiSystem->startMenu->startMenuRectangle = new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
	uiSystem->startMenu->startMenuPosition = new S2D::Vector2(0, 0);

	//load sound effects music
	pacmanBeginningSound = rl.LoadSound("Assets/Sounds/pacman_beginning.wav"); //used
	pacmanIntermissionSound = rl.LoadSound("Assets/Sounds/pacman_intermission.wav", true); //used

	//load GUI;
	uiSystem->gameGUI->highScoreTitleStringPosition = new S2D::Vector2((SCREEN_WIDTH / 2) - 64, 16);
	uiSystem->gameGUI->highScoreStringPosition = new S2D::Vector2((SCREEN_WIDTH / 2), 32);
	uiSystem->gameGUI->scoreStringPosition = new S2D::Vector2((SCREEN_WIDTH / 4), 32);

	playspaceTexture = rl.LoadTexture("Assets/Textures/Playspace.png");
	
	gameOverTexture = rl.LoadTexture("Assets/MenuScreens/GameOver.png");
	gameWinTexture = rl.LoadTexture("Assets/MenuScreens/GameWin.png");
	howToPlayTexture = rl.LoadTexture("Assets/MenuScreens/HowToPlay.png");

	highScore = scoreManager.GetHighscore();
}

void Pacman::Update(int elapsedTime)
{
	PollInput();
	switch (states) {
	case GameStates::MAIN_MENU:
		//TODO: Continue here
		break;
	case GameStates::HOW_TO_PLAY:

		break;
	case GameStates::GAME:
		S2D::Audio::Stop(pacmanIntermissionSound);
		if (!hasIntroMusicPlayed) {
			S2D::Audio::Play(pacmanBeginningSound);
			hasIntroMusicPlayed = true;
		}
		
		frameCount++;

		for (auto & munchie : munchies) {
			if (munchie != nullptr)
				munchie->Update(elapsedTime, frameCount);
		}

		for each (GameObject * object in gameObjects)
		{
			object->Update(elapsedTime, frameCount);
		}
		if (!pacman->invincible) {
			CheckGhostCollisions();

		}
		CheckMunchieCollisions();
		CheckCherryCollisions();
		CheckBigMunchieCollisions();

		if (pacman->GetPowerUp()) {
			powerUpMode = true;

		}

		if (powerUpMode && !pacman->GetPowerUp()) {
			for (auto& ghost : ghosts) {
				ghost->frozen = false;
			}
			powerUpMode = false;
		}
		break;
	case GameStates::PAUSED:
		if (!hasIntermissionSoundPlayed) {
			S2D::Audio::Play(pacmanIntermissionSound);
			hasIntermissionSoundPlayed = true;
		}
		break;
	case GameStates::GAME_OVER:
		if (!deathSoundPlayed) {
			pacman->PlaySound(pacman->pacmanMunchSound);
			if (pacman->pacmanMunchSound->GetState() == S2D::SoundEffectState::PLAYING) {
				deathSoundPlayed = true;
			}
		}
		break;
	case GameStates::GAME_WIN:

		break;

	};
}

void Pacman::Draw(int elapsedTime)
{
	S2D::SpriteBatch::BeginDraw(); // Starts Drawing

	switch (states) {
	case GameStates::MAIN_MENU:
		S2D::SpriteBatch::Draw(uiSystem->startMenu->startMenuLogo, uiSystem->startMenu->startMenuPosition, uiSystem->startMenu->startMenuRectangle);
		break;
	case GameStates::HOW_TO_PLAY:
		S2D::SpriteBatch::Draw(howToPlayTexture, new S2D::Vector2(0, 0), new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT));
		break;
	case GameStates::GAME:
		S2D::SpriteBatch::Draw(playspaceTexture, new S2D::Vector2(32, 32), new S2D::Rect(0.0f, 0.0f, 960, 704));
		for each (Munchies * munchie in munchies)
		{
			munchie->Render();
		}

		for each (BigMunchie * munchie in bigMunchies) {
			munchie->Render();
		}

		for each (GameObject * object in gameObjects)
		{
			if (object != pacman)
				object->Render();
			else {
				pacman->Render();
			}
		}

		if (frameCount >= PREFFERRED_FPS)
			frameCount = 0;

		DrawGUI();
		break;
	case GameStates::PAUSED:
		S2D::SpriteBatch::Draw(uiSystem->pauseScreen->pauseScreenBackground, uiSystem->pauseScreen->pauseScreenRectangle, nullptr);
		break;
	case GameStates::GAME_OVER:
		S2D::SpriteBatch::Draw(gameOverTexture, new S2D::Vector2(0, 0), new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT));
		break;
	case GameStates::GAME_WIN:
		S2D::SpriteBatch::Draw(gameWinTexture, new S2D::Vector2(0, 0), new S2D::Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT));
		break;

	};
	S2D::SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::DrawGUI() {
	std::stringstream highScoreTitleStream;
	highScoreTitleStream << "HIGH SCORE";

	if (playerScore >= highScore) {
		highScore = playerScore;
	}

	std::stringstream highScoreStream;
	highScoreStream << highScore;

	std::stringstream scoreStream;
	scoreStream << playerScore;

	S2D::SpriteBatch::DrawString(highScoreTitleStream.str().c_str(), uiSystem->gameGUI->highScoreTitleStringPosition, S2D::Color::White);
	S2D::SpriteBatch::DrawString(highScoreStream.str().c_str(), uiSystem->gameGUI->highScoreStringPosition, S2D::Color::White);
	S2D::SpriteBatch::DrawString(scoreStream.str().c_str(), uiSystem->gameGUI->scoreStringPosition, S2D::Color::White);
}

void Pacman::PollInput()
{
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();
	S2D::Input::MouseState* mouseState = S2D::Input::Mouse::GetState();
	switch (states)
	{
	case Pacman::MAIN_MENU:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE) && !isSpaceKeydown) {
			isSpaceKeydown = true;
			states = GameStates::GAME;
		}
		if (keyboardState->IsKeyUp(S2D::Input::Keys::SPACE)) { //allows space bar to toggle, stops the player from leaving the help menu and spawning straight into game
			isSpaceKeydown = false;
		}
		
		if (keyboardState->IsKeyDown(S2D::Input::Keys::H)) {
			states = GameStates::HOW_TO_PLAY;
		}

		if (!hasIntermissionSoundPlayed) {
			S2D::Audio::Play(pacmanIntermissionSound);
			hasIntermissionSoundPlayed = true;
		}
		break;
	case Pacman::HOW_TO_PLAY:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE) && !isSpaceKeydown) {
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
		hasIntermissionSoundPlayed = true;
		break;
	case Pacman::GAME_OVER:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::R)) {
			Respawn();
			states = GameStates::GAME;
		}
		break;
	case Pacman::GAME_WIN:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE)) {
			states = GameStates::GAME;
			SpawnObjects();
		}
		break;
	default:
		break;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::ESCAPE)) { //QUIT
		S2D::Graphics::Destroy();
	}
}

void Pacman::SpawnObjects()
{
	remainingMunchies = MUNCHIE_COUNT;
	gameObjects.clear();
	pacman = new Player(rl.LoadTexture("Assets/Textures/Pacman.tga"), co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 32, 32));

	cherry = new Cherry(rl.LoadTexture("Assets/Textures/Cherry.png"), co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 32, 32));

	gameObjects.push_back(pacman);
	gameObjects.push_back(cherry);

	ghostTexture = rl.LoadTexture("Assets/Textures/GhostSheet.png");
	for (auto& ghost : ghosts)
	{
		ghost = new Enemy(ghostTexture, co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 20, 20), pacman);
		gameObjects.push_back(ghost);
	}

	munchieTexture = rl.LoadTexture("Assets/Textures/Munchie.png");
	// Load Munchie
	for (auto & munchie : munchies) {
		munchie = new Munchies(munchieTexture, co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 8, 8));
	}

	bigMunchieTexture = rl.LoadTexture("Assets/Textures/BigMunchie.png");
	for (auto & bigMunchie : bigMunchies) {
		bigMunchie = new BigMunchie(bigMunchieTexture, co.GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 10, 10));
	}
}


void Pacman::Respawn()
{
	deathSoundPlayed = false;
	hasIntermissionSoundPlayed = false;
	hasIntroMusicPlayed = false;
	playerScore = 0;

	SpawnObjects();
}

void Pacman::CheckGhostCollisions()
{
	for (auto& ghost : ghosts) {
		if (!ghost->frozen) {
			if (collisionInstance.CheckCollisions(pacman, ghost)) {
				if (playerScore > scoreManager.GetHighscore()) {
					scoreManager.SaveScore(playerScore);
				}
				states = GameStates::GAME_OVER;
				pacman->dead = true;

			}
		}
	}

}

void Pacman::CheckMunchieCollisions()
{
	for (auto & munchie : munchies) {
		if (munchie != nullptr) {
			if (collisionInstance.CheckCollisions(pacman, munchie)) {
				playerScore += 10;
				munchie = nullptr;
				pacman->PlaySound(pacman->pacmanEatFruitSound);
				remainingMunchies--;
				if (remainingMunchies <= 0) {
					states = GameStates::GAME_WIN;
				}
			}
		}

	}
}


void Pacman::CheckCherryCollisions()
{
	if (cherry != nullptr) {
		if (collisionInstance.CheckCollisions(pacman, cherry)) {
			playerScore += 100;
			cherry->SetPosition(new S2D::Vector2(1100, 1100));
			cherry->SetTimer = true;
			pacman->PlaySound(pacman->pacmanEatFruitSound);
		}
	}
}

void Pacman::CheckBigMunchieCollisions()
{
	for (auto & bigMunchie : bigMunchies) {
		if (bigMunchie != nullptr) {
			if (collisionInstance.CheckCollisions(pacman, bigMunchie)) {
				pacman->SetPowerUp(true);
				bigMunchie = nullptr;
				playerScore += 100;
				pacman->PlaySound(pacman->pacmanEatFruitSound);
				for (auto& ghost : ghosts) {
					ghost->frozen = true;
				}
			}
		}
	}
}


