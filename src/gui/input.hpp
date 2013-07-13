#pragma once

#include "widget.hpp"

#include <string>
#include <vector>

class Input : public Widget {
public:
	Input(int x, int y);
	~Input();
	void step() override;
	void draw() const override;
	void SetText(const std::string&);
	std::string GetText() const;
	void SetPassword(bool);
	virtual void OnFocusChanged();
private:
	mutable std::string text_;
	std::vector<std::string> unicodeChars_;
	int x_, y_;
	bool password_;
	int displayCursor_;
};
