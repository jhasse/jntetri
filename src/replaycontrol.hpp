#pragma once

#include "control.hpp"

#include <queue>
#include <iosfwd>

class ReplayControl : public Control {
public:
	ReplayControl(std::ifstream&);
	void Step();
private:
	unsigned int time_;
	std::queue<std::pair<unsigned char, control::ControlType> > data_;
};
