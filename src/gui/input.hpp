#pragma once

#include <string>
#include <vector>

class Input {
public:
	Input(int x, int y);
	void Step();
	void Draw() const;
	std::string GetText() const;
private:
	std::string text_;
	std::vector<std::string> unicodeChars_;
	int x_, y_;
};
