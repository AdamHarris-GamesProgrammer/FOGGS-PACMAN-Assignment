#include "ScoreManager.h"

int ScoreManager::LoadScore()
{
	std::ifstream inFile("Assets/Data/Scores.txt"); //gets a ifstream to load the scores file
	char scoreText[8]; ///score can be a max of 8 characters, this shouldn't be an issue as that would require hundreds of wins in a row to get.

	if (!inFile) { //if the file failed to open
		std::cout << "Failed to open file" << std::endl;
	}
	else {
		inFile.get(scoreText, 8); //gets the line
		if (scoreText == "") { //if there is no score 
			mHighscore = 0; //sets score to 0
		}
		else {
			mHighscore = std::stoi(scoreText); //converts the line to a integer value so it can be set to mHighscore
		}
	}
	inFile.close(); //closes the file

	return mHighscore; //returns the highscore
}

void ScoreManager::SaveScore(int newValue) 
{
	std::ofstream outFile("Assets/Data/Scores.txt"); //opens the scores file for writing

	if (!outFile) { //if the file failed to open
		std::cout << "Failed to open file" << std::endl; 
	}
	else {
		outFile << newValue; //puts the new value into the file
	}

	outFile.close(); //closes the file
}
