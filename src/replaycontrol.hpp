#pragma once

#include "control.hpp"

#include <queue>
#include <iosfwd>

class ReplayControl : public ControlBase {
public:
	ReplayControl(std::ifstream&);
	void step(const std::function<void(ControlType)>&) override;

private:
	int time_;
	std::queue<std::pair<unsigned char, ControlType>> data_;
};
