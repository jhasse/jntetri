#include "input.hpp"

#include <jngl.hpp>

Input::Input(const jngl::Vec2 position) : displayCursor(0) {
	setPos(position);
}

void Input::setMaxWidth(unsigned int width) {
	maxWidth = width;
}

void Input::setPassword(bool password) {
	this->password = password;
}

void Input::onFocusChanged() {
	displayCursor = 50;
}

void Input::step() {
	const auto mousePos = jngl::getMousePos();
	if (mousePos.x > getX() && mousePos.y > getY() && mousePos.x < getX() + maxWidth &&
	    mousePos.y < getY() + 94 /* TODO: Do not hardcode */) {
		jngl::setCursor(jngl::Cursor::I);
		if (jngl::mousePressed()) {
			setFocus(true);
		}
	}
	if (!sensitive || !focus) {
		return;
	}
	--displayCursor;
	if (displayCursor < -35 || !jngl::getTextInput().empty()) {
		displayCursor = 35;
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
	if (password) {
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
	if (focus && displayCursor > 0) {
		jngl::print(text + "|", getPos());
	} else {
		jngl::print(text, getPos());
	}
	if (password) {
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
	displayCursor = 50;
	auto it = text.end();
	--it;
	while (*it & 0x80 && !(*it & 0x40)) { // Unicode character?
		--it;
	}
	text.erase(it, text.end());
}
