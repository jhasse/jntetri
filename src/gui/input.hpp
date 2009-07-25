#pragma once

#include "widget.hpp"

#include <string>
#include <vector>

class Input : public Widget {
public:
	Input(int x, int y);
	~Input();
	void Step();
	void Draw() const;
	void SetText(const std::string&);
	std::string GetText() const;
	void SetPassword(bool);
	void Focus();
private:
	mutable std::string text_;
	std::vector<std::string> unicodeChars_;
	int x_, y_;
	static Input* focus_;
	bool password_;
};
