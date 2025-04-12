#pragma once

#include "widget.hpp"

#include <string>

class Input : public Widget {
public:
	Input(int x, int y);

	void step() override;
	void draw() const override;

	std::string getText() const;
	void setText(const std::string&);

	void setMaxWidth(unsigned int);

	void setPassword(bool);

	void onFocusChanged() override;

private:
	void removeLastCharacter();

	mutable std::string text;
	bool password = false;
	int displayCursor;
	unsigned int maxWidth = 999;
};
