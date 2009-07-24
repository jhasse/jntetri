#pragma once

#include <string>
#include <vector>

class Input {
public:
	Input(int x, int y);
	~Input();
	void Step();
	void Draw() const;
	std::string GetText() const;
	void SetPassword(bool);
private:
	mutable std::string text_;
	std::vector<std::string> unicodeChars_;
	int x_, y_;
	static Input* focus_;
	bool password_;
};
