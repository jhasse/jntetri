#pragma once

#include "gametype.hpp"

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>

struct Data {
	Data();
	int score;
	double time;
	std::string name;
};

bool operator==(const Data& a, const Data& b);
bool operator<(const Data& a, const Data& b);

class Highscore {
public:
	Highscore(GameType type);
	bool IsHighscore(Data data) const;
	void Draw() const;
	void Add(Data);
	void Save() const;
	void Blink(Data);
private:
	std::list<Data> highscores_;
	GameType type_;
	std::string filename_;
	boost::shared_ptr<Data> blink_;
};
