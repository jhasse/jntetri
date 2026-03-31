#pragma once

#include "engine/work.hpp"
#include "engine/resizegraphics.hpp"

class Intro : public Work {
public:
	Intro();
	void step() override;
	void draw() const override;

private:
	int blink_;
	mutable bool finished_;
	mutable ResizeGraphics resizeGraphics_;
};
