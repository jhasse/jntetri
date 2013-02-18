#pragma once

#include "engine/work.hpp"
#include "field.hpp"

class SplitScreen : public Work {
public:
	SplitScreen();
	void step();
	void draw() const;
private:
	Field field1;
	Field field2;
};