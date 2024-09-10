#pragma once

#include "engine/work.hpp"

#include <memory>
#include <fstream>

class Game;

class ReplayPlayer : public Work {
public:
	ReplayPlayer(const std::string& filename);
	ReplayPlayer(const ReplayPlayer&) = delete;
	ReplayPlayer(ReplayPlayer&&) = delete;
	ReplayPlayer& operator=(const ReplayPlayer&) = delete;
	ReplayPlayer& operator=(ReplayPlayer&&) = delete;
	~ReplayPlayer();
	void step();
	void draw() const;
private:
	std::ifstream fin_;
	std::shared_ptr<Game> game_;
	int oldStartLevel, oldStartJunk;
};
