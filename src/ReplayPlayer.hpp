#pragma once

#include "engine/work.hpp"

#include <sstream>
#include <memory>

class Game;

class ReplayPlayer : public Work {
public:
	explicit ReplayPlayer(const std::string& filename);
	ReplayPlayer(const ReplayPlayer&) = delete;
	ReplayPlayer(ReplayPlayer&&) = delete;
	ReplayPlayer& operator=(const ReplayPlayer&) = delete;
	ReplayPlayer& operator=(ReplayPlayer&&) = delete;
	~ReplayPlayer() override;
	void step() override;
	void draw() const override;

private:
	std::istringstream fin_;
	std::shared_ptr<Game> game_;
	int oldStartLevel, oldStartJunk;
};
