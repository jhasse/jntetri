#pragma once

#include "button.hpp"

#include <vector>
#include <boost/function.hpp>

class ButtonBox {
public:
	ButtonBox(); // Creates a box at the center of the screen
	ButtonBox(int xCenter, int yCenter);
	void Add(const std::string& text, boost::function<void()>, char shortcut = ' ');
	void Draw() const;
	void Step();
	int GetMouseover() const; // Returns over which button the mouse is over. If there's none -1.
private:
	std::vector<Button> buttons_;
	const int xCenter_, yCenter_;
	const static int spacing_;
};
