#pragma once

#include "control.hpp"

#include <queue>
#include <iosfwd>

class ReplayControl : public ControlBase {
public:
	ReplayControl(std::ifstream&);
	void step(std::function<void(control::ControlType)>);
private:
	int time_;
	std::queue<std::pair<unsigned char, control::ControlType>> data_;
};
