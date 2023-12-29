#include "ButtonBox.hpp"

#include "../engine/work.hpp"
#include "Button.hpp"

const int ButtonBox::spacing_ = 30;

ButtonBox::ButtonBox(const int xCenter, const int yCenter)
: xCenter_(xCenter), yCenter_(yCenter) {
	setSensitive(false);
}

ButtonBox::ButtonBox()
: xCenter_(0), yCenter_(0) {
	setSensitive(false);
}

void ButtonBox::add(const std::string& text, std::function<void()> function) {
	buttons_.push_back(std::make_shared<Button>(text, function));

	auto end = buttons_.end();
	int yPosButton = yCenter_ -
	                 (buttons_.size() * (buttons_[0]->getHeight() + spacing_) - spacing_) / 2 +
	                 buttons_[0]->getHeight() / 2;
	for (auto it = buttons_.begin(); it != end; ++it) {
		(*it)->setCenter(xCenter_, yPosButton);
		yPosButton += buttons_[0]->getHeight() + spacing_;
	}
}

void ButtonBox::draw() const {
}

void ButtonBox::step() {
}

void ButtonBox::onAdd(Work& work) {
	for (auto& button : buttons_) {
		work.addWidget(button);
	}
}
