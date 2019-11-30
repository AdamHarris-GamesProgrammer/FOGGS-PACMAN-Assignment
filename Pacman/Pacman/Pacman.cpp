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



	//Audio::Initialise(); //TODO: Uncomment this to enable sounds

	//Initialise important Game aspects
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

	delete uiSystem->gameGUI->extraPacmanSprite;
	delete uiSystem->gameGUI->extraPacmanSpritePosition;
	delete uiSystem->gameGUI->extraPacmanSpriteRect;
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

	ghostTexture = rl.LoadTexture("Assets/Textures/GhostBlue.png");
	for (auto & ghost : ghosts)
	{
		ghost = new Enemy(ghostTexture, GeneratePositionWithinGameBounds(), new S2D::Rect(0.0f, 0.0f, 20, 20));
		gameObjects.push_back(ghost);
	}

	munchieTexture = rl.LoadTexture("Assets/Textures/Munchie.png");
	// Load Munchie
	for (int i = 0; i < MUNCHIE_COUNT; i++) {
		munchies[i] = new Munchies(munchieTexture, GeneratePositionWithinGameBounds(), new Rect(0.0f, 0.0f, 12, 12));
	}
	gameObjects.push_back(pacman);
	gameObjects.push_back(cherry);
	
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

	uiSystem->gameGUI->extraPacmanSprite = rl.LoadTexture("Assets/Textures/Pacman.tga");
	uiSystem->gameGUI->extraPacmanSpritePosition = new Vector2(SCREEN_WIDTH / 4, SCREEN_HEIGHT - 32);
	uiSystem->gameGUI->extraPacmanSpriteRect = new Rect(0.0f, 0.0f, 32, 32);
	uiSystem->gameGUI->extraPacmanSpriteRect->Y = 64;
	uiSystem->gameGUI->extraPacmanSpriteRect->X = 32;

	playspaceTexture = rl.LoadTexture("Assets/Textures/Playspace.png");

}

void Pacman::Update(int elapsedTime)
{
	PollInput();

	if (!isPaused || !isGameStarted) {
		frameCount++;
	}
	if (!isGameStarted || isPaused) {
		if (!hasIntermissionSoundPlayed) {
			Audio::Play(pacmanIntermissionSound);
			hasIntermissionSoundPlayed = true;
		}
	}
	else if (isGameStarted && !hasIntroMusicPlayed) {
		Audio::Stop(pacmanIntermissionSound);
		Audio::Play(pacmanBeginningSound);
		hasIntroMusicPlayed = true;
		hasIntermissionSoundPlayed = false;
	}
	if (isGameStarted && !isPaused) {
		for (int i = 0; i < MUNCHIE_COUNT; i++) {
			if(munchies[i] != nullptr)
				munchies[i]->Update(elapsedTime, frameCount);
		}

		for each (GameObject*  object in gameObjects)
		{
			object->Update(elapsedTime, frameCount);
		}
		if(!pacman->invincible)
			CheckGhostCollisions();
		CheckMunchieCollisions();
	}
}

void Pacman::Draw(int elapsedTime)
{
	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(playspaceTexture, new Vector2(32, 32), new Rect(0.0f, 0.0f, 960, 704));
	if (isGameStarted) {
		//SpriteBatch::Draw(playspaceTexture, new Rect(32.0f,32.0f,960,704));
		for each (GameObject * object in gameObjects)
		{
			if(object != pacman)
				object->Render();
			else{
				pacman->Render();
			}
		}
		for each (Munchies* munchie in munchies)
		{
			munchie->Render();
		}

		if (frameCount < PREFFERRED_FPS / 2)
		{
			if (!hasChompedSoundPlayed) {
				pacman->PlaySound(pacman->pacmanChompSound);
				hasChompedSoundPlayed = true;
			}
		}
		else
		{
			if (frameCount >= PREFFERRED_FPS)
				frameCount = 0;
			hasChompedSoundPlayed = false;
		}
		DrawGUI();
	}
	else {
		SpriteBatch::Draw(uiSystem->startMenu->startMenuLogo, uiSystem->startMenu->startMenuPosition, uiSystem->startMenu->startMenuRectangle);
	}

	if (isPaused) {
		SpriteBatch::Draw(uiSystem->pauseScreen->pauseScreenBackground, uiSystem->pauseScreen->pauseScreenRectangle, nullptr);
	}

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

	SpriteBatch::Draw(uiSystem->gameGUI->extraPacmanSprite, uiSystem->gameGUI->extraPacmanSpritePosition, uiSystem->gameGUI->extraPacmanSpriteRect);
	SpriteBatch::Draw(uiSystem->gameGUI->extraPacmanSprite, new Vector2(uiSystem->gameGUI->extraPacmanSpritePosition->X + 32, uiSystem->gameGUI->extraPacmanSpritePosition->Y), uiSystem->gameGUI->extraPacmanSpriteRect);

}

void Pacman::PollInput()
{
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();
	S2D::Input::MouseState* mouseState = S2D::Input::Mouse::GetState();

	if (keyboardState->IsKeyDown(S2D::Input::Keys::P) && !ispKeyDown) { //PAUSE
		ispKeyDown = true;
		isPaused = !isPaused;
	}

	if (keyboardState->IsKeyUp(S2D::Input::Keys::P)) { //allows pause to toggle 
		ispKeyDown = false;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::ESCAPE)) { //QUIT
		S2D::Graphics::Destroy();
	}

	if (isGameStarted) {
		if (!isPaused) {
			pacman->PollInput();
		}
	}
	else {
		if (keyboardState->IsKeyDown(S2D::Input::Keys::SPACE)) { //START GAME
			std::cout << "space pressed: " << isGameStarted << std::endl;
			isGameStarted = true;
		}
	}
}

Vector2* Pacman::GeneratePositionWithinGameBounds()
{
	int randX = rand() % 992 + 48;
	int randY = rand() % 736 + 48;
	if (randX >= 960) {
		randX -= 128;
	}
	if (randY >= 704) {
		randY -= 96;
	}

	return new Vector2(randX, randY);
}

void Pacman::CheckGhostCollisions()
{
	for (auto & ghost : ghosts) {
		if (collisionInstance.CheckCollisions(pacman, ghost)) {
			pacman->dead = true;
			if (!deathSoundPlayed) {
				pacman->PlaySound(pacman->pacmanMunchSound);
				if (pacman->pacmanExtraPacSound->GetState() == SoundEffectState::PLAYING) {
					deathSoundPlayed = true;
				}
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

