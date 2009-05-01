#pragma once

#include "gametype.hpp"

#include <list>
#include <string>

struct Data {
	int score;
	double time;
	std::string name;
};

bool operator<(const Data& a, const Data& b);

class Highscore {
public:
	Highscore(GameType type);
	bool IsHighscore(Data data) const;
	void Draw() const;
	void Add(Data);
	void Save() const;
private:
	std::list<Data> highscores_;
	GameType type_;
	std::string filename_;
};
