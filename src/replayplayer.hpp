#pragma once

#include "engine/work.hpp"

#include <memory>
#include <boost/noncopyable.hpp>
#include <fstream>

class Game;

class ReplayPlayer : public Work, boost::noncopyable {
public:
	ReplayPlayer(const std::string& filename);
	~ReplayPlayer();
	void Step();
	void Draw() const;
private:
	std::ifstream fin_;
	std::unique_ptr<Game> game_;
	int oldStartLevel, oldStartJunk;
};
