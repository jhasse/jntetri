#include "chooser.hpp"

#include "../engine/screen.hpp"

#include <jngl.hpp>

Chooser::Chooser(const int x, const int y)
: previous_(
      "", [this]() { Previous(); }, "chooser_left", "chooser_left_over", "chooser_left_over"),
  next_(
      "", [this]() { Next(); }, "chooser_right", "chooser_right_over", "chooser_right_over"),
  sprite_("gfx/chooser") {
	setWidth(jngl::getWidth(sprite_));
	setHeight(jngl::getHeight(sprite_));
	setCenter(x, y);
	previous_.setCenter(
	    x - static_cast<double>(jngl::getWidth(sprite_)) / 2 - previous_.getWidth() / 2, y);
	next_.setCenter(x + static_cast<double>(jngl::getWidth(sprite_)) / 2 + next_.getWidth() / 2, y);
}

void Chooser::AddValue(int v) {
	values_.push_back(v);
	activeValue_ = values_.begin();
}

void Chooser::step() {
	previous_.setFocus(focus);
	next_.setFocus(focus);
	previous_.step();
	next_.step();
	if (focus) {
		if (jngl::keyPressed(jngl::key::Left)) {
			Previous();
		}
		if (jngl::keyPressed(jngl::key::Right)) {
			Next();
		}
	}
}

void Chooser::draw() const {
	jngl::draw(sprite_, getX(), getY());
	jngl::setFontSize(70);
	jngl::setFontColor(255, 255, 255);
	GetScreen().printCentered(std::to_string(*activeValue_), getCenter());
	previous_.draw();
	next_.draw();
}

int Chooser::GetValue() const {
	return *activeValue_;
}

void Chooser::Next() {
	++activeValue_;
	if (activeValue_ == values_.end()) {
		activeValue_ = values_.begin();
	}
}

void Chooser::Previous() {
	if (activeValue_ == values_.begin()) {
		activeValue_ = values_.end();
	}
	--activeValue_;
}
