#pragma once

#include <fstream>
#include <iostream>
#include <string>

class ScoreManager
{
private:
	int mHighscore = 0;

public:
	int GetHighscore() { return LoadScore(); }
	void SetHighscore(int score) { SaveScore(score); }

	int LoadScore();
	void SaveScore(int newValue);
};

