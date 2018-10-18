#pragma once

#include "control.hpp"
#include "gametype.hpp"

#include <fstream>

class Field;

class ReplayRecorder {
public:
	ReplayRecorder(Field&, GameType);
	void Step();

private:
	void Write(control::ControlType);
	std::ofstream fout;
	Field& field_;
	unsigned char time_;
};
