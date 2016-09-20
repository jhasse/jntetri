#include "input.hpp"
#include "../engine/screen.hpp"

#include <jngl/all.hpp>

Input::Input(int x, int y) : x_(x), y_(y), password_(false), displayCursor_(0) {
	unicodeChars_.push_back("ä");
	unicodeChars_.push_back("ö");
	unicodeChars_.push_back("ü");
	unicodeChars_.push_back("ß");
	unicodeChars_.push_back("§");
}

void Input::setMaxWidth(unsigned int width) {
	maxWidth = width;
}

void Input::SetPassword(bool password) {
	password_ = password;
}

void Input::OnFocusChanged() {
	displayCursor_ = 50;
}

void Input::step() {
	if (!sensitive_ || !focus_) {
		return;
	}
	--displayCursor_;
	if (displayCursor_ < -35) {
		displayCursor_ = 35;
	}
	if (jngl::getTextWidth(text) < maxWidth) {
		for (char c = ' '; c < '~' + 1; ++c) {
			if (jngl::keyPressed(c)) {
				displayCursor_ = 50;
				text += c;
			}
		}
		auto end = unicodeChars_.end();
		for (auto it = unicodeChars_.begin(); it != end; ++it) {
			if (jngl::keyPressed(*it)) {
				displayCursor_ = 50;
				text += *it;
			}
		}
	}
	if (jngl::keyPressed(jngl::key::BackSpace) && !text.empty()) {
		displayCursor_ = 50;
		auto it = text.end();
		--it;
		while (*it & 0x80 && !(*it & 0x40)) { // Unicode character?
			--it;
		}
		text.erase(it, text.end());
	}
}

void Input::draw() const {
	std::string temp;
	if (password_) {
		temp = text;
		size_t size = text.size();
		text.clear();
		for (size_t i = 0; i < size; ++i) {
			text += "●";
		}
	}
	if(sensitive_) {
		jngl::setFontColor(0, 0, 0);
	} else {
		jngl::setFontColor(150, 150, 150);
	}
	if (focus_ && displayCursor_ > 0) {
		jngl::print(text + "|", x_, y_);
	} else {
		jngl::print(text, x_, y_);
	}
	if (password_) {
		text = temp;
	}
}

std::string Input::getText() const {
	return text;
}

void Input::setText(const std::string& text) {
	this->text = text;
}
