#pragma once

#include "widget.hpp"

#include <functional>
#include <memory>
#include <vector>

class Button;

class ButtonBox : public Widget {
public:
	ButtonBox(); // Creates a box at the center of the screen
	ButtonBox(int xCenter, int yCenter);
	void add(const std::string& text, std::function<void()>);
	void draw() const override;
	void step() override;

	void onAdd(Work&) override;

private:
	std::vector<std::shared_ptr<Button>> buttons_;
	const int xCenter_, yCenter_;
	const static int spacing_;
};
