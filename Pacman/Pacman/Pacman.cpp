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

	statsFile = fstream("Assets/Data/Scores.txt", std::fstream::out);
	states = GameStates::MAIN_MENU;

	//Initialise important Game aspects
	Audio::Initialise(); //TODO: Uncomment this to enable sounds

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

	//delete playspaceTexture;

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

	statsFile.close();
}

void Pacman::LoadContent()
{
	pacman = new Player(rl.LoadTexture("Assets/Textures/Pacman.tga"), new S2D::Vector2(350.f, 350.0f), new S2D::Rect(0.0f, 0.0f, 32, 32));

	cherry = new Cherry(rl.LoadTexture("Assets/Textures/Cherry.png"), new S2D::Vector2(300.0f, 300.0f), new S2D::Rect(0.0f, 0.0f, 32, 32));

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
	for (int i = 0; i < MUNCHIE_COUNT; i++) {
		munchies[i] = new Munchies(munchieTexture, co.GeneratePositionWithinGameBounds(), new Rect(0.0f, 0.0f, 8, 8));
	}

	bigMunchieTexture = rl.LoadTexture("Assets/Textures/BigMunchie.png");
	for (int i = 0; i < BIG_MUNCHIE_COUNT; i++) {
		bigMunchies[i] = new BigMunchie(bigMunchieTexture, co.GeneratePositionWithinGameBounds(), new Rect(0.0f, 0.0f, 10, 10));
	}

	LoadScore();

	//load menu background
	uiSystem->pauseScreen->pauseScreenBackground = rl.LoadTexture("Assets/Textures/PacmanPauseMenu.png");
	uiSystem->pauseScreen->pauseScreenRectangle = new Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);

	uiSystem->startMenu->startMenuLogo = rl.LoadTexture("Assets/Textures/PacmanLogo.png");
	uiSystem->startMenu->startMenuRectangle = new Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
	uiSystem->startMenu->startMenuPosition = new Vector2(0, 0);

	//load sound effects music
	pacmanBeginningSound = rl.LoadSound("Assets/Sounds/pacman_beginning.wav"); //used
	pacmanIntermissionSound = rl.LoadSound("Assets/Sounds/pacman_intermission.wav", true); //used

	//load GUI;
	uiSystem->gameGUI->highScoreTitleStringPosition = new Vector2((SCREEN_WIDTH / 2) - 64, 16);
	uiSystem->gameGUI->highScoreStringPosition = new Vector2((SCREEN_WIDTH / 2), 32);
	uiSystem->gameGUI->scoreStringPosition = new Vector2((SCREEN_WIDTH / 4), 32);

	playspaceTexture = rl.LoadTexture("Assets/Textures/Playspace.png");
	
	gameOverTexture = rl.LoadTexture("Assets/MenuScreens/GameOver.png");
	gameWinTexture = rl.LoadTexture("Assets/MenuScreeens/GameWin.png");
	howToPlayTexture = rl.LoadTexture("Assets/MenuScreens/HowToPlay.png");
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
		Audio::Stop(pacmanIntermissionSound);
		if (!hasIntroMusicPlayed) {
			Audio::Play(pacmanBeginningSound);
			hasIntroMusicPlayed = true;
		}
		
		frameCount++;

		for (int i = 0; i < MUNCHIE_COUNT; i++) {
			if (munchies[i] != nullptr)
				munchies[i]->Update(elapsedTime, frameCount);
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
			Audio::Play(pacmanIntermissionSound);
			hasIntermissionSoundPlayed = true;
		}
		break;
	case GameStates::GAME_OVER:
		if (!deathSoundPlayed) {
			pacman->PlaySound(pacman->pacmanMunchSound);
			if (pacman->pacmanMunchSound->GetState() == SoundEffectState::PLAYING) {
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
	SpriteBatch::BeginDraw(); // Starts Drawing

	switch (states) {
	case GameStates::MAIN_MENU:
		SpriteBatch::Draw(uiSystem->startMenu->startMenuLogo, uiSystem->startMenu->startMenuPosition, uiSystem->startMenu->startMenuRectangle);
		break;
	case GameStates::HOW_TO_PLAY:
		SpriteBatch::Draw(howToPlayTexture, new Vector2(0, 0), new Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT));
		break;
	case GameStates::GAME:
		SpriteBatch::Draw(playspaceTexture, new Vector2(32, 32), new Rect(0.0f, 0.0f, 960, 704));
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
		SpriteBatch::Draw(uiSystem->pauseScreen->pauseScreenBackground, uiSystem->pauseScreen->pauseScreenRectangle, nullptr);
		break;
	case GameStates::GAME_OVER:
		SpriteBatch::Draw(gameOverTexture, new Vector2(0, 0), new Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT));
		break;
	case GameStates::GAME_WIN:
		SpriteBatch::Draw(gameWinTexture, new Vector2(0, 0), new Rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT));
		break;

	};
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::DrawGUI() {
	std::stringstream highScoreTitleStream;
	highScoreTitleStream << "HIGH SCORE";

	if (playerScore >= highScore) {
		highScore = playerScore;
		SaveScore();
	}

	std::stringstream highScoreStream;
	highScoreStream << highScore;

	std::stringstream scoreStream;
	scoreStream << playerScore;

	SpriteBatch::DrawString(highScoreTitleStream.str().c_str(), uiSystem->gameGUI->highScoreTitleStringPosition, Color::White);
	SpriteBatch::DrawString(highScoreStream.str().c_str(), uiSystem->gameGUI->highScoreStringPosition, Color::White);
	SpriteBatch::DrawString(scoreStream.str().c_str(), uiSystem->gameGUI->scoreStringPosition, Color::White);
}

void Pacman::PollInput()
{
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();
	S2D::Input::MouseState* mouseState = S2D::Input::Mouse::GetState();
	switch (states)
	{
	case Pacman::MAIN_MENU:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE)) {
			states = GameStates::GAME;
		}
		else if (keyboardState->IsKeyDown(S2D::Input::Keys::H)) {
			states = GameStates::HOW_TO_PLAY;
		}

		if (!hasIntermissionSoundPlayed) {
			Audio::Play(pacmanIntermissionSound);
			hasIntermissionSoundPlayed = true;
		}
		break;
	case Pacman::HOW_TO_PLAY:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE)) {
			states = GameStates::MAIN_MENU;
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

		Audio::Stop(pacmanIntermissionSound);
		hasIntermissionSoundPlayed = true;
		break;
	case Pacman::GAME_OVER:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::R)) {
			states = GameStates::GAME;
		}
		break;
	case Pacman::GAME_WIN:
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE)) {
			states = GameStates::GAME;
		}
		break;
	default:
		break;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::ESCAPE)) { //QUIT
		S2D::Graphics::Destroy();
	}


}

void Pacman::CheckGhostCollisions()
{
	for (auto& ghost : ghosts) {
		if (!ghost->frozen) {
			if (collisionInstance.CheckCollisions(pacman, ghost)) {
				states = GameStates::GAME_OVER;
				pacman->dead = true;

			}
		}
	}

}

void Pacman::CheckMunchieCollisions()
{
	for (int i = 0; i < MUNCHIE_COUNT; i++) {
		if (munchies[i] != nullptr) {
			if (collisionInstance.CheckCollisions(pacman, munchies[i])) {
				playerScore += 10;
				munchies[i] = nullptr;
				pacman->PlaySound(pacman->pacmanEatFruitSound);
			}
		}

	}
}


void Pacman::CheckCherryCollisions()
{
	if (cherry != nullptr) {
		if (collisionInstance.CheckCollisions(pacman, cherry)) {
			playerScore += 100;
			cherry->SetPosition(new Vector2(1100, 1100));
			cherry->SetTimer = true;
			pacman->PlaySound(pacman->pacmanEatFruitSound);
		}
	}
}


void Pacman::CheckBigMunchieCollisions()
{
	for (int i = 0; i < BIG_MUNCHIE_COUNT; i++) {
		if (bigMunchies[i] != nullptr) {
			if (collisionInstance.CheckCollisions(pacman, bigMunchies[i])) {
				pacman->SetPowerUp(true);
				bigMunchies[i] = nullptr;
				playerScore += 100;
				pacman->PlaySound(pacman->pacmanEatFruitSound);
				for (auto& ghost : ghosts) {
					ghost->frozen = true;
				}
			}
		}
	}
}

void Pacman::SaveScore()
{
	statsFile.close();
	statsFile.open("Assets/Data/Scores.txt", std::ifstream::out | std::ifstream::trunc);
	statsFile << playerScore;
}

void Pacman::LoadScore()
{
	std::string score;
	getline(statsFile, score);
	if (score != "") {
		playerScore = std::stoi(score);
	}
	else {
		playerScore = 0;
	}
}

void Pacman::DeleteScore()
{
	statsFile.clear();
	statsFile << 0;
}

