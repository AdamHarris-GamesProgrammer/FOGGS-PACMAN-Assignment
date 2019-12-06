#include "ScoreManager.h"

int ScoreManager::LoadScore()
{
	std::ifstream inFile("Assets/Data/Scores.txt");
	char scoreText[8];

	if (!inFile) {
		std::cout << "Failed to open file" << std::endl;
	}
	else {
		inFile.get(scoreText, 8);
		if (scoreText == "") {
			mHighscore = 0;
		}
		else {
			mHighscore = std::stoi(scoreText);
		}
	}
	inFile.close();

	return mHighscore;
}

void ScoreManager::SaveScore(int newValue)
{
	std::ofstream outFile("Assets/Data/Scores.txt");

	if (!outFile) {
		std::cout << "Failed to open file" << std::endl;
	}
	else {
		outFile << newValue;
	}

	outFile.close();
}
