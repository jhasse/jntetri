#pragma once

#include "gametype.hpp"

#include <list>
#include <memory>
#include <string>

struct Data {
	Data();
	int score;
	double time;
	std::string name;
};

bool operator==(const Data& a, const Data& b);

class Highscore {
public:
	Highscore(GameType type);
	bool isHighscore(Data data) const;
	void draw() const;
	void Add(Data);
	void save() const;
	void Blink(Data);
private:
	std::list<Data> highscores_;
	GameType type_;
	std::string filename_;
	std::shared_ptr<Data> blink_;
};
