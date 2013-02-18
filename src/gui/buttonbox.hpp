#pragma once

#include "button.hpp"

#include <vector>

class ButtonBox : public Widget {
public:
	ButtonBox(); // Creates a box at the center of the screen
	ButtonBox(int xCenter, int yCenter);
	void add(const std::string& text, std::function<void()>, char shortcut = ' ');
	void Draw() const;
	void Step();
	int GetMouseover() const; // Returns over which button the mouse is over. If there's none -1.
	virtual void OnAdd(Work&);
private:
	std::vector<std::shared_ptr<Button>> buttons_;
	const int xCenter_, yCenter_;
	const static int spacing_;
};
