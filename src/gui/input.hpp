#pragma once

#include "widget.hpp"

#include <string>
#include <vector>

class Input : public Widget {
public:
	Input(int x, int y);

	void step() override;
	void draw() const override;

	std::string getText() const;
	void setText(const std::string&);

	void setMaxWidth(unsigned int);

	void SetPassword(bool);

	void OnFocusChanged() override;

private:
	mutable std::string text;
	std::vector<std::string> unicodeChars_;
	int x_, y_;
	bool password_;
	int displayCursor_;
	unsigned int maxWidth = 999;
};
