#include "input.hpp"

#include <jngl.hpp>

Input::Input(int x, int y) : password_(false), displayCursor_(0) {
	setPos(x, y);
}

void Input::setMaxWidth(unsigned int width) {
	maxWidth = width;
}

void Input::SetPassword(bool password) {
	password_ = password;
}

void Input::onFocusChanged() {
	displayCursor_ = 50;
}

void Input::step() {
	const auto mousePos = jngl::getMousePos();
	if (mousePos.x > position.x && mousePos.y > position.y && mousePos.x < position.x + maxWidth &&
	    mousePos.y < position.y + 94 /* TODO: Do not hardcode */) {
		jngl::setCursor(jngl::Cursor::I);
		if (jngl::mousePressed()) {
			setFocus(true);
		}
	}
	if (!sensitive || !focus) {
		return;
	}
	--displayCursor_;
	if (displayCursor_ < -35 || !jngl::getTextInput().empty()) {
		displayCursor_ = 35;
	}
	text += jngl::getTextInput();
	if (jngl::keyPressed(jngl::key::BackSpace) && !text.empty()) {
		removeLastCharacter();
	}
	while (jngl::getTextWidth(text) > maxWidth) {
		removeLastCharacter();
	}
}

void Input::draw() const {
	std::string temp;
	if (password_) {
		temp = text;
		size_t size = jngl::utf8Length(text);
		text.clear();
		for (size_t i = 0; i < size; ++i) {
			text += "●";
		}
	}
	if (sensitive) {
		jngl::setFontColor(0, 0, 0);
	} else {
		jngl::setFontColor(150, 150, 150);
	}
	if (focus && displayCursor_ > 0) {
		jngl::print(text + "|", static_cast<int>(std::lround(position.x)),
		            static_cast<int>(std::lround(position.y)));
	} else {
		jngl::print(text, static_cast<int>(std::lround(position.x)),
		            static_cast<int>(std::lround(position.y)));
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

void Input::removeLastCharacter() {
	displayCursor_ = 50;
	auto it = text.end();
	--it;
	while (*it & 0x80 && !(*it & 0x40)) { // Unicode character?
		--it;
	}
	text.erase(it, text.end());
}
