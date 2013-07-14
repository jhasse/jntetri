#pragma once

#include "engine/work.hpp"
#include "field.hpp"

class SplitScreen : public Work {
public:
	SplitScreen();
	void step();
	void draw() const;
private:
	void reset();

	std::unique_ptr<Field> field1;
	std::unique_ptr<Field> field2;
	int wins1;
	int wins2;
};
