#pragma once

#include <fstream>
#include <iostream>
#include <string>

class ScoreManager
{
private:
	int mHighscore = 0;// stores the highscore

public:
	int GetHighscore() { return LoadScore(); } //gets the highscore
	void SetHighscore(int score) { SaveScore(score); } //sets the highscore

	int LoadScore(); //loads the score
	void SaveScore(int newValue); //saves the score
};

