#pragma once

#include "engine/work.hpp"

#include <memory>
#include <boost/noncopyable.hpp>
#include <fstream>

class Game;

class ReplayPlayer : public Work, boost::noncopyable {
public:
	ReplayPlayer(const std::string& filename);
	void Step();
	void Draw() const;
private:
	std::ifstream fin_;
	std::auto_ptr<Game> game_;
};
