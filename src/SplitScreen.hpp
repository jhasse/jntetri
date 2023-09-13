#pragma once

#include "engine/work.hpp"

class Field;
class ControlBase;

class SplitScreen : public Work {
public:
	SplitScreen(std::shared_ptr<ControlBase> opponentControl, int32_t seed);
	~SplitScreen();
	void step();
	void draw() const;
private:
	void reset(int32_t seed);

	std::unique_ptr<Field> field1;
	std::unique_ptr<Field> field2;
	int wins1 = 0;
	int wins2 = 0;
	int freezeCountdown = 0;
	std::shared_ptr<ControlBase> opponentControl;
};
